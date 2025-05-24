package com.example.learncpp

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp

class MainActivity : ComponentActivity() {

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
     * A native method that is implemented by the 'DoodleJumpCpp' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String


    @Composable
    fun MainScreen() {
        Text(
            text = stringFromJNI(),
            modifier = Modifier
                .fillMaxSize()
                .padding(16.dp)
        )
    }


    companion object {
        // Used to load the 'DoodleJumpCpp' library on application startup.
        init {
            System.loadLibrary("DoodleJumpCpp")
        }
    }
}
