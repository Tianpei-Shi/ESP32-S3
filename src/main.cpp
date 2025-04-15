#include <Arduino.h>
#include "oled_display.h"
#include "sensors.h"
#include "audio.h"
#include "ai_interface.h"

// 运行状态变量
bool recordingVoice = false;
unsigned long lastSensorUpdateTime = 0;
unsigned long lastVoiceCheckTime = 0;
const unsigned long SENSOR_UPDATE_INTERVAL = 2000; // 传感器更新间隔：2秒
const unsigned long VOICE_CHECK_INTERVAL = 5000;   // 语音检测间隔：5秒

void setup()
{
  // 初始化串口
  Serial.begin(115200);
  Serial.println("System starting...");

  // 初始化传感器
  if (!initSensors())
  {
    Serial.println("Sensor initialization failed!");
  }

  // 初始化OLED显示屏
  if (!initOLEDDisplays())
  {
    Serial.println("OLED display initialization failed!");
  }

  // 初始化麦克风
  if (!initMicrophone())
  {
    Serial.println("Microphone initialization failed!");
  }

  // 初始化AI接口
  if (!initAIInterface())
  {
    Serial.println("AI interface initialization failed!");
  }

  // 显示启动画面
  showStartupScreen();

  Serial.println("System initialization complete!");
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
    float temperature, humidity;
    readDHTData(temperature, humidity);

    int pm25Value = readPM25();

    bool waterLevelWarning;
    int waterLevel = readWaterLevel(waterLevelWarning);

    // 更新OLED显示
    updateDisplay096(temperature, humidity, pm25Value, waterLevel, waterLevelWarning);
  }

  // 2. 检查是否需要进行语音识别
  if (currentMillis - lastVoiceCheckTime >= VOICE_CHECK_INTERVAL && !recordingVoice)
  {
    lastVoiceCheckTime = currentMillis;
    recordingVoice = true;

    // 语音识别
    String voiceInput = captureAndRecognizeSpeech();
    Serial.print("Recognized voice: ");
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

  // 3. 更新AI动画状态 - 这个可以每次循环都更新，不需要等待
  AIState aiState = getAIState();
  updateDisplay091(aiState.userInput, aiState.aiResponse);

  // 简单的延时，避免过快循环
  delay(100);
}
