#include "huawei_iot.h"
#include <WiFiClientSecure.h>

// 华为云IoT平台配置（根据下载的连接信息文件更新）
const char *mqtt_server = "xxxxxxxx.st1.iotda-device.cn-east-3.myhuaweicloud.com";
const int mqtt_port = 8883;
// 设备连接信息更新 - 请确保这些信息与华为云平台完全匹配
const char *mqtt_client_id = "xxxxxxxx_ESP32S3_0_1_xxxxxxxx";                 // 客户端ID
const char *mqtt_username = "xxxxxxxx_ESP32S3";                               // 用户名
const char *mqtt_password = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; // 密码

// MQTT客户端
WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);

// 连接状态
static bool iotConnected = false;
int reconnectAttempts = 0;

// MQTT回调函数
void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("收到消息 [");
    Serial.print(topic);
    Serial.print("]: ");

    // 将payload转换为字符串
    char message[length + 1];
    memcpy(message, payload, length);
    message[length] = '\0';
    Serial.println(message);

    // 这里可以添加对特定主题的处理逻辑
    // 例如处理命令、属性设置等
}

// 初始化华为云IoT设备
bool initHuaweiIoT()
{
    Serial.println("初始化华为云IoT连接...");

    // 重置重连计数
    reconnectAttempts = 0;

    // 设置为不验证证书（使用密钥认证方式）
    espClient.setInsecure();

    // 设置服务器超时时间，增加为30秒以应对网络延迟
    espClient.setTimeout(30000);

    // 设置MQTT服务器和回调函数
    mqttClient.setServer(mqtt_server, mqtt_port);
    mqttClient.setCallback(mqttCallback);

    // 设置更大的缓冲区大小以支持更长的消息
    mqttClient.setBufferSize(1024);

    // 设置MQTT保持连接时间为120秒
    mqttClient.setKeepAlive(120);

    return true;
}

// 连接到华为云IoT平台
bool connectToHuaweiIoT()
{
    // 检查WiFi连接
    if (getWiFiStatus() != WIFI_CONNECTED)
    {
        Serial.println("WiFi未连接，无法连接到华为云IoT平台");
        return false;
    }

    Serial.println("正在连接到华为云IoT平台...");
    reconnectAttempts++;

    // 首先断开当前连接（如果有）
    if (mqttClient.connected())
    {
        Serial.println("清除现有连接...");
        mqttClient.disconnect();
        delay(500);
    }

    // 重置MQTT客户端
    mqttClient.setServer(mqtt_server, mqtt_port);

    // 尝试连接MQTT服务器
    if (mqttClient.connect(mqtt_client_id, mqtt_username, mqtt_password))
    {
        Serial.println("华为云IoT平台连接成功！");

        // 订阅设备相关主题
        char commandTopic[128];
        sprintf(commandTopic, "$oc/devices/%s/sys/commands/#", mqtt_username);
        mqttClient.subscribe(commandTopic);

        char messageTopic[128];
        sprintf(messageTopic, "$oc/devices/%s/sys/messages/down", mqtt_username);
        mqttClient.subscribe(messageTopic);

        // 订阅属性响应主题
        char propRespTopic[128];
        sprintf(propRespTopic, "$oc/devices/%s/sys/properties/get/response/#", mqtt_username);
        mqttClient.subscribe(propRespTopic);

        char propSetTopic[128];
        sprintf(propSetTopic, "$oc/devices/%s/sys/properties/set/#", mqtt_username);
        mqttClient.subscribe(propSetTopic);

        iotConnected = true;
        reconnectAttempts = 0; // 连接成功，重置计数

        // 发送设备上线事件
        char bootTopic[128];
        sprintf(bootTopic, "$oc/devices/%s/sys/events/up", mqtt_username);

        // 创建引导消息
        StaticJsonDocument<256> bootDoc;
        JsonObject eventObj = bootDoc.createNestedObject("device_startup");
        eventObj["device_id"] = mqtt_username;
        eventObj["event_time"] = millis();
        eventObj["status"] = "online";

        char jsonBuffer[256];
        serializeJson(bootDoc, jsonBuffer);

        // 发送引导消息
        mqttClient.publish(bootTopic, jsonBuffer);

        return true;
    }
    else
    {
        int state = mqttClient.state();
        Serial.print("华为云IoT平台连接失败，错误码：");
        Serial.println(state);

        iotConnected = false;
        return false;
    }
}

// 检查并维持与华为云IoT平台的连接
bool checkAndMaintainConnection()
{
    if (!mqttClient.connected())
    {
        iotConnected = false;
        // 如果重试次数过多，延长重试间隔
        if (reconnectAttempts > 5)
        {
            delay(reconnectAttempts * 1000); // 根据重试次数延长延迟
        }
        return connectToHuaweiIoT();
    }

    // 处理MQTT消息
    mqttClient.loop();
    return true;
}

// 发送设备属性报告
bool sendPropertyReport(float temperature, float humidity, int pm25, int waterLevel)
{
    if (!mqttClient.connected())
    {
        Serial.println("MQTT未连接，无法发送属性报告");
        return false;
    }

    // 创建JSON文档
    StaticJsonDocument<256> doc;

    // 直接使用平台定义的服务名和属性名（与物模型完全匹配）
    JsonObject serviceData = doc.createNestedObject("environment");
    serviceData["temperature"] = temperature; // 浮点数，保留小数
    serviceData["humidity"] = (int)humidity;  // 转为整数
    serviceData["PM2.5"] = pm25;              // 整数
    serviceData["water"] = waterLevel;        // 整数值，百分比

    // 序列化为JSON字符串
    char jsonBuffer[256];
    serializeJson(doc, jsonBuffer);

    // 创建主题
    char topic[128];
    sprintf(topic, "$oc/devices/%s/sys/properties/report", mqtt_username);

    // 发送MQTT消息
    Serial.println("发送属性报告...");

    return mqttClient.publish(topic, jsonBuffer);
}

// 处理来自云平台的命令
void handleCloudCommand(String command, String &response)
{
    // 解析命令JSON
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, command);

    if (error)
    {
        response = "命令解析失败";
        return;
    }

    // 根据命令内容执行操作
    // 这里需要根据实际的设备命令模型来实现
    response = "命令已执行";
}

// 获取设备连接状态
bool isDeviceConnected()
{
    return iotConnected && mqttClient.connected();
}