package com.stp.rhtc_app

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.navigation.compose.rememberNavController
import androidx.navigation.compose.currentBackStackEntryAsState
import com.stp.rhtc_app.ui.navigation.BottomNavigation
import com.stp.rhtc_app.ui.navigation.NavGraph
import com.stp.rhtc_app.ui.theme.RHTC_AppTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            RHTC_AppTheme {
                MainContent()
            }
        }
    }
}

@Composable
fun MainContent() {
    val navController = rememberNavController()
    
    Scaffold(
        bottomBar = {
            // 只在主屏幕显示底部导航栏
            val currentRoute = navController.currentBackStackEntryAsState().value?.destination?.route
            val showBottomNav = when (currentRoute) {
                "devices", "community", "store", "profile" -> true
                else -> false
            }
            
            if (showBottomNav) {
                BottomNavigation(navController = navController)
            }
        }
    ) { padding ->
        // 内容区域
        Box(
            modifier = Modifier
                .fillMaxSize()
                .padding(padding)
        ) {
            NavGraph(navController = navController)
        }
    }
}

// 获取当前路由
@Composable
fun androidx.navigation.NavController.currentBackStackEntryAsState() =
    currentBackStackEntryAsState()