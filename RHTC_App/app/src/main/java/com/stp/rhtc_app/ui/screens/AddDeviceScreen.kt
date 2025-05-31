package com.stp.rhtc_app.ui.screens

import androidx.compose.foundation.layout.*
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.automirrored.filled.ArrowBack
import androidx.compose.material.icons.filled.Bluetooth
import androidx.compose.material.icons.filled.Refresh
import androidx.compose.material3.*
import androidx.compose.material3.HorizontalDivider
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp

data class BluetoothDeviceInfo(
    val address: String,
    val name: String
)

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun AddDeviceScreen(
    onBackClick: () -> Unit,
    onDeviceSelected: (BluetoothDeviceInfo) -> Unit
) {
    var isScanning by remember { mutableStateOf(false) }
    var devices by remember { mutableStateOf(listOf<BluetoothDeviceInfo>()) }
    
    Scaffold(
        topBar = {
            TopAppBar(
                title = { Text("添加设备") },
                navigationIcon = {
                    IconButton(onClick = onBackClick) {
                        Icon(Icons.AutoMirrored.Filled.ArrowBack, contentDescription = "返回")
                    }
                },
                actions = {
                    IconButton(
                        onClick = {
                            isScanning = true
                            // 模拟蓝牙扫描
                            devices = listOf(
                                BluetoothDeviceInfo("00:11:22:33:44:55", "加湿小车_A1B2"),
                                BluetoothDeviceInfo("66:77:88:99:AA:BB", "加湿小车_C3D4")
                            )
                            isScanning = false
                        },
                        enabled = !isScanning
                    ) {
                        Icon(Icons.Default.Refresh, contentDescription = "刷新")
                    }
                }
            )
        }
    ) { padding ->
        Box(
            modifier = Modifier
                .fillMaxSize()
                .padding(padding)
        ) {
            Column(
                modifier = Modifier
                    .fillMaxSize()
                    .padding(16.dp)
            ) {
                Text(
                    text = "请确保：",
                    style = MaterialTheme.typography.titleMedium
                )
                
                Spacer(modifier = Modifier.height(8.dp))
                
                Text("1. 设备已开启并处于蓝牙配对模式")
                Text("2. 设备指示灯正在闪烁")
                Text("3. 您的手机蓝牙已打开")
                
                Spacer(modifier = Modifier.height(24.dp))
                
                Text(
                    text = "可用设备",
                    style = MaterialTheme.typography.titleMedium
                )
                
                Spacer(modifier = Modifier.height(8.dp))
                
                if (isScanning) {
                    CircularProgressIndicator(
                        modifier = Modifier.align(Alignment.CenterHorizontally)
                    )
                } else if (devices.isEmpty()) {
                    Text(
                        text = "未找到设备\n请点击右上角刷新按钮重新扫描",
                        textAlign = TextAlign.Center,
                        modifier = Modifier
                            .fillMaxWidth()
                            .padding(vertical = 32.dp)
                    )
                } else {
                    devices.forEach { device ->
                        DeviceListItem(device = device, onClick = { onDeviceSelected(device) })
                    }
                }
            }
            
            Button(
                onClick = {
                    isScanning = true
                    // 模拟蓝牙扫描
                    devices = listOf(
                        BluetoothDeviceInfo("00:11:22:33:44:55", "加湿小车_A1B2"),
                        BluetoothDeviceInfo("66:77:88:99:AA:BB", "加湿小车_C3D4")
                    )
                    isScanning = false
                },
                enabled = !isScanning,
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(16.dp)
                    .align(Alignment.BottomCenter)
            ) {
                Icon(Icons.Default.Bluetooth, contentDescription = null)
                Spacer(modifier = Modifier.width(8.dp))
                Text("扫描设备")
            }
        }
    }
}

@Composable
fun DeviceListItem(device: BluetoothDeviceInfo, onClick: () -> Unit) {
    Surface(
        onClick = onClick,
        modifier = Modifier
            .fillMaxWidth()
            .padding(vertical = 4.dp)
    ) {
        Row(
            modifier = Modifier
                .fillMaxWidth()
                .padding(16.dp),
            verticalAlignment = Alignment.CenterVertically
        ) {
            Icon(Icons.Default.Bluetooth, contentDescription = null)
            
            Spacer(modifier = Modifier.width(16.dp))
            
            Column {
                Text(
                    text = device.name,
                    style = MaterialTheme.typography.bodyLarge
                )
                Text(
                    text = device.address,
                    style = MaterialTheme.typography.bodyMedium,
                    color = MaterialTheme.colorScheme.outline
                )
            }
        }
    }
    
    HorizontalDivider()
} 