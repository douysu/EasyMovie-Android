#include "jni.h"
#include <android/log.h>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>

#define ehome_printf(format, ...)  \
	__android_log_print(ANDROID_LOG_DEBUG,  "jni_debug", format, ##__VA_ARGS__)

static JavaVM *ms2_vm = NULL;
static jobject g_obj;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    ms2_vm = vm;
    return JNI_VERSION_1_4;
}

bool get_env(JNIEnv ** env) {
    int status = ms2_vm->GetEnv((void**) env, JNI_VERSION_1_4);
    if (status != JNI_OK) {
        status = ms2_vm->AttachCurrentThread(env, NULL);
        if(status != JNI_OK){
            ehome_printf("[%s]FAILED\n", __FUNCTION__);
            return false;
        }
        ehome_printf("[%s]SUCCESS\n", __FUNCTION__);
    }else{
        ehome_printf("[%s]Attach aready\n", __FUNCTION__);
    }
    return true;
}

void release_env(void) {
    JNIEnv *env ;
    int status = ms2_vm->GetEnv((void**)&env, JNI_VERSION_1_4);
    if (status == JNI_EDETACHED) {
        ehome_printf("[%s]getpid=%d, gettid=%d\n", __FUNCTION__, getpid(),gettid());
        ms2_vm->DetachCurrentThread();
        ehome_printf("Release success");
    }else{
        ehome_printf("[%s]NEED NOT DETACH\n", __FUNCTION__);
    }
}

extern "C"
{
    void start(int unityTextureId, int width, int height)
    {
        ehome_printf("start function is start");
        JNIEnv *env;
        ehome_printf("get env start");
        if (!get_env(&env)) {
            ehome_printf("[%s]get_env error!\n", __FUNCTION__);
            return;
        }
        ehome_printf("[%s]GetVersion=%d\n", __FUNCTION__, env->GetVersion());

        jclass clazz = env->FindClass("com/pvr/videoplugin/VideoPlugin");
        jmethodID mt = env->GetMethodID(clazz, "start", "(III)V");
        env->CallVoidMethod(g_obj, mt, unityTextureId, width, height);

        // release_env();
        ehome_printf("start function is end");
    }

    void release()
    {
        ehome_printf("release function is start");
        JNIEnv *env;
        if (!get_env(&env)) {
            ehome_printf("[%s]get_env error!\n", __FUNCTION__);
            return;
        }
        ehome_printf("[%s]GetVersion=%d\n", __FUNCTION__, env->GetVersion());

        jclass clazz = env->FindClass("com/pvr/videoplugin/VideoPlugin");
        jmethodID mt = env->GetMethodID(clazz, "release", "()V");
        env->CallVoidMethod(g_obj, mt);

        // release_env();
        ehome_printf("release function is end");
    }

    void updateTexture()
    {
        ehome_printf("updateTexture function is start");
        JNIEnv *env;
        if (!get_env(&env)) {
            ehome_printf("[%s]get_env error!\n", __FUNCTION__);
            return;
        }
        ehome_printf("[%s]GetVersion=%d\n", __FUNCTION__, env->GetVersion());

        jclass clazz = env->FindClass("com/pvr/videoplugin/VideoPlugin");
        jmethodID mt = env->GetMethodID(clazz, "updateTexture", "()V");
        env->CallVoidMethod(g_obj, mt);

        // release_env();
        ehome_printf("updateTexture function is end");
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_pvr_videoplugin_VideoPlugin_initObject(
        JNIEnv* env,
jobject obj) {
    ehome_printf("InitObject Function is run");
    g_obj = env->NewGlobalRef(obj);
}