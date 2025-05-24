#include <jni.h>
#include <string>
#include <sstream>
#include <thread>   // для std::thread
#include <chrono>   // для std::this_thread::sleep_for
#include <future>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_learncpp_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    // Создаем promise для получения результата из потока
    std::promise<std::string> resultPromise;
    std::future<std::string> resultFuture = resultPromise.get_future();

    // Запускаем поток
    std::thread([promise = std::move(resultPromise)]() mutable {
        // Имитация длительной работы
        for (int i = 0; i < 1000000000; ++i) {
            // Пустой цикл для имитации работы
        }

        // Устанавливаем результат
        promise.set_value("Result from background thread!");
    }).detach();

    // Ждем результат из потока
    std::string result = resultFuture.get();

    // Возвращаем результат
    return env->NewStringUTF(result.c_str());
}


extern "C" JNIEXPORT void JNICALL
Java_com_example_learncpp_MainActivity_stringFromJNIAsync(
        JNIEnv* env, jobject thiz, jobject callback) {

    // Создаем глобальные ссылки для использования в другом потоке
    JavaVM* jvm;
    env->GetJavaVM(&jvm);
    jobject globalCallback = env->NewGlobalRef(callback);

    // Запускаем в отдельном потоке
    std::thread([jvm, globalCallback]() {
        JNIEnv* threadEnv;
        jvm->AttachCurrentThread(&threadEnv, nullptr);

        // Получаем метод колбэка
        jclass callbackClass = threadEnv->GetObjectClass(globalCallback);
        jmethodID onResultMethod = threadEnv->GetMethodID(
                callbackClass,
                "onResult",
                "(Ljava/lang/String;)V"
        );

        // Имитация длительной работы
        for (long i = 0; i < 1999999999; ++i) {
            // Пустой цикл для имитации работы
        }

        // Начальное сообщение
        threadEnv->CallVoidMethod(
                globalCallback,
                onResultMethod,
                threadEnv->NewStringUTF("result form cpp background...")
        );




        // Очистка
        threadEnv->DeleteGlobalRef(globalCallback);
        jvm->DetachCurrentThread();
    }).detach();  // Отсоединяем поток
}

// emit after delay
//extern "C" JNIEXPORT void JNICALL
//Java_com_example_learncpp_MainActivity_stringFromJNIAsync(
//        JNIEnv* env, jobject thiz, jobject callback) {
//
//    // Создаем глобальные ссылки для использования в другом потоке
//    JavaVM* jvm;
//    env->GetJavaVM(&jvm);
//    jobject globalCallback = env->NewGlobalRef(callback);
//
//    // Запускаем в отдельном потоке
//    std::thread([jvm, globalCallback]() {
//        JNIEnv* threadEnv;
//        jvm->AttachCurrentThread(&threadEnv, nullptr);
//
//        // Получаем метод колбэка
//        jclass callbackClass = threadEnv->GetObjectClass(globalCallback);
//        jmethodID onResultMethod = threadEnv->GetMethodID(
//                callbackClass,
//                "onResult",
//                "(Ljava/lang/String;)V"
//        );
//
//        // Начальное сообщение
//        threadEnv->CallVoidMethod(
//                globalCallback,
//                onResultMethod,
//                threadEnv->NewStringUTF("Starting iterations...")
//        );
//
//        // 10 итераций с задержкой
//        for (int i = 1; i <= 10; ++i) {
//            // Задержка 500мс
//            std::this_thread::sleep_for(std::chrono::milliseconds(500));
//
//            // Формируем сообщение о прогрессе
//            std::stringstream ss;
//            ss << "Progress: " << i << "/10";
//            std::string progress = ss.str();
//
//            // Отправляем прогресс
//            threadEnv->CallVoidMethod(
//                    globalCallback,
//                    onResultMethod,
//                    threadEnv->NewStringUTF(progress.c_str())
//            );
//        }
//
//        // Финальное сообщение
//        std::string result = "All iterations completed!";
//        threadEnv->CallVoidMethod(
//                globalCallback,
//                onResultMethod,
//                threadEnv->NewStringUTF(result.c_str())
//        );
//
//        // Очистка
//        threadEnv->DeleteGlobalRef(globalCallback);
//        jvm->DetachCurrentThread();
//    }).detach();  // Отсоединяем поток
//}