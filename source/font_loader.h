#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>
#include <stdexcept>
#include <jni.h>

const unsigned char first_char = 33;
const unsigned char last_char = 126;

struct ivec2 {
    int x;
    int y;

    ivec2(int x, int y) : x(x), y(y) {}
};

struct Character {
    ivec2 size;       // Size of glyph
    ivec2 bearing;    // Offset from baseline to left/top of glyph
    unsigned int advance;
    // Offset to advance to next glyph
    float offset;
    float height;
};

void check_exception(JNIEnv *env, const std::string &message) {
  jboolean flag = env->ExceptionCheck();
  if (flag) {
    env->ExceptionClear();
    throw std::runtime_error(message);
  }
}

FT_Face load_font(const FT_Library &library, const char *filename) {
  FT_Face face;
  auto error = FT_New_Face(library, filename, 0, &face);
  if (error)
    throw std::runtime_error(std::string("ERROR::FREETYPE: Failed to load font: ") + filename);

  FT_Set_Pixel_Sizes(face, 0, 48);
  return face;
}

ivec2 get_texture_dimensions(FT_Face face) {
  unsigned int max_width = 0;
  unsigned int offset = 0;

  for (unsigned char c = first_char; c <= last_char; c++) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0)
      throw std::runtime_error("Failed to load glyph");

    if (!face->glyph->bitmap.buffer)
      FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

    auto glyph = face->glyph;
    auto bitmap = glyph->bitmap;
    if (bitmap.width > max_width)
      max_width = bitmap.width;

    offset += bitmap.rows + 2;
  }

  return {(int) max_width + 1, (int) offset};
}

void render_font(JNIEnv *env, const FT_Library &library, FT_Face face, jobject character_map, unsigned char *buffer,
                 int width,
                 int height) {
  int memory_offset = 0;
  float vertical_offset = 0;
  const int memory_margin = width * 2;

//  auto map_class = env->GetObjectClass(character_map);
  auto map_class = env->FindClass("java/util/HashMap");
  auto method = env->GetMethodID(map_class, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
  check_exception(env, "Could not load Map.put method.");
  auto character_class = env->FindClass("mythic/typography/CharacterInfo");
  auto constructor = env->GetMethodID(character_class, "<init>", "(IIIIIFF)V");

  for (unsigned char c = first_char; c <= last_char; c++) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0)
      throw std::runtime_error("Failed to load glyph");

    // A hack to work around an issue where the bitmap is not already rendered in certain cases.
    // This happens on the 32 bit Windows build  but not the 64 bit build.
    if (!face->glyph->bitmap.buffer)
      FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

    auto glyph = face->glyph;
    auto bitmap = face->glyph->bitmap;

    auto character = env->NewObject(character_class, constructor,
                                    bitmap.width, bitmap.rows,
                                    glyph->bitmap_left, glyph->bitmap_top,
                                    glyph->advance.x,
                                    vertical_offset / height,
                                    (float) bitmap.rows / height
    );
    env->CallVoidMethod(character_map, method, (jchar)c, character);

    memory_offset++;
    for (int i = 0; i < bitmap.rows; i++) {
      memcpy(buffer + memory_offset, bitmap.buffer + i * bitmap.width, bitmap.width);
      memory_offset += width;
    }
    memory_offset--;
    memory_offset += memory_margin; // margin between characters
    vertical_offset += bitmap.rows + 2;
  }
}