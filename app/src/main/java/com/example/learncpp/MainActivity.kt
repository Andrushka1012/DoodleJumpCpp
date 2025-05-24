package com.example.learncpp

import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.async
import kotlinx.coroutines.channels.awaitClose
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.callbackFlow
import kotlinx.coroutines.flow.catch
import kotlinx.coroutines.withContext

class MainActivity : ComponentActivity() {
    // Колбэк интерфейс
    interface NativeCallback {
        fun onResult(result: String)
        fun onError(error: String)
    }


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setContent {
            Scaffold(
                modifier = Modifier.fillMaxSize(),
                content = { paddingValues ->
                    Box(modifier = Modifier.padding(paddingValues)) {
                        MainScreen()
                    }
                }
            )
        }
    }

    /**
     * A native method that is implemented by the 'learncpp' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    // Нативный метод с колбэком
    private external fun stringFromJNIAsync(callback: NativeCallback)

    // Использование с корутинами
    fun getStringFromJNIAsync(): Flow<String> = callbackFlow {
        stringFromJNIAsync(object : NativeCallback {
            override fun onResult(result: String) {
                Log.d("qwe", "Result: $result")
                trySend(result) // Отправляем каждое значение в поток
            }

            override fun onError(error: String) {
                Log.d("qwe", "Result: $error")
                close(Exception(error)) // Закрываем поток с ошибкой
            }
        })

        // Очистка при отмене
        awaitClose {
            // Здесь можно добавить логику отмены C++ операции если нужно
        }
    }


    suspend fun longForKotlin() = withContext(Dispatchers.Default) {

        for (i in 0..1999999999) {
        }

        "Long operation completed kotlin"
    }

    suspend fun longForCpp() = withContext(Dispatchers.Default) {
        stringFromJNI()
    }


    @Composable
    fun MainScreen() {
        var textCpp by remember { mutableStateOf("Loading...") }
        var textKotlin by remember { mutableStateOf("Loading...") }

        LaunchedEffect(Unit) {
//            getStringFromJNIAsync()
//                .catch { e ->
//                    textCpp = "Error: ${e.message}"
//                }
//                .collect { newValue ->
//                    Log.d("qwe", "collected: $newValue")
//                    textCpp = newValue
//                }

            // ИЛИ можно использовать async если нужен результат
//            val cppDeferred = async { longForCpp() }
            val cppDeferred = async {
                getStringFromJNIAsync()
                .catch { e ->
                    textCpp = "Error: ${e.message}"
                }
                .collect { newValue ->
                    Log.d("qwe", "collected: $newValue")
                    textCpp = newValue
                }


            }
            val kotlinDeferred = async { longForKotlin() }

            // Ждем результаты
            textKotlin = kotlinDeferred.await()
//            cppDeferred.await()
        }

        Column {
            Text(text = "Cpp Text: $textCpp")
            Text(text = "Kotlin Text: $textKotlin")
        }
    }


    companion object {
        // Used to load the 'learncpp' library on application startup.
        init {
            System.loadLibrary("learncpp")
        }
    }
}
