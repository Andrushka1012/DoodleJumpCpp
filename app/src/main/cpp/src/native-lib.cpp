#include <jni.h>
#include <string>
#include <sstream>
#include <thread>   // для std::thread
#include <chrono>   // для std::this_thread::sleep_for
#include <future>
#include "engine/engine.h"
#include "engine/renderer.h"

using std::string;

extern "C" JNIEXPORT void JNICALL
Java_com_example_learncpp_MainActivity_initRenderer(
        JNIEnv* env,
        jobject /* this */) {
    Engine::startEngine();
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_learncpp_MainActivity_renderFrame(
        JNIEnv* env,
        jobject /* this */) {
    Engine::renderFrame();
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_learncpp_MainActivity_setViewport(
        JNIEnv* env,
        jobject /* this */,
        jint width,
        jint height) {
    Engine::setViewport(width, height);
}