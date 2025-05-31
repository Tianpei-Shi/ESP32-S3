#include "huawei_iot.h"
#include <WiFiClientSecure.h>
#include <time.h>
#include <mbedtls/md.h>

// 华为云IoT平台配置（根据下载的连接信息文件更新）
const char *mqtt_server = "8e5166a6be.st1.iotda-device.cn-east-3.myhuaweicloud.com";
const int mqtt_port = 8883;
// 设备连接信息 - 基础信息，将根据需要动态生成完整的连接参数
const char *device_id = "6805063cfde7ae374599a9ad_ESP32S3";     // 设备ID
const char *device_secret = "Stp20031017";                      // 设备密钥
char mqtt_client_id[128];                                       // 动态生成，格式为：设备ID_0_0_时间戳08
const char *mqtt_username = "6805063cfde7ae374599a9ad_ESP32S3"; // 用户名就是设备ID
char mqtt_password[65];                                         // HMACSHA256生成的密码，64个字符+结束符

// MQTT客户端
WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);

// 连接状态
static bool iotConnected = false;
int reconnectAttempts = 0;

// 计算HMACSHA256摘要
// key: 密钥（时间戳）
// message: 消息内容（设备密钥）
// output: 输出缓冲区，存储计算结果（64个十六进制字符+结束符）
void calculateHMACSHA256(const char *message, const char *key, char *output)
{
    byte hmacResult[32];

    mbedtls_md_context_t ctx;
    mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;

    mbedtls_md_init(&ctx);
    mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 1);
    mbedtls_md_hmac_starts(&ctx, (const unsigned char *)key, strlen(key));
    mbedtls_md_hmac_update(&ctx, (const unsigned char *)message, strlen(message));
    mbedtls_md_hmac_finish(&ctx, hmacResult);
    mbedtls_md_free(&ctx);

    for (int i = 0; i < 32; i++)
    {
        sprintf(output + i * 2, "%02x", hmacResult[i]);
    }
    output[64] = '\0';
}

// 获取当前时间戳（格式：YYYYMMDD08）HH固定为08
void getCurrentTimestamp(char *timestamp)
{
    time_t now;
    struct tm timeinfo;

    time(&now);
    localtime_r(&now, &timeinfo);

    // 格式化为YYYYMMDD08，小时部分固定为08
    sprintf(timestamp, "%04d%02d%02d08",
            timeinfo.tm_year + 1900,
            timeinfo.tm_mon + 1,
            timeinfo.tm_mday);
}

// 生成MQTT连接凭证
void generateMQTTCredentials()
{
    char timestamp[13];
    getCurrentTimestamp(timestamp);

    // 生成ClientId: 设备ID_0_0_时间戳08
    sprintf(mqtt_client_id, "%s_0_0_%s", device_id, timestamp);

    // 计算Password: HMACSHA256(secret, timestamp)
    calculateHMACSHA256(device_secret, timestamp, mqtt_password);

    Serial.print("生成的ClientID: ");
    Serial.println(mqtt_client_id);
    Serial.print("生成的Password: ");
    Serial.println(mqtt_password);
}

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

    // 设置NTP获取时间（需要预先配置NTP）
    configTime(8 * 3600, 0, "ntp.ntsc.ac.cn", "ntp1.aliyun.com");

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

    // 重新生成连接凭证（自动更新ClientId和Password）
    generateMQTTCredentials();

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

    // 创建services数组
    JsonArray servicesArray = doc.createNestedArray("services");

    // 添加服务对象
    JsonObject serviceObj = servicesArray.createNestedObject();
    serviceObj["service_id"] = "environment";

    // 创建properties对象
    JsonObject propertiesObj = serviceObj.createNestedObject("properties");
    // 处理温度值，保留一位小数
    propertiesObj["temperature"] = round(temperature * 10.0) / 10.0; // 浮点数，保留一位小数
    propertiesObj["humidity"] = (int)humidity;                       // 转为整数
    propertiesObj["pm25"] = pm25;                                    // 整数，注意不使用"PM2.5"而是"pm25"
    propertiesObj["water"] = waterLevel;                             // 整数值（1-100的百分比）

    // 序列化为JSON字符串
    char jsonBuffer[256];
    serializeJson(doc, jsonBuffer);

    // 创建主题
    char topic[128];
    sprintf(topic, "$oc/devices/%s/sys/properties/report", mqtt_username);

    // 发送MQTT消息
    Serial.println("发送属性报告...");
    Serial.println(jsonBuffer); // 打印JSON内容用于调试

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