cmake_minimum_required(VERSION 3.8)
project(java_freetype)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES source/interface.cpp source/interface.h)
add_library(java_freetype ${SOURCE_FILES})