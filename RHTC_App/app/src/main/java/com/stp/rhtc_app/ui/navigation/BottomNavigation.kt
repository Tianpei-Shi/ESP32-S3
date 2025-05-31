package com.stp.rhtc_app.ui.navigation

import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.AccountCircle
import androidx.compose.material.icons.filled.Home
import androidx.compose.material.icons.filled.ShoppingCart
import androidx.compose.material.icons.filled.Forum
import androidx.compose.material3.Icon
import androidx.compose.material3.NavigationBar
import androidx.compose.material3.NavigationBarItem
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.ui.graphics.vector.ImageVector
import androidx.navigation.NavController
import androidx.navigation.compose.currentBackStackEntryAsState

sealed class BottomNavItem(val route: String, val icon: ImageVector, val title: String) {
    object Devices : BottomNavItem("devices", Icons.Default.Home, "我的设备")
    object Community : BottomNavItem("community", Icons.Default.Forum, "社区")
    object Store : BottomNavItem("store", Icons.Default.ShoppingCart, "商城")
    object Profile : BottomNavItem("profile", Icons.Default.AccountCircle, "我的")
}

@Composable
fun BottomNavigation(navController: NavController) {
    val items = listOf(
        BottomNavItem.Devices,
        BottomNavItem.Community,
        BottomNavItem.Store,
        BottomNavItem.Profile
    )
    
    NavigationBar {
        val navBackStackEntry by navController.currentBackStackEntryAsState()
        val currentRoute = navBackStackEntry?.destination?.route
        
        items.forEach { item ->
            NavigationBarItem(
                icon = { Icon(item.icon, contentDescription = item.title) },
                label = { Text(text = item.title) },
                selected = currentRoute == item.route,
                onClick = {
                    if (currentRoute != item.route) {
                        navController.navigate(item.route) {
                            // 避免创建多个相同实例
                            popUpTo(navController.graph.startDestinationId)
                            launchSingleTop = true
                        }
                    }
                }
            )
        }
    }
} 