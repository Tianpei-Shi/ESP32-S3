#include "wifi_manager.h"

// Wi-Fi连接状态变量
static WiFiConnectionStatus currentWiFiStatus = WIFI_DISCONNECTED;
static unsigned long wifiConnectionStartTime = 0;
static const unsigned long WIFI_CONNECTION_TIMEOUT = 30000; // 30秒超时

// 初始化Wi-Fi模块
bool initWiFi()
{
    // 设置Wi-Fi模式为站点模式
    WiFi.mode(WIFI_STA);
    delay(100);

    Serial.println("Wi-Fi模块初始化完成");
    return true;
}

// 连接到指定的Wi-Fi网络
bool connectToWiFi(const char *ssid, const char *password)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("已经连接到Wi-Fi网络，先断开连接");
        WiFi.disconnect();
        delay(500);
    }

    Serial.print("正在连接到Wi-Fi: ");
    Serial.println(ssid);

    // 记录连接开始时间
    wifiConnectionStartTime = millis();
    currentWiFiStatus = WIFI_CONNECTING;

    // 开始连接
    WiFi.begin(ssid, password);

    // 等待连接完成或超时
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_CONNECTION_TIMEOUT)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();

    // 检查连接结果
    if (WiFi.status() == WL_CONNECTED)
    {
        currentWiFiStatus = WIFI_CONNECTED;
        Serial.print("Wi-Fi连接成功! IP地址: ");
        Serial.println(WiFi.localIP());
        return true;
    }
    else
    {
        currentWiFiStatus = WIFI_CONNECTION_FAILED;
        Serial.println("Wi-Fi连接失败");
        return false;
    }
}

// 断开Wi-Fi连接
void disconnectWiFi()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        WiFi.disconnect();
        currentWiFiStatus = WIFI_DISCONNECTED;
        Serial.println("已断开Wi-Fi连接");
    }
}

// 获取当前Wi-Fi连接状态
WiFiConnectionStatus getWiFiStatus()
{
    // 更新内部状态变量
    if (currentWiFiStatus == WIFI_CONNECTING)
    {
        // 检查是否已连接或超时
        if (WiFi.status() == WL_CONNECTED)
        {
            currentWiFiStatus = WIFI_CONNECTED;
        }
        else if (millis() - wifiConnectionStartTime > WIFI_CONNECTION_TIMEOUT)
        {
            currentWiFiStatus = WIFI_CONNECTION_FAILED;
        }
    }
    else if (currentWiFiStatus == WIFI_CONNECTED && WiFi.status() != WL_CONNECTED)
    {
        // 连接已断开
        currentWiFiStatus = WIFI_DISCONNECTED;
    }

    return currentWiFiStatus;
}

// 获取当前连接的Wi-Fi信号强度
int getWiFiSignalStrength()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return WiFi.RSSI();
    }
    else
    {
        return 0; // 未连接时返回0
    }
}

// 获取当前连接的Wi-Fi SSID
String getConnectedSSID()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return WiFi.SSID();
    }
    else
    {
        return ""; // 未连接时返回空字符串
    }
}

// 获取当前IP地址
String getLocalIP()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return WiFi.localIP().toString();
    }
    else
    {
        return "0.0.0.0"; // 未连接时返回0.0.0.0
    }
}