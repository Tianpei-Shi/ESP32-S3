#ifndef LD3320_ESP32_H
#define LD3320_ESP32_H

#include <Arduino.h>
#include <HardwareSerial.h>

// 语音命令字符串定义（LD3320模块输出的字符串）
#define VOICE_CMD_WAKE_UP "shou dao"              // 小艺唤醒词，模块输出固定为"shou dao"
#define VOICE_CMD_HUMIDIFY "yi jia shi"           // 已加湿
#define VOICE_CMD_STOP_HUMIDIFY "guan bi jia shi" // 关闭加湿
#define VOICE_CMD_FORWARD "qian jin"              // 前进
#define VOICE_CMD_BACKWARD "hou tui"              // 后退

// ESP32引脚定义
#define LD3320_RX_PIN 14 // ESP32 RX (连接LD3320 TXD)
#define LD3320_TX_PIN 38 // ESP32 TX (连接LD3320 RXD)

// 继电器控制引脚定义
#define RELAY_HUMIDIFIER 5 // 加湿器继电器 (GPIO 5)

// L298N电机驱动模块引脚定义
#define L298N_IN1 1 // 电机A方向控制1 (GPIO 1)
#define L298N_IN2 2 // 电机A方向控制2 (GPIO 2)
#define L298N_IN3 3 // 电机B方向控制1 (GPIO 3)
#define L298N_IN4 4 // 电机B方向控制2 (GPIO 4)

class LD3320_ESP32
{
private:
    HardwareSerial *serialPort;
    String receivedData;
    bool voiceCommandReady;
    unsigned long lastCommandTime;
    static const unsigned long COMMAND_TIMEOUT = 5000; // 5秒超时

    // 解析接收到的语音命令
    void parseVoiceCommand(const String &command);

public:
    LD3320_ESP32();

    // 公共接口函数
    bool begin();
    void update(); // 在主循环中调用，检查串口数据
    bool hasNewCommand();
    String getLastCommand();

    // 语音命令处理
    void handleVoiceCommand(const String &command);

    // 继电器控制
    void setHumidifier(bool state);

    // L298N电机控制
    void setMotorForward(bool state);
    void setMotorBackward(bool state);
    void stopAllMotors();

    // 获取加湿器状态
    bool getHumidifierState();
};

#endif // LD3320_ESP32_H