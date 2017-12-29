#include "interface.h"
#include <iostream>
#include <desktop/Desktop.h>
#include "lookinglass/Graphic_Options.h"
#include <clienting/Mythic_Client.h>
#include <Marloth_Myth.h>

using namespace mythic;

#ifndef GAME_WIDTH
#define GAME_WIDTH 900
#endif

#ifndef GAME_HEIGHT
#define GAME_HEIGHT 600
#endif

static desktop::Desktop desk("Marloth");
std::unique_ptr<Marloth_Myth> marloth_myth;

JNIEXPORT void JNICALL Java_mythic_MythicInterface_initialize(JNIEnv *env, jobject self) {
  try {
    lookinglass::Graphic_Options graphic_options(GAME_WIDTH, GAME_HEIGHT);
    graphic_options.set_multisamples(4);
    marloth_myth.reset(new Marloth_Myth(desk, graphic_options));
    marloth_myth->load();
  }
  catch (const std::exception &e) {
    jclass jc = env->FindClass("java/lang/Error");
    env->ThrowNew(jc, e.what());
  }
}

//JNIEXPORT int JNICALL Java_mythic_MythicInterface_update(JNIEnv *env, jobject self) {
//  if (!marloth_myth->update())
//    return 1;
//
//  return 0;
//}

JNIEXPORT void JNICALL Java_mythic_MythicInterface_loop(JNIEnv *env, jobject self) {
  try {
    marloth_myth->loop();
  }
  catch (const std::exception &e) {
    jclass jc = env->FindClass("java/lang/Error");
    env->ThrowNew(jc, e.what());
  }
}

JNIEXPORT void JNICALL Java_mythic_MythicInterface_shutdown(JNIEnv *env, jobject self) {
  try {
    marloth_myth.reset();
  }
  catch (const std::exception &e) {
    jclass jc = env->FindClass("java/lang/Error");
    env->ThrowNew(jc, e.what());
  }
}

//JNIEXPORT jobject JNICALL Java_mythic_MythicInterface_allocate(JNIEnv *env, jobject self, jobject byte_count) {
//  return new char[byte_count];
//}

struct Raw_Mesh {
    int32 vertex_float_count;
    int32 face_count;
    float *vertices;
    float *offsets;
    float *counts;
};

JNIEXPORT int JNICALL Java_mythic_MythicInterface_createMesh(JNIEnv *env, jobject self, jlong mesh_pointer) {
  auto mesh = (Raw_Mesh *) (void *) mesh_pointer;
//  jclass jRawMesh = env->GetObjectClass(mesh);
//  jfieldID field_vertexSize = env->GetFieldID(jRawMesh, "")
  return 0;
}

//JNIEXPORT int JNICALL Java_mythic_MythicInterface_resources_add_mesh(const char *name, sculpting::Raw_Mesh *mesh,
//                                                  sculpting::Array<sculpting::Raw_Color_Info> *face_details) {
//  auto &renderer = marloth_myth->get_client().get_renderer();
//  auto &vertex_schema = renderer.get_shader_library().get_basic_scenery_vertex_schema();
////  renderer.get_model_manager().add_mesh(name, mesh, face_details, vertex_schema);
//  return 0;
//}

//JNIEXPORT void JNICALL Java_mythic_MythicInterface_test(JNIEnv *env, jobject self, jlong pointer) {
//  auto l = (__int64) pointer;
//  auto local_pointer = reinterpret_cast<char const *>(l);
//  delete local_pointer;
//}