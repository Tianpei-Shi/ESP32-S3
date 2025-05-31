#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>

// Wi-Fi连接状态枚举
enum WiFiConnectionStatus
{
    WIFI_DISCONNECTED,
    WIFI_CONNECTING,
    WIFI_CONNECTED,
    WIFI_CONNECTION_FAILED
};

// 初始化Wi-Fi模块
bool initWiFi();

// 连接到指定的Wi-Fi网络
bool connectToWiFi(const char *ssid, const char *password);

// 断开Wi-Fi连接
void disconnectWiFi();

// 获取当前Wi-Fi连接状态
WiFiConnectionStatus getWiFiStatus();

// 获取当前连接的Wi-Fi信号强度
int getWiFiSignalStrength();

// 获取当前连接的Wi-Fi SSID
String getConnectedSSID();

// 获取当前IP地址
String getLocalIP();

#endif // WIFI_MANAGER_H