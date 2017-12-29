#pragma once

#include <jni.h>

#if defined (_MSC_VER)
#define JNIEXPORT __declspec( dllexport )
#else
#define JNIEXPORT
#endif

extern "C" {

JNIEXPORT void JNICALL Java_mythic_MythicInterface_initialize(JNIEnv *env, jobject self);
//JNIEXPORT int JNICALL Java_mythic_MythicInterface_update(JNIEnv *env, jobject self);
JNIEXPORT void JNICALL Java_mythic_MythicInterface_shutdown(JNIEnv *env, jobject self);
JNIEXPORT void JNICALL Java_mythic_MythicInterface_loop(JNIEnv *env, jobject self);

//JNIEXPORT jobject JNICALL Java_mythic_MythicInterface_allocate(JNIEnv *env, jobject self, jobject byte_count);

JNIEXPORT int JNICALL Java_mythic_MythicInterface_createMesh(JNIEnv *env, jobject self, jlong mesh);

}
