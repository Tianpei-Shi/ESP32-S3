package com.stp.rhtc_app.ui.screens

import androidx.compose.foundation.layout.*
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.*
import androidx.compose.material.icons.automirrored.filled.KeyboardArrowRight
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.unit.dp
import androidx.compose.material3.HorizontalDivider

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun ProfileScreen() {
    var isLoggedIn by remember { mutableStateOf(true) }
    var userName by remember { mutableStateOf("用户名") }
    
    Scaffold(
        topBar = {
            TopAppBar(
                title = { Text("我的") },
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
                .padding(16.dp),
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            if (isLoggedIn) {
                // 用户头像
                Box(
                    modifier = Modifier
                        .size(100.dp)
                        .clip(CircleShape)
                        .align(Alignment.CenterHorizontally),
                    contentAlignment = Alignment.Center
                ) {
                    Icon(
                        Icons.Default.Person,
                        contentDescription = null,
                        modifier = Modifier.size(64.dp)
                    )
                }
                
                Spacer(modifier = Modifier.height(16.dp))
                
                // 用户名
                Text(
                    text = userName,
                    style = MaterialTheme.typography.headlineSmall
                )
                
                Spacer(modifier = Modifier.height(32.dp))
            } else {
                Button(
                    onClick = { isLoggedIn = true },
                    modifier = Modifier
                        .fillMaxWidth()
                        .padding(vertical = 16.dp)
                ) {
                    Text("登录/注册")
                }
            }
            
            // 功能列表
            ProfileMenuItem(
                icon = Icons.Default.Bookmark,
                title = "我的订单",
                onClick = {}
            )
            
            ProfileMenuItem(
                icon = Icons.Default.Favorite,
                title = "我的收藏",
                onClick = {}
            )
            
            ProfileMenuItem(
                icon = Icons.Default.LocationOn,
                title = "收货地址",
                onClick = {}
            )
            
            ProfileMenuItem(
                icon = Icons.Default.Info,
                title = "关于我们",
                onClick = {}
            )
            
            ProfileMenuItem(
                icon = Icons.Default.Phone,
                title = "联系客服",
                onClick = {}
            )
            
            Spacer(modifier = Modifier.weight(1f))
            
            if (isLoggedIn) {
                Button(
                    onClick = { isLoggedIn = false },
                    modifier = Modifier.fillMaxWidth(),
                    colors = ButtonDefaults.buttonColors(
                        containerColor = MaterialTheme.colorScheme.error
                    )
                ) {
                    Text("退出登录")
                }
            }
        }
    }
}

@Composable
fun ProfileMenuItem(
    icon: androidx.compose.ui.graphics.vector.ImageVector,
    title: String,
    onClick: () -> Unit
) {
    Surface(
        onClick = onClick,
        modifier = Modifier.fillMaxWidth()
    ) {
        Row(
            modifier = Modifier
                .fillMaxWidth()
                .padding(vertical = 12.dp),
            verticalAlignment = Alignment.CenterVertically
        ) {
            Icon(
                icon,
                contentDescription = null,
                modifier = Modifier.padding(horizontal = 16.dp)
            )
            
            Text(
                text = title,
                style = MaterialTheme.typography.bodyLarge
            )
            
            Spacer(modifier = Modifier.weight(1f))
            
            Icon(
                Icons.AutoMirrored.Filled.KeyboardArrowRight,
                contentDescription = null,
                modifier = Modifier.padding(horizontal = 16.dp)
            )
        }
    }
    
    HorizontalDivider()
} 