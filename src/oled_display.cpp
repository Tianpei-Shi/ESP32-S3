#include "oled_display.h"

// 创建OLED对象
Adafruit_SSD1306 display096(SCREEN_WIDTH_096, SCREEN_HEIGHT_096, &Wire, OLED_RESET);
Adafruit_SSD1306 display091(SCREEN_WIDTH_091, SCREEN_HEIGHT_091, &Wire1, OLED_RESET);

// AI圆圈动画参数
int circleRadius = 8;
int circleX = SCREEN_WIDTH_091 - 16;
int circleY = SCREEN_HEIGHT_091 / 2;
bool growing = true;

// 初始化OLED显示屏
bool initOLEDDisplays()
{
    // 初始化两个I2C总线
    Wire.begin(SDA_096_PIN, SCL_096_PIN);
    Wire1.begin(SDA_091_PIN, SCL_091_PIN);

    // 初始化0.96英寸OLED
    if (!display096.begin(SSD1306_SWITCHCAPVCC, OLED_096_ADDR))
    {
        Serial.println(F("0.96 inch OLED initialization failed"));
        return false;
    }
    display096.clearDisplay();
    display096.display();

    // 初始化0.91英寸OLED
    if (!display091.begin(SSD1306_SWITCHCAPVCC, OLED_091_ADDR))
    {
        Serial.println(F("0.91 inch OLED initialization failed"));
        return false;
    }
    display091.clearDisplay();
    display091.display();

    return true;
}

// 显示系统启动信息
void showStartupScreen()
{
    // 0.96英寸OLED启动信息
    display096.clearDisplay();
    display096.setTextSize(2);
    display096.setTextColor(SSD1306_WHITE);
    drawCenteredText(display096, "System", 10);
    display096.setTextSize(2);
    display096.setTextColor(SSD1306_WHITE);
    drawCenteredText(display096, "Starting", 30);
    display096.display();

    // 0.91英寸OLED启动信息
    display091.clearDisplay();
    display091.setTextSize(1);
    display091.setTextColor(SSD1306_WHITE);
    drawCenteredText(display091, "AI Assistant", 10);
    drawCenteredText(display091, "Ready", 20);
    display091.display();

    delay(2000);
}

// 更新0.96英寸OLED显示屏（温度、湿度、PM2.5、水位）
void updateDisplay096(float temperature, float humidity, int pm25Value, int waterLevel, bool waterLevelWarning)
{
    display096.clearDisplay();
    display096.setTextColor(SSD1306_WHITE);

    // 显示温度
    display096.setTextSize(1);
    display096.setCursor(0, 0);
    display096.print("Temp: ");
    display096.setTextSize(2);
    display096.setCursor(0, 10);
    display096.print(temperature, 1);
    display096.print(" C");

    // 显示湿度
    display096.setTextSize(1);
    display096.setCursor(80, 0);
    display096.print("Humid: ");
    display096.setTextSize(2);
    display096.setCursor(80, 10);
    display096.print(humidity, 0);
    display096.print("%");

    // 显示PM2.5
    display096.setTextSize(1);
    display096.setCursor(0, 35);
    display096.print("PM2.5: ");
    display096.setTextSize(2);
    display096.setCursor(0, 45);
    display096.print(pm25Value);
    display096.setTextSize(1);
    display096.print(" ug/m3");

    // 显示水位
    display096.setTextSize(1);
    display096.setCursor(80, 35);
    display096.print("Water: ");
    display096.setTextSize(2);
    display096.setCursor(80, 45);

    // 显示具体水位数值
    display096.print(waterLevel);

    display096.display();
}

// 更新0.91英寸OLED显示屏（AI对话）
void updateDisplay091(String userInput, String aiResponse)
{
    display091.clearDisplay();

    // 绘制AI圆圈（右侧）
    if (growing)
    {
        circleRadius += 1;
        if (circleRadius >= 12)
            growing = false;
    }
    else
    {
        circleRadius -= 1;
        if (circleRadius <= 4)
            growing = true;
    }

    display091.fillCircle(circleX, circleY, circleRadius, SSD1306_WHITE);

    // 显示对话内容（左侧）
    display091.setTextSize(1);
    display091.setTextColor(SSD1306_WHITE);

    // 显示用户输入
    display091.setCursor(0, 0);
    display091.print("User: ");
    display091.setCursor(0, 10);

    // 如果字符串太长，截断显示
    if (userInput.length() > 12)
    {
        display091.print(userInput.substring(0, 12));
    }
    else
    {
        display091.print(userInput);
    }

    // 显示AI回复
    display091.setCursor(0, 20);
    display091.print("AI: ");

    // 如果字符串太长，实现滚动显示
    static int scrollPos = 0;
    static unsigned long lastScrollTime = 0;

    if (aiResponse.length() > 12)
    {
        if (millis() - lastScrollTime > 500)
        {
            scrollPos = (scrollPos + 1) % aiResponse.length();
            lastScrollTime = millis();
        }

        if (scrollPos + 12 <= aiResponse.length())
        {
            display091.print(aiResponse.substring(scrollPos, scrollPos + 12));
        }
        else
        {
            // 处理文本环绕
            display091.print(aiResponse.substring(scrollPos));
            display091.print(aiResponse.substring(0, 12 - (aiResponse.length() - scrollPos)));
        }
    }
    else
    {
        display091.print(aiResponse);
    }

    display091.display();
}

// 绘制居中文本的辅助函数
void drawCenteredText(Adafruit_SSD1306 &display, const char *text, int16_t y)
{
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    display.setCursor((display.width() - w) / 2, y);
    display.print(text);
}