package com.stp.rhtc_app.ui.screens

import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Add
import androidx.compose.material.icons.filled.Refresh
import androidx.compose.material.icons.filled.Settings
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp

data class DeviceInfo(
    val id: String,
    val name: String,
    val isConnected: Boolean = false,
    val status: String = "离线"
)

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun DevicesScreen(onAddDevice: () -> Unit = {}, onDeviceSettings: (String) -> Unit = {}) {
    var deviceList by remember { mutableStateOf(listOf<DeviceInfo>()) }
    var isLoading by remember { mutableStateOf(false) }
    
    Scaffold(
        topBar = {
            TopAppBar(
                title = { Text("我的设备") },
                actions = {
                    IconButton(onClick = {
                        isLoading = true
                        // 模拟刷新设备列表
                        deviceList = getSampleDevices()
                        isLoading = false
                    }) {
                        Icon(Icons.Default.Refresh, contentDescription = "刷新")
                    }
                    IconButton(onClick = { onAddDevice() }) {
                        Icon(Icons.Default.Add, contentDescription = "添加设备")
                    }
                }
            )
        }
    ) { padding ->
        Box(modifier = Modifier
            .fillMaxSize()
            .padding(padding)
        ) {
            if (isLoading) {
                CircularProgressIndicator(modifier = Modifier.align(Alignment.Center))
            } else if (deviceList.isEmpty()) {
                Text(
                    text = "没有发现设备\n点击右上角添加设备",
                    textAlign = TextAlign.Center,
                    modifier = Modifier.align(Alignment.Center)
                )
            } else {
                LazyColumn(
                    modifier = Modifier.fillMaxSize(),
                    contentPadding = PaddingValues(16.dp)
                ) {
                    items(deviceList) { device ->
                        DeviceItem(device = device) {
                            onDeviceSettings(device.id)
                        }
                    }
                }
            }
        }
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun DeviceItem(device: DeviceInfo, onClick: () -> Unit) {
    Card(
        onClick = onClick,
        modifier = Modifier
            .fillMaxWidth()
            .padding(vertical = 8.dp)
    ) {
        Row(
            modifier = Modifier
                .fillMaxWidth()
                .padding(16.dp),
            verticalAlignment = Alignment.CenterVertically,
            horizontalArrangement = Arrangement.SpaceBetween
        ) {
            Column {
                Text(text = device.name, style = MaterialTheme.typography.titleMedium)
                Spacer(modifier = Modifier.height(4.dp))
                Text(
                    text = device.status,
                    style = MaterialTheme.typography.bodyMedium,
                    color = if (device.isConnected) MaterialTheme.colorScheme.primary else MaterialTheme.colorScheme.error
                )
            }
            
            IconButton(onClick = onClick) {
                Icon(Icons.Default.Settings, contentDescription = "设置")
            }
        }
    }
}

// 示例数据
private fun getSampleDevices(): List<DeviceInfo> {
    return listOf(
        DeviceInfo("1", "加湿小车 #1", true, "在线"),
        DeviceInfo("2", "加湿小车 #2", false, "离线")
    )
} 