#include <jni.h>
#include <string>
#include <sstream>
#include <thread>   // для std::thread
#include <chrono>   // для std::this_thread::sleep_for
#include <future>

using std::string;

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_learncpp_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    string result = "Hello Doodle!";
    return env->NewStringUTF(result.c_str());
}