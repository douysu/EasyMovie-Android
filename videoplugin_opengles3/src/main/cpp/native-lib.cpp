#include "jni.h"
#include <android/log.h>
#include <cstdio>

#define ehome_printf(format, ...)  \
	__android_log_print(ANDROID_LOG_DEBUG,  "jni_debug", format, ##__VA_ARGS__)

extern "C"
{
    int addInt(int a, int b)
    {
        return a + b;
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_pvr_videoplugin_VideoPlugin_testRun(
        JNIEnv* env,
jobject obj) {
    ehome_printf("yindouyindou");
}