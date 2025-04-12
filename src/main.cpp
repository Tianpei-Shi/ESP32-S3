#include <Arduino.h>
// DHT11温湿度传感器
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
// 0.96寸显示屏
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// WiFi
#include <WiFi.h>
// RGB灯
#include <Adafruit_NeoPixel.h>

// 引脚定义
#define DHTPIN 7      // DHT11连接到GPIO7
#define DHTTYPE DHT11 // DHT类型为DHT11
#define I2C_SDA 15    // OLED SDA连接到GPIO15
#define I2C_SCL 16    // OLED SCL连接到GPIO16
#define RGB_PIN 48    // RGB LED连接到GPIO48
#define NUM_LEDS 1    // LED数量为1个

// OLED显示屏设置
#define SCREEN_WIDTH 128    // OLED显示屏宽度，单位：像素
#define SCREEN_HEIGHT 64    // 0.96寸OLED的高度是64像素
#define OLED_RESET -1       // Reset pin
#define SCREEN_ADDRESS 0x3C // I2C地址，通常是0x3C

// 创建对象
DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// 创建NeoPixel对象
Adafruit_NeoPixel pixels(NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);

// 定义全局变量用于存储上次读取的值
float lastTemp = 0;
float lastHumi = 0;

// 绘制居中的文本
void drawCenteredText(const char *text, int16_t y)
{
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, y);
  display.print(text);
}

void setup()
{
  Serial.begin(115200); // 初始化串口通信

  // 初始化RGB LED
  pixels.begin();
  pixels.clear();                                 // 设置所有像素为'关闭'状态
  pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // 设置第一个LED为黑色（关闭）
  pixels.show();                                  // 更新LED状态
  Serial.println("RGB LED已关闭");

  // 配置I2C引脚
  Wire.begin(I2C_SDA, I2C_SCL);

  // 初始化DHT11
  dht.begin();
  Serial.println("DHT11初始化完成");

  // 初始化OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306初始化失败"));
    for (;;)
      ; // 如果初始化失败则不再继续
  }
  Serial.println("OLED初始化完成");

  // 显示启动信息
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  drawCenteredText("System", 10);
  drawCenteredText("Starting", 30);
  display.display();
  delay(2000);
}

void loop()
{

  // 读取温湿度数据
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // 检查是否读取失败
  if (isnan(humidity) || isnan(temperature))
  {
    Serial.println(F("DHT11读取失败!"));
    // 如果读取失败，使用上次的有效值
    humidity = lastHumi;
    temperature = lastTemp;
  }
  else
  {
    // 更新最后的有效值
    lastTemp = temperature;
    lastHumi = humidity;

    // 在串口打印数据
    Serial.print(F("温度: "));
    Serial.print(temperature);
    Serial.print(F("°C, 湿度: "));
    Serial.print(humidity);
    Serial.println(F("%"));
  }

  // 清除显示缓冲
  display.clearDisplay();

  // 显示标题
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  drawCenteredText("Temperature & Humidity", 0);

  // 显示分隔线
  display.drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);

  // 显示温度（大字体）
  display.setTextSize(2);
  char tempStr[16];
  sprintf(tempStr, "%.1fC", temperature);
  drawCenteredText(tempStr, 16);

  // 显示湿度（大字体）
  char humiStr[16];
  sprintf(humiStr, "%.1f%%", humidity);
  drawCenteredText(humiStr, 40);

  // 更新显示
  display.display();

  // 延时2秒
  delay(2000);
}

// put function definitions here:
