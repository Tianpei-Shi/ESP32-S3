#include "LD3320_ESP32.h"

// 包含AI接口，用于更新显示
extern bool processUserInput(const String &input, String &response);

// 加湿器状态变量
static bool humidifierState = false;

LD3320_ESP32::LD3320_ESP32()
{
    serialPort = nullptr;
    receivedData = "";
    voiceCommandReady = false;
    lastCommandTime = 0;
}

bool LD3320_ESP32::begin()
{
    Serial.println("正在初始化LD3320语音识别模块...");

    // 初始化串口2用于与LD3320通信
    serialPort = &Serial2;
    serialPort->begin(115200, SERIAL_8N1, LD3320_RX_PIN, LD3320_TX_PIN);

    // 初始化继电器引脚
    pinMode(RELAY_HUMIDIFIER, OUTPUT);

    // 初始化L298N电机驱动引脚
    pinMode(L298N_IN1, OUTPUT);
    pinMode(L298N_IN2, OUTPUT);
    pinMode(L298N_IN3, OUTPUT);
    pinMode(L298N_IN4, OUTPUT);

    // 初始化继电器状态（关闭）
    digitalWrite(RELAY_HUMIDIFIER, LOW);
    humidifierState = false;

    // 初始化电机状态（停止）
    digitalWrite(L298N_IN1, LOW);
    digitalWrite(L298N_IN2, LOW);
    digitalWrite(L298N_IN3, LOW);
    digitalWrite(L298N_IN4, LOW);

    Serial.println("LD3320语音识别模块初始化完成");
    Serial.println("串口配置: RX=GPIO14, TX=GPIO38, 波特率=115200");
    Serial.println("L298N电机驱动连接:");
    Serial.println("  IN1 -> GPIO 1 (电机A方向控制1)");
    Serial.println("  IN2 -> GPIO 2 (电机A方向控制2)");
    Serial.println("  IN3 -> GPIO 3 (电机B方向控制1)");
    Serial.println("  IN4 -> GPIO 4 (电机B方向控制2)");
    Serial.println("支持的语音命令:");
    Serial.println("1. '小一' - 唤醒词");
    Serial.println("2. '加湿' - 开启加湿器");
    Serial.println("3. '关闭加湿' - 关闭加湿器");
    Serial.println("4. '前进' - 小车前进");
    Serial.println("5. '后退' - 小车后退");

    return true;
}

void LD3320_ESP32::update()
{
    if (!serialPort)
    {
        Serial.println("❌ LD3320串口未初始化");
        return;
    }

    // 检查串口是否有数据
    if (serialPort->available())
    {
        Serial.print("📨 LD3320串口有数据，字节数: ");
        Serial.println(serialPort->available());
    }

    while (serialPort->available())
    {
        char c = serialPort->read();
        Serial.print("接收字符: '");
        Serial.print(c);
        Serial.print("' (ASCII: ");
        Serial.print((int)c);
        Serial.println(")");

        if (c == '\n' || c == '\r')
        {
            if (receivedData.length() > 0)
            {
                Serial.print("🎯 收到完整LD3320数据: '");
                Serial.print(receivedData);
                Serial.println("'");
                parseVoiceCommand(receivedData);
                receivedData = "";
            }
        }
        else
        {
            receivedData += c;
        }
    }

    // 检查命令超时
    if (voiceCommandReady && (millis() - lastCommandTime > COMMAND_TIMEOUT))
    {
        voiceCommandReady = false;
        Serial.println("⏰ LD3320命令超时，清除状态");
    }
}

void LD3320_ESP32::parseVoiceCommand(const String &command)
{
    String trimmedCommand = command;
    trimmedCommand.trim();

    Serial.print("🔍 解析语音命令: '");
    Serial.print(trimmedCommand);
    Serial.print("' (长度: ");
    Serial.print(trimmedCommand.length());
    Serial.println(")");

    Serial.print("🔍 期望的唤醒词: '");
    Serial.print(VOICE_CMD_WAKE_UP);
    Serial.print("' (长度: ");
    Serial.print(String(VOICE_CMD_WAKE_UP).length());
    Serial.println(")");

    // 检查多种可能的唤醒词格式，主要是"shou dao"
    bool isWakeUpCommand = false;
    if (trimmedCommand == VOICE_CMD_WAKE_UP || // "shou dao" (主要)
        trimmedCommand == "xiao yi" ||         // "xiao yi" (备用)
        trimmedCommand == "xiaoyi" ||          // "xiaoyi" (连写备用)
        trimmedCommand.indexOf("shou") >= 0 || // 包含"shou"的任何变体
        trimmedCommand.indexOf("dao") >= 0)    // 包含"dao"的任何变体
    {
        isWakeUpCommand = true;
    }

    if (isWakeUpCommand)
    {
        Serial.println("✅ 匹配成功！收到唤醒词确认 (小艺 -> shou dao)");
        voiceCommandReady = true;
        lastCommandTime = millis();

        // 更新AI交互显示
        String aiResponse;
        processUserInput("小一", aiResponse);
    }
    else if (trimmedCommand == VOICE_CMD_HUMIDIFY) // "yi jia shi"
    {
        Serial.println("✅ 匹配成功！收到加湿命令");
        handleVoiceCommand("加湿");

        // 更新AI交互显示
        String aiResponse;
        processUserInput("加湿", aiResponse);
    }
    else if (trimmedCommand == VOICE_CMD_STOP_HUMIDIFY) // "guan bi jia shi"
    {
        Serial.println("✅ 匹配成功！收到关闭加湿命令");
        handleVoiceCommand("关闭加湿");

        // 更新AI交互显示
        String aiResponse;
        processUserInput("关闭加湿", aiResponse);
    }
    else if (trimmedCommand == VOICE_CMD_FORWARD) // "qian jin"
    {
        Serial.println("✅ 匹配成功！收到前进命令");
        handleVoiceCommand("前进");

        // 更新AI交互显示
        String aiResponse;
        processUserInput("前进", aiResponse);
    }
    else if (trimmedCommand == VOICE_CMD_BACKWARD) // "hou tui"
    {
        Serial.println("✅ 匹配成功！收到后退命令");
        handleVoiceCommand("后退");

        // 更新AI交互显示
        String aiResponse;
        processUserInput("后退", aiResponse);
    }
    else
    {
        Serial.print("❌ 未识别的命令: '");
        Serial.print(trimmedCommand);
        Serial.println("'");
        Serial.println("📝 支持的命令列表:");
        Serial.println("   - shou dao (小艺唤醒，模块固定输出)");
        Serial.println("   - yi jia shi (加湿)");
        Serial.println("   - guan bi jia shi (关闭加湿)");
        Serial.println("   - qian jin (前进)");
        Serial.println("   - hou tui (后退)");
    }
}

bool LD3320_ESP32::hasNewCommand()
{
    return voiceCommandReady;
}

String LD3320_ESP32::getLastCommand()
{
    return receivedData;
}

void LD3320_ESP32::handleVoiceCommand(const String &command)
{
    Serial.print("执行语音命令: ");
    Serial.println(command);

    if (command == "加湿")
    {
        setHumidifier(true);
        Serial.println("加湿器已开启 (GPIO 5 = HIGH)");
    }
    else if (command == "关闭加湿")
    {
        setHumidifier(false);
        Serial.println("加湿器已关闭 (GPIO 5 = LOW)");
    }
    else if (command == "前进")
    {
        stopAllMotors();
        setMotorForward(true);
        Serial.println("小车开始前进");
    }
    else if (command == "后退")
    {
        stopAllMotors();
        setMotorBackward(true);
        Serial.println("小车开始后退");
    }
    else if (command == "停止")
    {
        stopAllMotors();
        Serial.println("所有电机已停止");
    }
}

// 继电器控制函数
void LD3320_ESP32::setHumidifier(bool state)
{
    digitalWrite(RELAY_HUMIDIFIER, state ? HIGH : LOW);
    humidifierState = state;
    Serial.print("加湿器状态: ");
    Serial.println(state ? "开启" : "关闭");
    Serial.print("GPIO 5 输出: ");
    Serial.println(state ? "HIGH" : "LOW");
}

void LD3320_ESP32::setMotorForward(bool state)
{
    if (state)
    {
        // 双电机前进：电机A和电机B都正转
        digitalWrite(L298N_IN1, HIGH);
        digitalWrite(L298N_IN2, LOW);
        digitalWrite(L298N_IN3, HIGH);
        digitalWrite(L298N_IN4, LOW);
        Serial.println("L298N: 双电机前进 (IN1=HIGH, IN2=LOW, IN3=HIGH, IN4=LOW)");
    }
    else
    {
        stopAllMotors();
    }
}

void LD3320_ESP32::setMotorBackward(bool state)
{
    if (state)
    {
        // 双电机后退：电机A和电机B都反转
        digitalWrite(L298N_IN1, LOW);
        digitalWrite(L298N_IN2, HIGH);
        digitalWrite(L298N_IN3, LOW);
        digitalWrite(L298N_IN4, HIGH);
        Serial.println("L298N: 双电机后退 (IN1=LOW, IN2=HIGH, IN3=LOW, IN4=HIGH)");
    }
    else
    {
        stopAllMotors();
    }
}

void LD3320_ESP32::stopAllMotors()
{
    // 停止所有电机
    digitalWrite(L298N_IN1, LOW);
    digitalWrite(L298N_IN2, LOW);
    digitalWrite(L298N_IN3, LOW);
    digitalWrite(L298N_IN4, LOW);
    Serial.println("L298N: 所有电机停止 (所有IN引脚=LOW)");
}

bool LD3320_ESP32::getHumidifierState()
{
    return humidifierState;
}