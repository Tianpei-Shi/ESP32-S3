package com.stp.rhtc_app.ui.navigation

import androidx.compose.runtime.Composable
import androidx.compose.runtime.remember
import androidx.navigation.NavHostController
import androidx.navigation.NavType
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.navArgument
import com.stp.rhtc_app.ui.screens.*

object Routes {
    const val DEVICES = "devices"
    const val COMMUNITY = "community"
    const val STORE = "store"
    const val PROFILE = "profile"
    const val DEVICE_CONTROL = "device_control/{deviceId}"
    const val ADD_DEVICE = "add_device"
    const val CONFIGURE_WIFI = "configure_wifi/{deviceAddress}/{deviceName}"
    
    fun deviceControl(deviceId: String): String = "device_control/$deviceId"
    fun configureWifi(deviceAddress: String, deviceName: String): String = 
        "configure_wifi/$deviceAddress/$deviceName"
}

@Composable
fun NavGraph(navController: NavHostController) {
    NavHost(
        navController = navController,
        startDestination = Routes.DEVICES
    ) {
        // 主要标签页
        composable(Routes.DEVICES) {
            DevicesScreen(
                onAddDevice = {
                    navController.navigate(Routes.ADD_DEVICE)
                },
                onDeviceSettings = { deviceId ->
                    navController.navigate(Routes.deviceControl(deviceId))
                }
            )
        }
        
        composable(Routes.COMMUNITY) {
            CommunityScreen(
                onPostClick = { postId ->
                    // 帖子详情页导航 (未实现)
                }
            )
        }
        
        composable(Routes.STORE) {
            StoreScreen(
                onProductClick = { productId ->
                    // 商品详情页导航 (未实现)
                }
            )
        }
        
        composable(Routes.PROFILE) {
            ProfileScreen()
        }
        
        // 设备控制相关
        composable(
            route = Routes.DEVICE_CONTROL,
            arguments = listOf(
                navArgument("deviceId") {
                    type = NavType.StringType
                }
            )
        ) { entry ->
            val deviceId = entry.arguments?.getString("deviceId") ?: ""
            DeviceControlScreen(
                deviceId = deviceId,
                onBackClick = {
                    navController.popBackStack()
                },
                onConfigureWifi = {
                    // 在实际应用中，需要先获取设备的蓝牙地址和名称
                    val deviceInfo = BluetoothDeviceInfo("00:11:22:33:44:55", "加湿小车")
                    navController.navigate(Routes.configureWifi(deviceInfo.address, deviceInfo.name))
                }
            )
        }
        
        composable(Routes.ADD_DEVICE) {
            AddDeviceScreen(
                onBackClick = {
                    navController.popBackStack()
                },
                onDeviceSelected = { deviceInfo ->
                    navController.navigate(Routes.configureWifi(deviceInfo.address, deviceInfo.name))
                }
            )
        }
        
        composable(
            route = Routes.CONFIGURE_WIFI,
            arguments = listOf(
                navArgument("deviceAddress") {
                    type = NavType.StringType
                },
                navArgument("deviceName") {
                    type = NavType.StringType
                }
            )
        ) { entry ->
            val deviceAddress = entry.arguments?.getString("deviceAddress") ?: ""
            val deviceName = entry.arguments?.getString("deviceName") ?: ""
            val deviceInfo = remember {
                BluetoothDeviceInfo(deviceAddress, deviceName)
            }
            
            ConfigureWifiScreen(
                deviceInfo = deviceInfo,
                onBackClick = {
                    navController.popBackStack()
                },
                onConfigComplete = {
                    // 配置完成后返回设备列表
                    navController.navigate(Routes.DEVICES) {
                        popUpTo(Routes.DEVICES) {
                            inclusive = true
                        }
                    }
                }
            )
        }
    }
} 