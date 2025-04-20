#ifndef HUAWEI_IOT_H
#define HUAWEI_IOT_H

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "wifi_manager.h"

// 设备连接信息（main.cpp中需要引用）
extern const char *mqtt_username;
extern const char *mqtt_client_id;

// 初始化华为云IoT设备
bool initHuaweiIoT();

// 连接到华为云IoT平台
bool connectToHuaweiIoT();

// 检查并维持与华为云IoT平台的连接
bool checkAndMaintainConnection();

// 发送设备属性报告
bool sendPropertyReport(float temperature, float humidity, int pm25, int waterLevel);

// 处理来自云平台的命令
void handleCloudCommand(String command, String &response);

// 获取设备连接状态
bool isDeviceConnected();

// MQTT客户端实例（供main.cpp使用）
extern PubSubClient mqttClient;

#endif // HUAWEI_IOT_H