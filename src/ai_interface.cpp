#include "ai_interface.h"

// AI状态
AIState currentAIState = {
    .userInput = "Hello",
    .aiResponse = "How can I help?",
    .isProcessing = false,
    .lastUpdateTime = 0};

// 初始化AI接口
bool initAIInterface()
{
    currentAIState.lastUpdateTime = millis();
    Serial.println("AI interface initialization complete");
    return true;
}

// 处理用户输入，获取AI响应
bool processUserInput(const String &input, String &response)
{
    // 设置处理状态
    currentAIState.isProcessing = true;
    currentAIState.userInput = input;

    // 这只是模拟AI处理
    // 在实际应用中，应该连接到真正的AI服务或本地模型

    // 简单的预定义回复
    if (input.indexOf("turn on") >= 0 && input.indexOf("humidifier") >= 0)
    {
        response = "Humidifier turned on";
    }
    else if (input.indexOf("turn off") >= 0 && input.indexOf("humidifier") >= 0)
    {
        response = "Humidifier turned off";
    }
    else if (input.indexOf("forward") >= 0 || input.indexOf("go ahead") >= 0)
    {
        response = "Car moving forward";
    }
    else if (input.indexOf("backward") >= 0 || input.indexOf("go back") >= 0)
    {
        response = "Car moving backward";
    }
    else if (input.indexOf("left") >= 0)
    {
        response = "Car turning left";
    }
    else if (input.indexOf("right") >= 0)
    {
        response = "Car turning right";
    }
    else if (input.indexOf("stop") >= 0)
    {
        response = "Car stopped";
    }
    else if (input.indexOf("temperature") >= 0)
    {
        response = "Checking temperature";
    }
    else if (input.indexOf("humidity") >= 0)
    {
        response = "Checking humidity";
    }
    else if (input.indexOf("water") >= 0)
    {
        response = "Checking water level";
    }
    else
    {
        response = "Sorry, I don't understand";
    }

    // 更新AI状态
    currentAIState.aiResponse = response;
    currentAIState.isProcessing = false;
    currentAIState.lastUpdateTime = millis();

    return true;
}

// 获取AI对话状态
AIState getAIState()
{
    return currentAIState;
}

// 更新AI动画状态
void updateAIAnimationState()
{
    // 此函数可在主循环中调用，用于更新动画状态
    // 如果AI处于处理状态，可以更新一些动画参数
    // 例如改变颜色、闪烁频率等
}