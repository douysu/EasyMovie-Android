#include "jni.h"
#include <android/log.h>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>

#define ehome_printf(format, ...)  \
	__android_log_print(ANDROID_LOG_DEBUG,  "jni_debug", format, ##__VA_ARGS__)

static JavaVM *ms2_vm = NULL;
static jobject g_obj = NULL;
static jmethodID g_mt_s = NULL;
static jmethodID g_mt_u = NULL;
static jmethodID g_mt_r = NULL;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    ms2_vm = vm;
    ehome_printf("[%s]getpid=%d, gettid=%d\n", __FUNCTION__, getpid(),gettid());
    return JNI_VERSION_1_4;
}

bool get_env(JNIEnv ** env) {
    int status = ms2_vm->GetEnv((void**) env, JNI_VERSION_1_4);
    if (status != JNI_OK) {
        ehome_printf("[%s]getpid=%d, gettid=%d\n", __FUNCTION__, getpid(),gettid());
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
        JNIEnv *env;
        if (!get_env(&env)) {
            ehome_printf("[%s]get_env error!\n", __FUNCTION__);
            return;
        }

        env->CallVoidMethod(g_obj, g_mt_s, unityTextureId, width, height);

        release_env();
        ehome_printf("start function is end");
    }

    void release()
    {
        JNIEnv *env;
        if (!get_env(&env)) {
            ehome_printf("[%s]get_env error!\n", __FUNCTION__);
            return;
        }

        env->CallVoidMethod(g_obj, g_mt_r);

        release_env();
        ehome_printf("release function is end");
    }

    void updateTexture()
    {
        JNIEnv *env;
        if (!get_env(&env)) {
            ehome_printf("[%s]get_env error!\n", __FUNCTION__);
            return;
        }

        env->CallVoidMethod(g_obj, g_mt_u);

        release_env();
        ehome_printf("updateTexture function is end");
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_pvr_videoplugin_VideoPlugin_init(
        JNIEnv* env,
jobject obj) {
    // 初始化缓存
    // 此处采用较为简单的全局变量，应采用其他缓存方式处理
    g_obj = env->NewGlobalRef(obj);

    jclass clazz = env->FindClass("com/pvr/videoplugin/VideoPlugin");
    jmethodID mt_s = env->GetMethodID(clazz, "start", "(III)V");
    jmethodID mt_u = env->GetMethodID(clazz, "updateTexture", "()V");
    jmethodID mt_r = env->GetMethodID(clazz, "release", "()V");
    g_mt_s = mt_s;
    g_mt_u = mt_u;
    g_mt_r = mt_r;

    ehome_printf("InitObject function is end");
}