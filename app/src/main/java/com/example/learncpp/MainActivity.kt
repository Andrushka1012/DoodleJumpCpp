package com.example.learncpp

import android.annotation.SuppressLint
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.view.MotionEvent
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.viewinterop.AndroidView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MainActivity : ComponentActivity() {

    @SuppressLint("UnusedMaterial3ScaffoldPaddingParameter")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            Scaffold(
                modifier = Modifier.fillMaxSize(),
                content = { paddingValues ->
                    MainScreen()
                }
            )
        }
    }

    external fun startEngine()
    external fun drawFrame()
    external fun setViewport(width: Int, height: Int)
    external fun onHorizontalMove(x: Float)

    @SuppressLint("ClickableViewAccessibility")
    @Composable
    fun MainScreen() {
        AndroidView(factory = { context ->
            GLSurfaceView(context).apply {
                setEGLContextClientVersion(2)
                setRenderer(object : GLSurfaceView.Renderer {
                    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
                        startEngine()
                    }

                    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
                        setViewport(width, height)
                    }

                    override fun onDrawFrame(gl: GL10?) {
                        drawFrame()
                    }
                })

                setOnTouchListener { view, event ->
                    when (event.action) {
                        MotionEvent.ACTION_DOWN, MotionEvent.ACTION_MOVE -> {
                            val screenWidth = view.width.toFloat()
                            val touchX = event.x

                            // Нормализуем: 0.0 (левый край) -> 1.0 (правый край)
                            val normalizedX = touchX / screenWidth

                            // Преобразуем в диапазон [-1, 1]: центр = 0
                            val horizontalInput = (normalizedX - 0.5f) * 2.0f

                            // Ограничиваем диапазон
                            val clampedInput = horizontalInput.coerceIn(-1.0f, 1.0f)

                            onHorizontalMove(clampedInput)
                        }

                        MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {
                            // Отпустили палец - останавливаем движение
                            onHorizontalMove(0.0f)
                        }
                    }
                    true // Обрабатываем событие
                }
            }
        })
    }

    companion object {
        // Used to load the 'DoodleJumpCpp' library on application startup.
        init {
            System.loadLibrary("DoodleJumpCpp")
        }
    }
}
