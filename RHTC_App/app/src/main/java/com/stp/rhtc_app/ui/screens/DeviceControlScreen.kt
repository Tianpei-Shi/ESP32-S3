package com.stp.rhtc_app.ui.screens

import androidx.compose.foundation.layout.*
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.verticalScroll
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.automirrored.filled.ArrowBack
import androidx.compose.material.icons.filled.*
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun DeviceControlScreen(
    deviceId: String,
    onBackClick: () -> Unit,
    onConfigureWifi: () -> Unit
) {
    // 设备状态数据
    var deviceName by remember { mutableStateOf("加湿小车") }
    var isPoweredOn by remember { mutableStateOf(true) }
    var humidityLevel by remember { mutableStateOf(50) }
    var waterLevel by remember { mutableStateOf(80) }
    var isAutoMode by remember { mutableStateOf(true) }
    var fanSpeed by remember { mutableStateOf(2) }
    
    Scaffold(
        topBar = {
            TopAppBar(
                title = { Text(deviceName) },
                navigationIcon = {
                    IconButton(onClick = onBackClick) {
                        Icon(Icons.AutoMirrored.Filled.ArrowBack, contentDescription = "返回")
                    }
                },
                actions = {
                    IconButton(onClick = {}) {
                        Icon(Icons.Default.Settings, contentDescription = "设置")
                    }
                }
            )
        }
    ) { padding ->
        Column(
            modifier = Modifier
                .fillMaxSize()
                .padding(padding)
                .padding(16.dp)
                .verticalScroll(rememberScrollState()),
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            // 设备状态卡片
            Card(
                modifier = Modifier.fillMaxWidth()
            ) {
                Column(
                    modifier = Modifier
                        .fillMaxWidth()
                        .padding(16.dp),
                    horizontalAlignment = Alignment.CenterHorizontally
                ) {
                    // 设备状态指示
                    Text(
                        text = if (isPoweredOn) "设备已开启" else "设备已关闭",
                        style = MaterialTheme.typography.headlineSmall,
                        color = if (isPoweredOn) MaterialTheme.colorScheme.primary else MaterialTheme.colorScheme.error
                    )
                    
                    Spacer(modifier = Modifier.height(16.dp))
                    
                    // 开关按钮
                    Switch(
                        checked = isPoweredOn,
                        onCheckedChange = { isPoweredOn = it }
                    )
                    
                    Spacer(modifier = Modifier.height(16.dp))
                    
                    Row(
                        modifier = Modifier.fillMaxWidth(),
                        horizontalArrangement = Arrangement.SpaceEvenly
                    ) {
                        Column(horizontalAlignment = Alignment.CenterHorizontally) {
                            Text("湿度", style = MaterialTheme.typography.bodyMedium)
                            Text(
                                "$humidityLevel%", 
                                style = MaterialTheme.typography.titleLarge
                            )
                        }
                        
                        Column(horizontalAlignment = Alignment.CenterHorizontally) {
                            Text("水位", style = MaterialTheme.typography.bodyMedium)
                            Text(
                                "$waterLevel%",
                                style = MaterialTheme.typography.titleLarge,
                                color = if (waterLevel > 20) MaterialTheme.colorScheme.primary else MaterialTheme.colorScheme.error
                            )
                        }
                    }
                }
            }
            
            Spacer(modifier = Modifier.height(24.dp))
            
            // 模式选择
            Text(
                text = "加湿模式",
                style = MaterialTheme.typography.titleMedium,
                modifier = Modifier.fillMaxWidth()
            )
            
            Spacer(modifier = Modifier.height(8.dp))
            
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.spacedBy(16.dp)
            ) {
                FilterChip(
                    selected = isAutoMode,
                    onClick = { isAutoMode = true },
                    label = { Text("自动模式") },
                    modifier = Modifier.weight(1f)
                )
                
                FilterChip(
                    selected = !isAutoMode,
                    onClick = { isAutoMode = false },
                    label = { Text("手动模式") },
                    modifier = Modifier.weight(1f)
                )
            }
            
            Spacer(modifier = Modifier.height(24.dp))
            
            // 风速控制
            Text(
                text = "风速控制",
                style = MaterialTheme.typography.titleMedium,
                modifier = Modifier.fillMaxWidth()
            )
            
            Spacer(modifier = Modifier.height(8.dp))
            
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.SpaceBetween,
                verticalAlignment = Alignment.CenterVertically
            ) {
                IconButton(
                    onClick = { 
                        if (fanSpeed > 1) fanSpeed--
                    },
                    enabled = !isAutoMode && fanSpeed > 1
                ) {
                    Icon(Icons.Default.Remove, contentDescription = "减少")
                }
                
                Text(
                    text = when (fanSpeed) {
                        1 -> "低速"
                        2 -> "中速"
                        else -> "高速"
                    },
                    style = MaterialTheme.typography.titleLarge,
                    modifier = Modifier.padding(horizontal = 16.dp)
                )
                
                IconButton(
                    onClick = { 
                        if (fanSpeed < 3) fanSpeed++
                    },
                    enabled = !isAutoMode && fanSpeed < 3
                ) {
                    Icon(Icons.Default.Add, contentDescription = "增加")
                }
            }
            
            Spacer(modifier = Modifier.height(24.dp))
            
            // 湿度设置
            Text(
                text = "目标湿度",
                style = MaterialTheme.typography.titleMedium,
                modifier = Modifier.fillMaxWidth()
            )
            
            Spacer(modifier = Modifier.height(8.dp))
            
            Text(
                text = "$humidityLevel%",
                style = MaterialTheme.typography.bodyLarge,
                textAlign = TextAlign.Center,
                modifier = Modifier.fillMaxWidth()
            )
            
            Slider(
                value = humidityLevel.toFloat(),
                onValueChange = { humidityLevel = it.toInt() },
                valueRange = 30f..80f,
                enabled = !isAutoMode,
                modifier = Modifier.fillMaxWidth()
            )
            
            Spacer(modifier = Modifier.height(32.dp))
            
            // WiFi配置按钮
            OutlinedButton(
                onClick = onConfigureWifi,
                modifier = Modifier.fillMaxWidth()
            ) {
                Icon(Icons.Default.Wifi, contentDescription = null)
                Spacer(modifier = Modifier.width(8.dp))
                Text("配置WiFi连接")
            }
            
            Spacer(modifier = Modifier.height(16.dp))
            
            // 重启设备按钮
            OutlinedButton(
                onClick = { /* 重启设备逻辑 */ },
                modifier = Modifier.fillMaxWidth()
            ) {
                Icon(Icons.Default.Refresh, contentDescription = null)
                Spacer(modifier = Modifier.width(8.dp))
                Text("重启设备")
            }
        }
    }
} 