cmake_minimum_required(VERSION 3.8)
project(java_freetype)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--add-stdcall-alias")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")

set(BUILD_SHARED_LIBS OFF)
add_subdirectory("freetype-2.8.1")

set(SOURCE_FILES source/interface.cpp source/font_loader.h)

include_directories(
  "${JNI_INCLUDE_PATH}"
  "${JNI_INCLUDE_PATH}/win32"
  "freetype-2.8.1/include"
  "freetype-2.8.1/include/freetype"
)
set(BUILD_SHARED_LIBS ON)

add_library(java_freetype ${SOURCE_FILES})
target_link_libraries(java_freetype freetype)
set_target_properties(java_freetype PROPERTIES PREFIX "lib")
