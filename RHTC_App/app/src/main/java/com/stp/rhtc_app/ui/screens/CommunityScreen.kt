package com.stp.rhtc_app.ui.screens

import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.automirrored.filled.Comment
import androidx.compose.material.icons.filled.Person
import androidx.compose.material.icons.filled.ThumbUp
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.text.style.TextOverflow
import androidx.compose.ui.unit.dp
import java.text.SimpleDateFormat
import java.util.*

data class PostInfo(
    val id: String,
    val author: String,
    val title: String,
    val content: String,
    val date: Date,
    val likes: Int,
    val comments: Int
)

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun CommunityScreen(onPostClick: (String) -> Unit = {}) {
    var posts by remember { mutableStateOf(getSamplePosts()) }
    
    Scaffold(
        topBar = {
            TopAppBar(
                title = { Text("社区") }
            )
        }
    ) { padding ->
        LazyColumn(
            modifier = Modifier
                .fillMaxSize()
                .padding(padding),
            contentPadding = PaddingValues(16.dp)
        ) {
            items(posts) { post ->
                PostItem(post = post, onClick = { onPostClick(post.id) })
            }
        }
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun PostItem(post: PostInfo, onClick: () -> Unit) {
    val dateFormat = remember { SimpleDateFormat("yyyy-MM-dd HH:mm", Locale.getDefault()) }
    
    Card(
        onClick = onClick,
        modifier = Modifier
            .fillMaxWidth()
            .padding(vertical = 8.dp)
    ) {
        Column(
            modifier = Modifier
                .fillMaxWidth()
                .padding(16.dp)
        ) {
            Row(
                verticalAlignment = Alignment.CenterVertically
            ) {
                Icon(Icons.Default.Person, contentDescription = null)
                Spacer(modifier = Modifier.width(8.dp))
                Text(text = post.author, style = MaterialTheme.typography.bodyMedium)
                Spacer(modifier = Modifier.weight(1f))
                Text(
                    text = dateFormat.format(post.date),
                    style = MaterialTheme.typography.bodySmall
                )
            }
            
            Spacer(modifier = Modifier.height(8.dp))
            
            Text(
                text = post.title,
                style = MaterialTheme.typography.titleMedium
            )
            
            Spacer(modifier = Modifier.height(4.dp))
            
            Text(
                text = post.content,
                style = MaterialTheme.typography.bodyMedium,
                maxLines = 3,
                overflow = TextOverflow.Ellipsis
            )
            
            Spacer(modifier = Modifier.height(8.dp))
            
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.End
            ) {
                Row(verticalAlignment = Alignment.CenterVertically) {
                    Icon(
                        Icons.Default.ThumbUp,
                        contentDescription = "点赞",
                        modifier = Modifier.size(16.dp)
                    )
                    Spacer(modifier = Modifier.width(4.dp))
                    Text(text = "${post.likes}", style = MaterialTheme.typography.bodySmall)
                }
                
                Spacer(modifier = Modifier.width(16.dp))
                
                Row(verticalAlignment = Alignment.CenterVertically) {
                    Icon(
                        Icons.AutoMirrored.Filled.Comment,
                        contentDescription = "评论",
                        modifier = Modifier.size(16.dp)
                    )
                    Spacer(modifier = Modifier.width(4.dp))
                    Text(text = "${post.comments}", style = MaterialTheme.typography.bodySmall)
                }
            }
        }
    }
}

// 示例数据
private fun getSamplePosts(): List<PostInfo> {
    return listOf(
        PostInfo(
            "1",
            "用户A",
            "我的加湿小车使用体验",
            "最近购买了加湿小车，使用效果非常好，可以方便地为整个房间加湿，而且噪音很小，非常适合在办公室或卧室使用。同时电池续航也很长，一次充电可以使用整整两天...",
            Date(),
            15,
            5
        ),
        PostInfo(
            "2",
            "用户B",
            "加湿小车WiFi连接问题",
            "有谁知道如何解决加湿小车的WiFi连接问题？我尝试了重置但仍然无法连接到我的路由器。希望有经验的朋友能够提供一些建议或者解决方案。",
            Date(System.currentTimeMillis() - 86400000),
            8,
            12
        ),
        PostInfo(
            "3",
            "用户C",
            "分享几个加湿小车使用技巧",
            "使用加湿小车一段时间后，我总结了一些小技巧：1. 清洁水箱时使用白醋可以去除水垢；2. 摆放在离床1-2米的地方效果最佳；3. 冬季可以搭配暖风机使用效果更好...",
            Date(System.currentTimeMillis() - 172800000),
            25,
            7
        )
    )
} 