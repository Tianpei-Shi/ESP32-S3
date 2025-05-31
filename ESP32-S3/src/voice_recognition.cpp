#include "voice_recognition.h"
#include "LD3320_ESP32.h"

// 创建LD3320实例
LD3320_ESP32 ld3320;

// 语音识别状态变量
static bool voiceRecognitionEnabled = false;
static bool commandExecutionEnabled = false;
static unsigned long lastRecognitionTime = 0;
static const unsigned long RECOGNITION_INTERVAL = 3000; // 语音识别间隔3秒

// LD3320库需要的processUserInput函数实现
bool processUserInput(const String &input, String &response)
{
    Serial.print("processUserInput收到输入: '");
    Serial.print(input);
    Serial.println("'");

    if (input == "小一")
    {
        response = "已唤醒";
        commandExecutionEnabled = true;
        lastRecognitionTime = millis();
        Serial.println("✓ 小一唤醒成功！");
        Serial.print("✓ commandExecutionEnabled = ");
        Serial.println(commandExecutionEnabled ? "true" : "false");
        Serial.print("✓ lastRecognitionTime = ");
        Serial.println(lastRecognitionTime);
        return true;
    }
    else if (input == "加湿")
    {
        if (commandExecutionEnabled)
        {
            response = "加湿器已开启";
            Serial.println("✓ 执行加湿命令");
            return true;
        }
        else
        {
            response = "请先说小一唤醒";
            Serial.println("✗ 命令模式未激活，无法执行加湿");
            return false;
        }
    }
    else if (input == "关闭加湿")
    {
        if (commandExecutionEnabled)
        {
            response = "加湿器已关闭";
            Serial.println("✓ 执行关闭加湿命令");
            return true;
        }
        else
        {
            response = "请先说小一唤醒";
            Serial.println("✗ 命令模式未激活，无法执行关闭加湿");
            return false;
        }
    }
    else if (input == "前进")
    {
        if (commandExecutionEnabled)
        {
            response = "开始前进";
            Serial.println("✓ 执行前进命令");
            return true;
        }
        else
        {
            response = "请先说小一唤醒";
            Serial.println("✗ 命令模式未激活，无法执行前进");
            return false;
        }
    }
    else if (input == "后退")
    {
        if (commandExecutionEnabled)
        {
            response = "开始后退";
            Serial.println("✓ 执行后退命令");
            return true;
        }
        else
        {
            response = "请先说小一唤醒";
            Serial.println("✗ 命令模式未激活，无法执行后退");
            return false;
        }
    }
    else
    {
        response = "命令未识别";
        Serial.print("✗ 未知命令: '");
        Serial.print(input);
        Serial.println("'");
        if (!commandExecutionEnabled)
        {
            Serial.println("✗ 提示：命令模式未激活，请先说'小一'");
        }
        return false;
    }
}

bool initVoiceRecognition()
{
    Serial.println("正在初始化LD3320语音识别模块...");

    // 初始化LD3320模块
    if (!ld3320.begin())
    {
        Serial.println("LD3320模块初始化失败!");
        return false;
    }

    Serial.println("LD3320语音识别模块初始化完成");
    Serial.println("硬件连接:");
    Serial.println("  LD3320 TXD -> ESP32 GPIO14 (RX)");
    Serial.println("  LD3320 RXD -> ESP32 GPIO38 (TX)");
    Serial.println("  LD3320 VCC -> ESP32 3.3V");
    Serial.println("  LD3320 GND -> ESP32 GND");
    Serial.println("");
    Serial.println("加湿器继电器: GPIO 5");
    Serial.println("L298N电机驱动模块:");
    Serial.println("  IN1 -> GPIO 1 (电机A方向控制1)");
    Serial.println("  IN2 -> GPIO 2 (电机A方向控制2)");
    Serial.println("  IN3 -> GPIO 3 (电机B方向控制1)");
    Serial.println("  IN4 -> GPIO 4 (电机B方向控制2)");
    Serial.println("");

    voiceRecognitionEnabled = true;
    commandExecutionEnabled = false; // 确保初始状态为待机
    Serial.println("LD3320语音识别模块初始化成功!");
    Serial.println("支持的语音命令:");
    Serial.println("1. '小艺' -> 串口输出'shou dao' -> 唤醒系统10秒");
    Serial.println("2. '加湿' -> 串口输出'yi jia shi' -> 开启加湿器");
    Serial.println("3. '关闭加湿' -> 串口输出'guan bi jia shi' -> 关闭加湿器");
    Serial.println("4. '前进' -> 串口输出'qian jin' -> 小车前进");
    Serial.println("5. '后退' -> 串口输出'hou tui' -> 小车后退");
    Serial.println("");

    return true;
}

void updateVoiceRecognition()
{
    // 更新LD3320模块，检查串口数据
    ld3320.update();

    // 检查命令模式超时 (10秒后自动关闭)
    static bool lastCommandModeState = false;
    if (commandExecutionEnabled && (millis() - lastRecognitionTime > 10000))
    {
        commandExecutionEnabled = false;
        Serial.println("⏰ 命令模式超时(10秒)，自动返回待机状态");
    }

    // 调试：状态变化时输出信息
    if (commandExecutionEnabled != lastCommandModeState)
    {
        Serial.print("🔄 语音识别状态变化: ");
        Serial.print(lastCommandModeState ? "聆听" : "待机");
        Serial.print(" -> ");
        Serial.println(commandExecutionEnabled ? "聆听" : "待机");
        lastCommandModeState = commandExecutionEnabled;
    }
}

String captureAndRecognizeVoice()
{
    if (!voiceRecognitionEnabled)
    {
        return "";
    }

    // 注意：在串口通信方式下，语音识别是自动进行的
    // LD3320模块会自动发送识别结果到串口
    // 这个函数保留是为了兼容性，但实际工作由updateVoiceRecognition()完成
    Serial.println("语音识别正在运行，请说话...");
    return "";
}

void processVoiceCommand(const String &command)
{
    if (!voiceRecognitionEnabled)
    {
        return;
    }

    Serial.print("处理语音命令: ");
    Serial.println(command);

    if (command == "小一")
    {
        commandExecutionEnabled = true;
        Serial.println("收到唤醒词，已激活命令模式");
    }
    else if (command == "加湿" && commandExecutionEnabled)
    {
        ld3320.setHumidifier(true);
        Serial.println("加湿器已开启");
    }
    else if (command == "关闭加湿" && commandExecutionEnabled)
    {
        ld3320.setHumidifier(false);
        Serial.println("加湿器已关闭");
    }
    else if (command == "前进" && commandExecutionEnabled)
    {
        ld3320.stopAllMotors();
        ld3320.setMotorForward(true);
        Serial.println("小车开始前进");
    }
    else if (command == "后退" && commandExecutionEnabled)
    {
        ld3320.stopAllMotors();
        ld3320.setMotorBackward(true);
        Serial.println("小车开始后退");
    }
    else if (command == "停止")
    {
        ld3320.stopAllMotors();
        Serial.println("所有电机已停止");
    }
    else if (!commandExecutionEnabled)
    {
        Serial.println("请先说'小一'唤醒语音控制");
    }
}

bool isVoiceRecognitionEnabled()
{
    return voiceRecognitionEnabled;
}

void setVoiceRecognitionEnabled(bool enabled)
{
    voiceRecognitionEnabled = enabled;
    if (!enabled)
    {
        commandExecutionEnabled = false;
        // 停止所有设备
        ld3320.stopAllMotors();
        ld3320.setHumidifier(false);
    }
    Serial.println(enabled ? "语音识别已启用" : "语音识别已禁用");
}

VoiceRecognitionStatus getVoiceRecognitionStatus()
{
    VoiceRecognitionStatus status;
    status.enabled = voiceRecognitionEnabled;
    status.commandModeActive = commandExecutionEnabled;
    status.lastRecognitionTime = lastRecognitionTime;
    return status;
}

// 用于兼容原有接口的函数
String captureAndRecognizeSpeech()
{
    return captureAndRecognizeVoice();
}

// 通过LD3320实例提供的接口函数
bool getHumidifierStatus()
{
    return ld3320.getHumidifierState();
}

void setHumidifierState(bool state)
{
    ld3320.setHumidifier(state);
}

void setMotorForward(bool state)
{
    ld3320.setMotorForward(state);
}

void setMotorBackward(bool state)
{
    ld3320.setMotorBackward(state);
}

void stopAllMotors()
{
    ld3320.stopAllMotors();
}