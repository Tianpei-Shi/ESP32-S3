#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED显示屏设置
#define SCREEN_WIDTH_096 128
#define SCREEN_HEIGHT_096 64
#define SCREEN_WIDTH_091 128
#define SCREEN_HEIGHT_091 32
#define OLED_RESET -1

// I2C地址
#define OLED_096_ADDR 0x3C
#define OLED_091_ADDR 0x3C

// I2C引脚
#define SDA_096_PIN 15
#define SCL_096_PIN 16
#define SDA_091_PIN 9
#define SCL_091_PIN 10

// 初始化OLED显示屏
bool initOLEDDisplays();

// 更新0.96寸OLED显示屏(温湿度、PM2.5、水位)
void updateDisplay096(float temperature, float humidity, int pm25Value, int waterLevel, bool waterLevelWarning);

// 更新0.91寸OLED显示屏(AI对话)
void updateDisplay091(String userInput, String aiResponse);

// 显示系统启动信息
void showStartupScreen();

// 绘制居中文本的辅助函数
void drawCenteredText(Adafruit_SSD1306 &display, const char *text, int16_t y);

#endif // OLED_DISPLAY_H