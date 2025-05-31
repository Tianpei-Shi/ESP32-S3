package com.stp.rhtc_app.ui.screens

import androidx.compose.foundation.layout.*
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.automirrored.filled.ArrowBack
import androidx.compose.material.icons.filled.Visibility
import androidx.compose.material.icons.filled.VisibilityOff
import androidx.compose.material.icons.filled.Wifi
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.text.input.PasswordVisualTransformation
import androidx.compose.ui.text.input.VisualTransformation
import androidx.compose.ui.unit.dp

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun ConfigureWifiScreen(
    deviceInfo: BluetoothDeviceInfo,
    onBackClick: () -> Unit,
    onConfigComplete: () -> Unit
) {
    var ssid by remember { mutableStateOf("") }
    var password by remember { mutableStateOf("") }
    var isPasswordVisible by remember { mutableStateOf(false) }
    var isConfiguring by remember { mutableStateOf(false) }
    
    Scaffold(
        topBar = {
            TopAppBar(
                title = { Text("配置WiFi") },
                navigationIcon = {
                    IconButton(onClick = onBackClick) {
                        Icon(Icons.AutoMirrored.Filled.ArrowBack, contentDescription = "返回")
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
                    .fillMaxWidth()
                    .padding(16.dp)
            ) {
                Text(
                    text = "为您的设备配置WiFi",
                    style = MaterialTheme.typography.titleLarge
                )
                
                Spacer(modifier = Modifier.height(8.dp))
                
                Text(
                    text = "设备: ${deviceInfo.name}",
                    style = MaterialTheme.typography.bodyLarge
                )
                
                Text(
                    text = "地址: ${deviceInfo.address}",
                    style = MaterialTheme.typography.bodyMedium
                )
                
                Spacer(modifier = Modifier.height(32.dp))
                
                OutlinedTextField(
                    value = ssid,
                    onValueChange = { ssid = it },
                    label = { Text("WiFi名称 (SSID)") },
                    leadingIcon = { Icon(Icons.Default.Wifi, contentDescription = null) },
                    modifier = Modifier.fillMaxWidth(),
                    singleLine = true
                )
                
                Spacer(modifier = Modifier.height(16.dp))
                
                OutlinedTextField(
                    value = password,
                    onValueChange = { password = it },
                    label = { Text("WiFi密码") },
                    trailingIcon = {
                        IconButton(onClick = { isPasswordVisible = !isPasswordVisible }) {
                            Icon(
                                if (isPasswordVisible) Icons.Default.VisibilityOff else Icons.Default.Visibility,
                                contentDescription = if (isPasswordVisible) "隐藏密码" else "显示密码"
                            )
                        }
                    },
                    visualTransformation = if (isPasswordVisible) VisualTransformation.None else PasswordVisualTransformation(),
                    modifier = Modifier.fillMaxWidth(),
                    singleLine = true
                )
                
                Spacer(modifier = Modifier.height(32.dp))
                
                Text(
                    text = "说明: 配置过程中，设备会重启并连接到您指定的WiFi网络。请确保提供的WiFi信息正确，否则可能导致设备无法连接。",
                    style = MaterialTheme.typography.bodySmall
                )
            }
            
            if (isConfiguring) {
                CircularProgressIndicator(
                    modifier = Modifier.align(Alignment.Center)
                )
            }
            
            Button(
                onClick = {
                    if (ssid.isNotBlank()) {
                        isConfiguring = true
                        // 模拟配置过程
                        // 在实际应用中，这里应该通过蓝牙向设备发送WiFi配置信息
                        // 延迟是为了模拟网络操作
                        
                        // 配置完成后
                        isConfiguring = false
                        onConfigComplete()
                    }
                },
                enabled = ssid.isNotBlank() && !isConfiguring,
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(16.dp)
                    .align(Alignment.BottomCenter)
            ) {
                Text("配置WiFi")
            }
        }
    }
} 