#ifndef HUAWEI_IOT_H
#define HUAWEI_IOT_H

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "wifi_manager.h"

// 设备连接信息（main.cpp中需要引用）
extern const char *mqtt_username;
extern char mqtt_client_id[128];
extern char mqtt_password[65];
extern const char *device_id;
extern const char *device_secret;

// 计算HMACSHA256摘要
// message: 消息内容（设备密钥）
// key: 密钥（时间戳）
// output: 输出缓冲区
void calculateHMACSHA256(const char *message, const char *key, char *output);

// 获取当前时间戳（格式：YYYYMMDD08）小时部分固定为08
void getCurrentTimestamp(char *timestamp);

// 生成MQTT连接凭证
void generateMQTTCredentials();

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