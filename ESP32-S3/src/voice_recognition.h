#ifndef VOICE_RECOGNITION_H
#define VOICE_RECOGNITION_H

#include <Arduino.h>

// 语音识别状态结构体
struct VoiceRecognitionStatus
{
    bool enabled;                      // 语音识别是否启用
    bool commandModeActive;            // 命令模式是否激活（收到唤醒词后）
    unsigned long lastRecognitionTime; // 上次识别时间
};

// 语音识别初始化
bool initVoiceRecognition();

// 更新语音识别（在主循环中调用）
void updateVoiceRecognition();

// 获取加湿器状态
bool getHumidifierStatus();

// 控制加湿器
void setHumidifierState(bool state);

// 控制电机
void setMotorForward(bool state);
void setMotorBackward(bool state);
void stopAllMotors();

// 执行语音识别并返回识别的文本
String captureAndRecognizeVoice();

// 处理语音命令
void processVoiceCommand(const String &command);

// 获取语音识别状态
bool isVoiceRecognitionEnabled();

// 设置语音识别启用状态
void setVoiceRecognitionEnabled(bool enabled);

// 获取详细的语音识别状态
VoiceRecognitionStatus getVoiceRecognitionStatus();

// 兼容原有接口的函数
String captureAndRecognizeSpeech();

#endif // VOICE_RECOGNITION_H