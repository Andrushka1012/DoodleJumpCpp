package com.example.learncpp

import android.annotation.SuppressLint
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.view.MotionEvent
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.height
import androidx.compose.material3.Button
import androidx.compose.material3.CircularProgressIndicator
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableIntStateOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp
import androidx.compose.ui.viewinterop.AndroidView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MainActivity : ComponentActivity() {

    companion object {
        private var instance: MainActivity? = null

        init {
            System.loadLibrary("DoodleJumpCpp")
        }

        fun getInstance(): MainActivity? = instance

        @JvmStatic
        fun notifyGameOver(score: Float) {
            instance?.runOnUiThread {
                instance?.onGameOver(score)
            }
        }
    }

    var isPlaying by mutableStateOf(false)
    var currentResult by mutableIntStateOf(-1)

    @SuppressLint("UnusedMaterial3ScaffoldPaddingParameter")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        instance = this
        enableEdgeToEdge()
        setContent {
            Scaffold(
                modifier = Modifier.fillMaxSize(), content = { paddingValues ->
                    MainScreen()
                })
        }
    }

    external fun startEngine()
    external fun startGame()
    external fun drawFrame()
    external fun setViewport(width: Int, height: Int)
    external fun onHorizontalMove(x: Float)

    @SuppressLint("ClickableViewAccessibility")
    @Composable
    fun MainScreen() {
        var ready by remember { mutableStateOf(false) }

        AndroidView(factory = { context ->
            GLSurfaceView(context).apply {
                setEGLContextClientVersion(2)
                setRenderer(object : GLSurfaceView.Renderer {
                    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
                        startEngine()
                        ready = true
                    }

                    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
                        setViewport(width, height)
                    }

                    override fun onDrawFrame(gl: GL10?) {
                        if (ready && isPlaying) {
                            drawFrame()
                        }
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

        if (!ready) CircularProgressIndicator(
            modifier = Modifier.fillMaxSize(), color = MaterialTheme.colorScheme.primary
        )

        if (!isPlaying && ready) {
            GameMenu(
                onStartGame = {
                    if (ready) {
                        startGame()
                        isPlaying = true
                    }
                })
        }
    }

    @Composable
    fun GameMenu(onStartGame: () -> Unit) {
        Column(
            modifier = Modifier.fillMaxSize(), horizontalAlignment = Alignment.CenterHorizontally, verticalArrangement = Arrangement.Center
        ) {
            Text(
                text = if (currentResult < 0) "Doodle Jump" else "Game Over",
                style = MaterialTheme.typography.headlineLarge.copy(
                    color = Color.White
                )
            )
            if (currentResult >= 0) {
                Text(
                    text = "Your score: $currentResult",
                    style = MaterialTheme.typography.bodyLarge.copy(
                        color = Color.White
                    )
                )
            }
            Spacer(modifier = Modifier.height(16.dp))
            Button(onClick = onStartGame) {
                Text(text = "Start Game")
            }
        }
    }

    fun onGameOver(score: Float) {
        isPlaying = false
        currentResult = score.toInt()
    }
}
