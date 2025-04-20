# ESP32-S3物联网平台

这是一个基于ESP32-S3的物联网平台项目，集成了多种传感器监控、AI语音助手和华为云IoT平台连接功能。

## 功能特性

- **环境监测**：温度、湿度、PM2.5、水位监测
- **OLED显示**：0.96寸OLED实时显示传感器数据
- **语音识别**：内置麦克风，支持语音指令识别
- **AI交互**：AI语音助手，显示在0.91寸OLED屏幕上
- **华为云IoT连接**：数据上云，远程监控和控制

## 硬件要求

- ESP32-S3开发板
- DHT22温湿度传感器
- PM2.5传感器
- 水位传感器
- 0.96寸OLED显示屏
- 0.91寸OLED显示屏
- 麦克风模块

## 软件依赖

- Arduino framework
- 依赖库：
  - Adafruit SSD1306
  - DHT sensor library
  - WiFi
  - Adafruit NeoPixel
  - ESP32-audioI2S
  - Sharp GP2Y Dust Sensor
  - Adafruit GFX Library
  - Adafruit BusIO
  - PubSubClient
  - ArduinoJson

## 快速开始

1. 使用PlatformIO或Arduino IDE打开项目
2. 修改WiFi配置和华为云IoT平台连接信息
3. 连接相关硬件
4. 编译并上传到ESP32-S3

## 目录结构

- **src/**：源代码文件夹
  - main.cpp：主程序入口
  - huawei_iot.cpp：华为云IoT平台连接模块
  - sensors.cpp：传感器管理模块
  - wifi_manager.cpp：WiFi连接管理
  - oled_display.cpp：OLED显示控制
  - audio.cpp：音频处理
  - ai_interface.cpp：AI服务接口
- **include/**：头文件文件夹
- **platformio.ini**：PlatformIO项目配置文件

## 华为云IoT平台配置

本项目使用华为云IoT平台作为数据管理和设备控制中心。需要在华为云IoT平台创建产品和设备，并配置相应的物模型。

### 物模型配置
  
物模型服务：`environment`
属性：
- temperature：温度，浮点数
- humidity：湿度，整数
- PM2.5：PM2.5值，整数
- water：水位，整数（百分比）

> 注意：GitHub上的代码已将敏感信息替换为占位符。实际使用时需替换为您自己的连接信息。

## 注意事项

- 华为云IoT连接信息（设备ID、密钥等）需要根据您的设备进行替换
- WiFi信息需要替换为您自己的网络信息
- 首次连接华为云IoT平台时，设备需要激活过程

## 许可证

MIT

## 贡献

欢迎提交Issue和Pull Request！

## 开发者

Tianpei-Shi