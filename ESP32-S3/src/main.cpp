#include <Arduino.h>
#include "oled_display.h"
#include "sensors.h"
#include "voice_recognition.h" // 替换原有的audio.h
#include "wifi_manager.h"
#include "huawei_iot.h"
#include "sensor_readings.h"

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
const char *DEFAULT_WIFI_PASSWORD = "20031017"; // 默认WiFi密码

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
  Serial.begin(115200);
  Serial.println("=== ESP32-S3 智能加湿小车启动 ===");

  // 初始化各个模块
  if (!initSensorReadings())
  {
    Serial.println("传感器初始化失败！");
    return;
  }

  if (!initVoiceRecognition())
  {
    Serial.println("语音识别初始化失败！");
    return;
  }

  // 初始化OLED显示屏
  if (!initOLEDDisplays())
  {
    Serial.println("OLED显示屏初始化失败!");
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

  Serial.println("所有模块初始化完成！");
  Serial.println("系统已准备就绪");
  Serial.println("请说出语音命令：");
  Serial.println("1. 先说'小艺'唤醒系统");
  Serial.println("2. 然后说'加湿'或'关闭加湿'控制加湿器");
  Serial.println("3. 或说'前进'、'后退'控制小车运动");
  Serial.println("注意：加湿器通过GPIO 5控制，默认关闭状态");
}

void loop()
{
  // 检查串口是否有数据（用于调试LD3320通信）
  static unsigned long lastSerialDebugTime = 0;
  if (millis() - lastSerialDebugTime >= 3000) // 每3秒检查一次
  {
    Serial.print("🔍 LD3320串口状态 - 可用字节数: ");
    Serial.print(Serial2.available());
    Serial.print(" | 串口配置: GPIO14(RX), GPIO38(TX), 115200bps");
    Serial.println();
    lastSerialDebugTime = millis();
  }

  // 更新语音识别（检查串口数据）
  updateVoiceRecognition();

  // 读取传感器数据
  updateSensorReadings();

  // 定期更新0.96寸OLED显示屏（传感器数据）
  static unsigned long lastDisplayUpdateTime = 0;
  if (millis() - lastDisplayUpdateTime >= SENSOR_UPDATE_INTERVAL)
  {
    // 获取传感器数据
    currentTemperature = getTemperature();
    currentHumidity = getHumidity();
    currentPM25 = getPM25();
    currentWaterLevel = getWaterLevel();

    // 更新0.96寸显示屏
    updateDisplay096(currentTemperature, currentHumidity, currentPM25, currentWaterLevel, currentWaterLevelWarning);

    lastDisplayUpdateTime = millis();
  }

  // 定期更新0.91寸OLED显示屏（根据语音识别状态）
  static unsigned long lastAIDisplayUpdateTime = 0;
  if (millis() - lastAIDisplayUpdateTime >= 200) // 提高更新频率以快速响应状态变化
  {
    VoiceRecognitionStatus voiceStatus = getVoiceRecognitionStatus();

    // 调试信息：显示当前状态
    static bool lastCommandModeActive = false;
    static unsigned long lastDebugTime = 0;

    if (voiceStatus.commandModeActive != lastCommandModeActive)
    {
      Serial.print("📺 显示屏状态变化: ");
      Serial.print(lastCommandModeActive ? "聆听模式" : "待机模式");
      Serial.print(" -> ");
      Serial.println(voiceStatus.commandModeActive ? "聆听模式" : "待机模式");
      lastCommandModeActive = voiceStatus.commandModeActive;
    }

    // 每2秒输出一次当前状态（用于调试）
    if (millis() - lastDebugTime >= 2000)
    {
      Serial.print("📺 当前显示状态: ");
      Serial.print(voiceStatus.commandModeActive ? "聆听中" : "待机中");
      if (voiceStatus.commandModeActive)
      {
        unsigned long timeLeft = 10000 - (millis() - voiceStatus.lastRecognitionTime);
        if (timeLeft > 0)
        {
          Serial.print(" (剩余时间: ");
          Serial.print(timeLeft / 1000);
          Serial.print("秒)");
        }
      }
      Serial.println();
      lastDebugTime = millis();
    }

    if (voiceStatus.commandModeActive)
    {
      // 收到"小艺"指令，显示聆听状态
      showDisplay091Listening();
    }
    else
    {
      // 待机状态，显示待机中
      showDisplay091Standby();
    }

    lastAIDisplayUpdateTime = millis();
  }

  // 每隔5秒输出一次系统状态
  static unsigned long lastStatusTime = 0;
  if (millis() - lastStatusTime >= 5000)
  {
    VoiceRecognitionStatus voiceStatus = getVoiceRecognitionStatus();
    Serial.println("========== 系统状态报告 ==========");
    Serial.print("🔊 语音识别启用: ");
    Serial.println(voiceStatus.enabled ? "是" : "否");
    Serial.print("🎯 命令模式激活: ");
    Serial.println(voiceStatus.commandModeActive ? "是 (聆听中)" : "否 (待机中)");
    if (voiceStatus.commandModeActive)
    {
      unsigned long elapsed = millis() - voiceStatus.lastRecognitionTime;
      unsigned long remaining = elapsed < 10000 ? (10000 - elapsed) : 0;
      Serial.print("⏱️  聆听剩余时间: ");
      Serial.print(remaining / 1000);
      Serial.println("秒");
    }
    Serial.print("💧 加湿器状态: ");
    Serial.println(getHumidifierStatus() ? "开启" : "关闭");
    Serial.print("🌡️  温度: ");
    Serial.print(getTemperature());
    Serial.println("°C");
    Serial.print("💨 湿度: ");
    Serial.print(getHumidity());
    Serial.println("%");
    Serial.println("📢 等待语音命令...");
    Serial.println("💡 提示：说'小艺'唤醒系统，LD3320模块会输出'shou dao'");
    Serial.println("🔧 测试命令：串口发送'test_xiaoyi'模拟唤醒");
    Serial.println("================================");

    lastStatusTime = millis();
  }

  // 简化的测试功能
  if (Serial.available())
  {
    String testCommand = Serial.readString();
    testCommand.trim();
    if (testCommand == "test_xiaoyi")
    {
      Serial.println("🧪 测试：模拟接收到'小艺'命令");
      processVoiceCommand("小一");
    }
    else if (testCommand == "test_sleep")
    {
      Serial.println("🧪 测试：重置为待机状态");
      setVoiceRecognitionEnabled(true);
    }
  }

  delay(100); // 主循环延时
}