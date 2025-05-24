#include <jni.h>
#include <string>
#include <sstream>
#include <thread>   // для std::thread
#include <chrono>   // для std::this_thread::sleep_for
#include <future>
#include "engine/engine.h"

using std::string;

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_learncpp_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    int result = Engine::sum(5, 10);
    string message = "Hello Doodle! Sum: " + std::to_string(result);
    return env->NewStringUTF(message.c_str());
}