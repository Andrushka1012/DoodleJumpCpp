#include <jni.h>
#include <string>
#include <sstream>
#include <thread>   // для std::thread
#include <chrono>   // для std::this_thread::sleep_for
#include <future>
#include "engine/engine.h"
#include "engine/renderer.h"

using std::string;

JavaVM* g_javaVM = nullptr;  // Global variable

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    g_javaVM = vm;  // Store the JavaVM pointer for later use
    return JNI_VERSION_1_6;
}

void notifyGameOver(float score) {
    if (!g_javaVM) return;  // safety check

    JNIEnv* env = nullptr;
    jint result = g_javaVM->AttachCurrentThread(&env, nullptr);
    if (result != JNI_OK || env == nullptr) return;

    jclass clazz = env->FindClass("com/example/learncpp/MainActivity");
    if (!clazz) return;

    jmethodID methodId = env->GetStaticMethodID(clazz, "notifyGameOver", "(F)V");
    if (!methodId) return;

    env->CallStaticVoidMethod(clazz, methodId, score);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_learncpp_MainActivity_startEngine(
        JNIEnv *env,
        jobject /* this */) {

    auto onGameOverCallback = [](float score) {
        notifyGameOver(score);
    };

    DoodleJumpGame::startEngine(onGameOverCallback);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_learncpp_MainActivity_drawFrame(
        JNIEnv *env,
        jobject /* this */) {
    DoodleJumpGame::drawFrame();
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_learncpp_MainActivity_setViewport(
        JNIEnv *env,
        jobject /* this */,
        jint width,
        jint height) {
    DoodleJumpGame::setViewport(width, height);
}


extern "C" JNIEXPORT void JNICALL
Java_com_example_learncpp_MainActivity_onHorizontalMove(
        JNIEnv *env,
        jobject /* this */,
        jfloat x) {
    DoodleJumpGame::onHorizontalMove(x);
}
extern "C" JNIEXPORT void JNICALL
Java_com_example_learncpp_MainActivity_startGame(
        JNIEnv *env,
        jobject /* this */) {
    DoodleJumpGame::startGame();
}
