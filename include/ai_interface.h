#ifndef AI_INTERFACE_H
#define AI_INTERFACE_H

#include <Arduino.h>

// AI对话状态
struct AIState
{
    String userInput;
    String aiResponse;
    bool isProcessing;
    unsigned long lastUpdateTime;
};

// 初始化AI接口
bool initAIInterface();

// 处理用户输入，获取AI响应
bool processUserInput(const String &input, String &response);

// 获取AI对话状态
AIState getAIState();

// 更新AI动画状态
void updateAIAnimationState();

#endif // AI_INTERFACE_H