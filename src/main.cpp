#include <Arduino.h>
#include "oled_display.h"
#include "sensors.h"
#include "audio.h"
#include "ai_interface.h"
#include "wifi_manager.h"
#include "huawei_iot.h"

// 引用华为云连接信息中的变量
extern const char *mqtt_username; // 从huawei_iot.cpp中引用

// 运行状态变量
bool recordingVoice = false;
unsigned long lastSensorUpdateTime = 0;
unsigned long lastVoiceCheckTime = 0;
unsigned long lastWiFiStatusCheckTime = 0;
unsigned long lastIoTReportTime = 0;
const unsigned long SENSOR_UPDATE_INTERVAL = 2000;      // 传感器更新间隔：2秒
const unsigned long VOICE_CHECK_INTERVAL = 5000;        // 语音检测间隔：5秒
const unsigned long WIFI_STATUS_CHECK_INTERVAL = 10000; // WiFi状态检查间隔：10秒
const unsigned long IOT_REPORT_INTERVAL = 30000;        // IoT数据上报间隔：30秒

// WiFi配置信息
const char *DEFAULT_WIFI_SSID = "TP";           // 默认WiFi名称
const char *DEFAULT_WIFI_PASSWORD = "20031017";     // 默认WiFi密码

// IoT连接尝试次数
int iotConnectionAttempts = 0;
const int MAX_IOT_CONNECTION_ATTEMPTS = 5;

// 存储最新的传感器数据
float currentTemperature = 0;
float currentHumidity = 0;
int currentPM25 = 0;
int currentWaterLevel = 0;
bool currentWaterLevelWarning = false;

void setup()
{
  // 初始化串口
  Serial.begin(115200);
  Serial.println("系统启动中...");

  // 初始化传感器
  if (!initSensors())
  {
    Serial.println("传感器初始化失败!");
  }

  // 初始化OLED显示屏
  if (!initOLEDDisplays())
  {
    Serial.println("OLED显示屏初始化失败!");
  }

  // 初始化麦克风
  if (!initMicrophone())
  {
    Serial.println("麦克风初始化失败!");
  }

  // 初始化AI接口
  if (!initAIInterface())
  {
    Serial.println("AI接口初始化失败!");
  }

  // 初始化WiFi
  if (!initWiFi())
  {
    Serial.println("WiFi初始化失败!");
  }
  else
  {
    // 尝试连接到WiFi
    if (connectToWiFi(DEFAULT_WIFI_SSID, DEFAULT_WIFI_PASSWORD))
    {
      Serial.println("WiFi连接成功!");
      delay(500); // 稳定WiFi连接

      // 初始化华为云IoT连接
      if (initHuaweiIoT())
      {
        Serial.println("华为云IoT初始化成功!");

        // 尝试连接到华为云IoT平台
        bool connected = false;
        for (int i = 0; i < MAX_IOT_CONNECTION_ATTEMPTS && !connected; i++)
        {
          connected = connectToHuaweiIoT();

          if (!connected)
          {
            delay(3000); // 失败后等待3秒重试
          }
        }
      }
    }
  }

  // 显示启动画面
  showStartupScreen();

  Serial.println("系统初始化完成!");
}

void loop()
{
  // 当前时间
  unsigned long currentMillis = millis();

  // 1. 定期更新传感器数据和显示
  if (currentMillis - lastSensorUpdateTime >= SENSOR_UPDATE_INTERVAL)
  {
    lastSensorUpdateTime = currentMillis;

    // 读取传感器数据
    readDHTData(currentTemperature, currentHumidity);
    currentPM25 = readPM25();
    currentWaterLevel = readWaterLevel(currentWaterLevelWarning);

    // 更新OLED显示
    updateDisplay096(currentTemperature, currentHumidity, currentPM25, currentWaterLevel, currentWaterLevelWarning);
  }

  // 2. 检查是否需要进行语音识别
  if (currentMillis - lastVoiceCheckTime >= VOICE_CHECK_INTERVAL && !recordingVoice)
  {
    lastVoiceCheckTime = currentMillis;
    recordingVoice = true;

    // 语音识别
    String voiceInput = captureAndRecognizeSpeech();
    Serial.print("识别到的语音: ");
    Serial.println(voiceInput);

    // 如果识别到语音，处理AI回复
    if (voiceInput.length() > 0)
    {
      String aiResponse;
      processUserInput(voiceInput, aiResponse);

      // 更新AI显示
      updateDisplay091(voiceInput, aiResponse);
    }

    recordingVoice = false;
  }

  // 3. 定期检查WiFi连接状态
  if (currentMillis - lastWiFiStatusCheckTime >= WIFI_STATUS_CHECK_INTERVAL)
  {
    lastWiFiStatusCheckTime = currentMillis;

    // 获取WiFi状态
    WiFiConnectionStatus wifiStatus = getWiFiStatus();

    // 如果WiFi断开连接，尝试重新连接
    if (wifiStatus == WIFI_DISCONNECTED || wifiStatus == WIFI_CONNECTION_FAILED)
    {
      Serial.println("WiFi连接断开，尝试重新连接...");
      connectToWiFi(DEFAULT_WIFI_SSID, DEFAULT_WIFI_PASSWORD);
    }
    else if (wifiStatus == WIFI_CONNECTED)
    {
      // 检查华为云IoT连接
      if (!isDeviceConnected())
      {
        Serial.println("华为云IoT连接断开，尝试重新连接...");
        connectToHuaweiIoT();
      }
    }
  }

  // 4. 定期向华为云IoT平台上报数据
  if (currentMillis - lastIoTReportTime >= IOT_REPORT_INTERVAL)
  {
    lastIoTReportTime = currentMillis;

    // 如果WiFi已连接且华为云IoT已连接，上报数据
    if (getWiFiStatus() == WIFI_CONNECTED && isDeviceConnected())
    {
      Serial.println("向华为云IoT平台上报数据...");
      sendPropertyReport(currentTemperature, currentHumidity, currentPM25, currentWaterLevel);
    }
  }

  // 5. 维持华为云IoT连接
  if (getWiFiStatus() == WIFI_CONNECTED)
  {
    checkAndMaintainConnection();
  }

  // 6. 更新AI动画状态 - 这个可以每次循环都更新，不需要等待
  AIState aiState = getAIState();
  updateDisplay091(aiState.userInput, aiState.aiResponse);

  // 简单的延时，避免过快循环
  delay(100);
}