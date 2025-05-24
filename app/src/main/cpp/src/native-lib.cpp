#include <jni.h>
#include <string>
#include <sstream>
#include <thread>   // для std::thread
#include <chrono>   // для std::this_thread::sleep_for
#include <future>
#include "engine/engine.h"
#include "engine/renderer.h"

using std::string;

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_learncpp_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    int result = Engine::sum(5, 10);
    string message = "Hello Doodle! Sum: " + std::to_string(result);
    return env->NewStringUTF(message.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_learncpp_MainActivity_initRenderer(
        JNIEnv* env,
        jobject /* this */) {
    Engine::initRenderer();
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