package com.stp.rhtc_app.ui.screens

import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.grid.GridCells
import androidx.compose.foundation.lazy.grid.LazyVerticalGrid
import androidx.compose.foundation.lazy.grid.items
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Search
import androidx.compose.material.icons.filled.ShoppingCart
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.text.style.TextOverflow
import androidx.compose.ui.unit.dp

data class ProductInfo(
    val id: String,
    val name: String,
    val price: Double,
    val imageUrl: String,
    val description: String
)

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun StoreScreen(onProductClick: (String) -> Unit = {}) {
    var searchQuery by remember { mutableStateOf("") }
    var products by remember { mutableStateOf(getSampleProducts()) }
    
    Scaffold(
        topBar = {
            TopAppBar(
                title = { Text("商城") },
                actions = {
                    IconButton(onClick = {}) {
                        Icon(Icons.Default.ShoppingCart, contentDescription = "购物车")
                    }
                }
            )
        }
    ) { padding ->
        Column(
            modifier = Modifier
                .fillMaxSize()
                .padding(padding)
        ) {
            // 搜索栏
            OutlinedTextField(
                value = searchQuery,
                onValueChange = { searchQuery = it },
                placeholder = { Text("搜索产品") },
                leadingIcon = { Icon(Icons.Default.Search, contentDescription = null) },
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(16.dp),
                singleLine = true
            )
            
            // 产品网格
            LazyVerticalGrid(
                columns = GridCells.Fixed(2),
                contentPadding = PaddingValues(16.dp),
                horizontalArrangement = Arrangement.spacedBy(16.dp),
                verticalArrangement = Arrangement.spacedBy(16.dp)
            ) {
                items(products) { product ->
                    ProductItem(product = product, onClick = { onProductClick(product.id) })
                }
            }
        }
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun ProductItem(product: ProductInfo, onClick: () -> Unit) {
    Card(
        onClick = onClick,
        modifier = Modifier.fillMaxWidth()
    ) {
        Column {
            // 这里应该是产品图片，现在用Box模拟
            Box(
                modifier = Modifier
                    .fillMaxWidth()
                    .height(120.dp)
                    .align(Alignment.CenterHorizontally),
                contentAlignment = Alignment.Center
            ) {
                Text("产品图片")
            }
            
            Column(
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(12.dp)
            ) {
                Text(
                    text = product.name,
                    style = MaterialTheme.typography.titleMedium,
                    maxLines = 1,
                    overflow = TextOverflow.Ellipsis
                )
                
                Spacer(modifier = Modifier.height(4.dp))
                
                Text(
                    text = "¥${product.price}",
                    style = MaterialTheme.typography.bodyMedium,
                    color = MaterialTheme.colorScheme.primary
                )
                
                Spacer(modifier = Modifier.height(4.dp))
                
                Text(
                    text = product.description,
                    style = MaterialTheme.typography.bodySmall,
                    maxLines = 2,
                    overflow = TextOverflow.Ellipsis
                )
            }
        }
    }
}

// 示例数据
private fun getSampleProducts(): List<ProductInfo> {
    return listOf(
        ProductInfo(
            "1",
            "智能加湿小车 标准版",
            699.00,
            "",
            "智能控制，远程操作，WiFi连接，长效加湿"
        ),
        ProductInfo(
            "2",
            "智能加湿小车 豪华版",
            899.00,
            "",
            "高端配置，智能控制，香薰功能，大容量水箱"
        ),
        ProductInfo(
            "3",
            "加湿小车专用香薰",
            59.00,
            "",
            "纯植物萃取，多种香型可选，舒缓神经"
        ),
        ProductInfo(
            "4",
            "加湿小车备用滤芯",
            79.00,
            "",
            "高效过滤，长效抑菌，3个月更换一次"
        ),
        ProductInfo(
            "5",
            "加湿小车消毒液",
            39.00,
            "",
            "安全环保，有效杀菌，适合水箱清洁"
        ),
        ProductInfo(
            "6",
            "加湿小车遥控器",
            49.00,
            "",
            "无线控制，轻松操作，适用于所有型号"
        )
    )
} 