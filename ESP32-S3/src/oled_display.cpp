#include "oled_display.h"
#include <Wire.h>
#include <WiFi.h>

// 创建U8g2 OLED对象 - 使用不同的I2C地址
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2_096(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2_091(U8G2_R0, /* clock=*/SCL_091_PIN, /* data=*/SDA_091_PIN, /* reset=*/U8X8_PIN_NONE);

// 动画参数
static unsigned long lastAnimationTime = 0;
static int animationFrame = 0;
static bool display096_initialized = false;
static bool display091_initialized = false;

// 初始化OLED显示屏
bool initOLEDDisplays()
{
    // 初始化I2C总线 - 仅初始化主总线
    Wire.begin(SDA_096_PIN, SCL_096_PIN);

    // 初始化0.96英寸OLED (使用硬件I2C)
    u8g2_096.setBusClock(100000);     // 降低I2C时钟频率以提高稳定性
    u8g2_096.setI2CAddress(0x3C * 2); // 设置I2C地址
    if (u8g2_096.begin())
    {
        display096_initialized = true;
        Serial.println("0.96寸OLED初始化成功");
    }
    else
    {
        Serial.println("0.96寸OLED初始化失败");
        display096_initialized = false;
    }

    // 初始化0.91英寸OLED (使用软件I2C)
    u8g2_091.setI2CAddress(0x3C * 2); // 设置I2C地址
    if (u8g2_091.begin())
    {
        display091_initialized = true;
        Serial.println("0.91寸OLED初始化成功");
    }
    else
    {
        Serial.println("0.91寸OLED初始化失败");
        display091_initialized = false;
    }

    return display096_initialized || display091_initialized;
}

// 显示系统启动信息
void showStartupScreen()
{
    // 0.96英寸OLED启动信息
    if (display096_initialized)
    {
        u8g2_096.clearBuffer();
        u8g2_096.setFont(u8g2_font_ncenB14_tr);
        drawCenteredText096("System", 30);
        drawCenteredText096("Starting", 50);
        u8g2_096.sendBuffer();
    }

    // 0.91英寸OLED启动信息
    if (display091_initialized)
    {
        u8g2_091.clearBuffer();
        u8g2_091.setFont(u8g2_font_ncenB10_tr);
        drawCenteredText091("Ready", 20);
        u8g2_091.sendBuffer();
    }

    delay(2000);
}

// 更新0.96英寸OLED显示屏（温度、湿度、PM2.5、水位）
void updateDisplay096(float temperature, float humidity, int pm25Value, int waterLevel, bool waterLevelWarning)
{
    if (!display096_initialized)
        return;

    u8g2_096.clearBuffer();
    u8g2_096.setFont(u8g2_font_6x10_tf);

    // 显示温度
    u8g2_096.setCursor(0, 10);
    u8g2_096.print("Temp:");
    u8g2_096.setCursor(0, 25);
    u8g2_096.setFont(u8g2_font_10x20_tf);
    u8g2_096.print(temperature, 1);
    u8g2_096.print("C");

    // 显示湿度
    u8g2_096.setFont(u8g2_font_6x10_tf);
    u8g2_096.setCursor(70, 10);
    u8g2_096.print("Humid:");
    u8g2_096.setCursor(70, 25);
    u8g2_096.setFont(u8g2_font_10x20_tf);
    u8g2_096.print(humidity, 0);
    u8g2_096.print("%");

    // 显示PM2.5
    u8g2_096.setFont(u8g2_font_6x10_tf);
    u8g2_096.setCursor(0, 40);
    u8g2_096.print("PM2.5:");
    u8g2_096.setCursor(0, 55);
    u8g2_096.setFont(u8g2_font_10x20_tf);
    u8g2_096.print(pm25Value);

    // 显示水位
    u8g2_096.setFont(u8g2_font_6x10_tf);
    u8g2_096.setCursor(70, 40);
    u8g2_096.print("Water:");
    u8g2_096.setCursor(70, 55);
    u8g2_096.setFont(u8g2_font_10x20_tf);
    u8g2_096.print(waterLevel);

    u8g2_096.sendBuffer();
}

// 显示0.91寸OLED待机状态
void showDisplay091Standby()
{
    if (!display091_initialized)
        return;

    u8g2_091.clearBuffer();

    // 左上角显示WiFi图标状态
    u8g2_091.setFont(u8g2_font_6x10_tf);
    u8g2_091.setCursor(2, 8);
    if (WiFi.status() == WL_CONNECTED)
    {
        u8g2_091.print("WiFi");
    }
    else
    {
        u8g2_091.print("NoWF");
    }

    // 中间显示"待命"状态
    u8g2_091.setFont(u8g2_font_ncenB08_tr);
    u8g2_091.setCursor(28, 16);
    u8g2_091.print("Standby");

    // 添加简单的点点动画
    if (millis() - lastAnimationTime > 800)
    {
        animationFrame = (animationFrame + 1) % 4;
        lastAnimationTime = millis();
    }

    u8g2_091.setFont(u8g2_font_6x10_tf);
    u8g2_091.setCursor(28, 26);
    for (int i = 0; i < animationFrame; i++)
    {
        u8g2_091.print(".");
    }

    // 右边显示笑脸头像
    drawSmileyFace(110, 16, false);

    u8g2_091.sendBuffer();
}

// 显示0.91寸OLED聆听状态
void showDisplay091Listening()
{
    if (!display091_initialized)
        return;

    u8g2_091.clearBuffer();

    // 左上角显示WiFi图标状态
    u8g2_091.setFont(u8g2_font_6x10_tf);
    u8g2_091.setCursor(2, 8);
    if (WiFi.status() == WL_CONNECTED)
    {
        u8g2_091.print("WiFi");
    }
    else
    {
        u8g2_091.print("NoWF");
    }

    // 中间显示"聆听"状态
    u8g2_091.setFont(u8g2_font_ncenB08_tr);
    u8g2_091.setCursor(20, 16);
    u8g2_091.print("Listening");

    // 添加呼吸动画效果
    if (millis() - lastAnimationTime > 300)
    {
        animationFrame = (animationFrame + 1) % 8;
        lastAnimationTime = millis();
    }

    // 在状态文字下方显示动态指示条
    u8g2_091.setFont(u8g2_font_6x10_tf);
    u8g2_091.setCursor(20, 26);
    for (int i = 0; i < animationFrame; i++)
    {
        u8g2_091.print("-");
    }

    // 右边显示激活状态的笑脸头像
    drawSmileyFace(110, 16, true);

    u8g2_091.sendBuffer();
}

// 绘制笑脸头像函数
void drawSmileyFace(int x, int y, bool active)
{
    // 绘制脸部轮廓
    u8g2_091.drawCircle(x, y, 8);

    // 绘制眼睛
    u8g2_091.drawCircle(x - 3, y - 2, 1);
    u8g2_091.drawCircle(x + 3, y - 2, 1);

    if (active)
    {
        // 激活状态：填充眼睛
        u8g2_091.drawDisc(x - 3, y - 2, 1);
        u8g2_091.drawDisc(x + 3, y - 2, 1);

        // 绘制微笑的嘴巴
        for (int i = -3; i <= 3; i++)
        {
            int mouth_y = y + 2 + (i * i) / 4;
            u8g2_091.drawPixel(x + i, mouth_y);
        }
    }
    else
    {
        // 待机状态：空心眼睛和平静的嘴巴
        u8g2_091.drawHLine(x - 2, y + 3, 4);
    }
}

// 绘制居中文本的辅助函数 - 0.96寸屏幕
void drawCenteredText096(const char *text, int16_t y)
{
    int textWidth = u8g2_096.getStrWidth(text);
    int x = (SCREEN_WIDTH_096 - textWidth) / 2;
    u8g2_096.setCursor(x, y);
    u8g2_096.print(text);
}

// 绘制居中文本的辅助函数 - 0.91寸屏幕
void drawCenteredText091(const char *text, int16_t y)
{
    int textWidth = u8g2_091.getStrWidth(text);
    int x = (SCREEN_WIDTH_091 - textWidth) / 2;
    u8g2_091.setCursor(x, y);
    u8g2_091.print(text);
}