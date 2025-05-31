#include <Arduino.h>
#include "LD3320_ESP32.h"

// 创建LD3320实例
LD3320_ESP32 ld3320;

void setup()
{
    Serial.begin(115200);
    Serial.println("=== 加湿器控制测试程序 ===");

    // 初始化LD3320模块
    if (!ld3320.begin())
    {
        Serial.println("LD3320初始化失败!");
        while (1)
            delay(1000);
    }

    Serial.println("LD3320初始化成功!");
    Serial.println("语音控制流程:");
    Serial.println("1. 说'小一'唤醒语音控制");
    Serial.println("2. 说'加湿'开启加湿器 (GPIO 5 输出HIGH)");
    Serial.println("3. 说'关闭加湿'关闭加湿器 (GPIO 5 输出LOW)");
    Serial.println("");

    // 验证加湿器默认关闭
    Serial.println("验证加湿器默认状态: 关闭 (GPIO 5 = LOW)");
    Serial.print("GPIO 5 状态: ");
    Serial.println(digitalRead(5) ? "HIGH" : "LOW");
    Serial.println("");
}

void loop()
{
    Serial.println("开始语音识别...");

    // 运行语音识别
    if (ld3320.runASR())
    {
        delay(3000); // 等待语音识别结果

        // 获取识别结果
        uint8_t result = ld3320.getResult();

        Serial.print("识别结果代码: 0x");
        Serial.println(result, HEX);

        // 处理语音命令
        switch (result)
        {
        case 0x00: // 唤醒词 "小一"
            Serial.println("✓ 收到唤醒词: 小一");
            Serial.println("语音控制已激活，可以说控制命令了");
            break;

        case 0x01: // "加湿"
            Serial.println("✓ 收到命令: 加湿");
            ld3320.setHumidifier(true);
            Serial.println("加湿器已开启 (GPIO 5 = HIGH)");
            Serial.print("GPIO 5 当前状态: ");
            Serial.println(digitalRead(5) ? "HIGH" : "LOW");
            break;

        case 0x02: // "关闭加湿"
            Serial.println("✓ 收到命令: 关闭加湿");
            ld3320.setHumidifier(false);
            Serial.println("加湿器已关闭 (GPIO 5 = LOW)");
            Serial.print("GPIO 5 当前状态: ");
            Serial.println(digitalRead(5) ? "HIGH" : "LOW");
            break;

        case 0x03: // "前进"
            Serial.println("✓ 收到命令: 前进");
            ld3320.handleVoiceCommand(result);
            break;

        case 0x04: // "后退"
            Serial.println("✓ 收到命令: 后退");
            ld3320.handleVoiceCommand(result);
            break;

        default:
            Serial.println("✗ 未识别到有效命令");
            break;
        }
    }
    else
    {
        Serial.println("✗ 语音识别启动失败");
    }

    Serial.println("==================");
    delay(5000); // 等待5秒再次识别
}