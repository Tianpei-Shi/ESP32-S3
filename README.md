# ESP32-S3智能语音控制加湿小车

这是一个基于ESP32-S3的物联网平台项目，集成了多种传感器监控、AI语音助手、小车控制和加湿功能。

## 功能特性

- **环境监测**：温度、湿度、PM2.5、水位监测
- **OLED显示**：0.96寸OLED实时显示传感器数据
- **语音识别与控制**：集成LD3320语音识别模块，支持语音指令识别
- **AI交互**：AI语音助手，显示在0.91寸OLED屏幕上
- **小车控制**：支持通过语音指令控制小车移动
- **加湿功能**：集成加湿器模块，可远程控制
- **华为云IoT连接**：数据上云，远程监控和控制

## 硬件要求

- **开发板型号**：ESP32 S3 N16R8
- **使用的模块**：
  - Max98357 音频模块 + 扬声器
  - 绿深LD3320语音识别模块
  - TB6612FNG电机驱动模块
  - 加湿器模块
  - YW01 液位传感器
  - PM2.5 传感器 GP2Y1014AU0F
  - DHT11 温湿度传感器
  - 温湿度显示屏（0.96寸OLED）
  - AI对话显示屏（0.91寸OLED）
  - 状态显示屏（0.96寸OLED）
  - RGB LED
  - 立式微动开关

## 针脚分配情况

### Max98357 音频模块 + 扬声器（I2S 接口）

| 引脚    | 连接到          | 说明         |
| ------- | --------------- | ------------ |
| VDD     | 3.3V            | 电源正极     |
| GND     | GND             | 电源负极     |
| BCLK    | GPIO36 (SPII07) | 位时钟       |
| LRCLK   | GPIO35 (SPII06) | 左右声道时钟 |
| DIN     | GPIO37 (SPIDOS) | 数据输入     |
| GAIN    | 悬空            | 12dB 增益    |
| SD/MODE | 3.3V            | 正常工作模式 |

### 绿深LD3320语音识别模块（5针脚）

| 引脚 | 连接到          | 说明         |
| ---- | --------------- | ------------ |
| VCC  | 3.3V            | 电源正极     |
| GND  | GND             | 电源负极     |
| RXD  | GPIO14 (GPIO14) | 串口接收     |
| TXD  | GPIO38 (FSPIWP) | 串口发送     |
| 5V   | 5V              | 5V电源输入   |

### 温湿度显示屏 - 0.96 寸 OLED (I2C 接口, 地址: 0x3C)

| 引脚 | 连接到          | 说明       |
| ---- | --------------- | ---------- |
| VDD  | 3.3V            | 电源正极   |
| GND  | GND             | 电源负极   |
| SDA  | GPIO15 (GPIO15) | I2C 数据线 |
| SCL  | GPIO16 (GPIO16) | I2C 时钟线 |

### AI 对话显示屏 - 0.91 寸 OLED (I2C 接口, 地址: 0x3C)

| 引脚 | 连接到          | 说明       |
| ---- | --------------- | ---------- |
| VDD  | 3.3V            | 电源正极   |
| GND  | GND             | 电源负极   |
| SDA  | GPIO9 (GPIO9)   | I2C 数据线 |
| SCL  | GPIO10 (GPIO10) | I2C 时钟线 |

### TB6612FNG电机驱动模块

| 引脚 | 连接到          | 说明                       |
| ---- | --------------- | -------------------------- |
| STBY | 3.3V            | 待机控制（高电平激活）     |
| AIN1 | GPIO1 (GPIO1)   | 电机A方向控制1             |
| AIN2 | GPIO2 (GPIO2)   | 电机A方向控制2             |
| PWMA | GPIO3 (GPIO3)   | 电机A PWM速度控制          |
| BIN1 | GPIO8 (GPIO8)   | 电机B方向控制1             |
| BIN2 | GPIO42 (GPIO42) | 电机B方向控制2             |
| PWMB | GPIO4 (GPIO4)   | 电机B PWM速度控制          |
| VM   | 5-12V           | 电机电源（根据电机要求）   |
| VCC  | 3.3V            | 逻辑电源                   |
| GND  | GND             | 接地                       |
| A01  | 电机A+          | 电机A正极连接              |
| A02  | 电机A-          | 电机A负极连接              |
| B01  | 电机B+          | 电机B正极连接              |
| B02  | 电机B-          | 电机B负极连接              |

### 加湿器模块

| 引脚     | 连接到        | 说明           |
| -------- | ------------- | -------------- |
| 控制引脚 | GPIO5 (GPIO5) | 控制加湿器开关 |
| VCC      | 5V            | 电源正极       |
| GND      | GND           | 电源负极       |

### YW01 液位传感器

| 引脚 | 连接到          | 说明     |
| ---- | --------------- | -------- |
| AO   | GPIO6 (GPIO6)   | 模拟输出 |
| DO   | GPIO11 (GPIO11) | 数字输出 |
| VCC  | 3.3V            | 电源正极 |
| GND  | GND             | 电源负极 |

### PM2.5 传感器 (GP2Y1014AU0F)

| 引脚 | 连接到         | 说明                     |
| ---- | -------------- | ------------------------ |
| PUL  | GPIO18 (U1TXD) | 脉冲输出                 |
| AO   | GPIO17 (U1RXD) | 模拟输出                 |
| VCC  | 5V/3.3V        | 电源正极（根据模块要求） |
| GND  | GND            | 电源负极                 |

### DHT11 温湿度传感器

| 引脚     | 连接到        | 说明       |
| -------- | ------------- | ---------- |
| 数据引脚 | GPIO7 (GPIO7) | 温湿度数据 |
| VCC      | 3.3V          | 电源正极   |
| GND      | GND           | 电源负极   |

## 软件依赖

- Arduino framework
- 依赖库：

### OLED 显示屏

- Adafruit SSD1306 (版本 2.5.13 或更高)
- Adafruit GFX Library (会作为依赖自动安装)
- Adafruit BusIO (会作为依赖自动安装)

### DHT11 温湿度传感器

- DHT sensor library (版本 1.4.6 或更高)
- Adafruit Unified Sensor (会作为依赖自动安装)

### 音频处理 (Max98357)

- ESP32-audioI2S (https://github.com/schreibfaul1/ESP32-audioI2S)
- ESP32-A2DP (可选，用于蓝牙音频)

### 语音识别 (LD3320)

- LD3320语音识别模块库 (https://github.com/BringBackSpace/LD3320)

### WiFi 连接

- WiFi (ESP32 内置，版本 2.0.0 或更高)

### 小车控制

- ESP32 Arduino ESP32Servo (舵机控制，如果有需要)
- ESP32MotorControl (用于直流电机控制)
- TB6612FNG库 (可选，或直接使用Arduino的digitalWrite和analogWrite函数)

### PM2.5 传感器

- Sharp GP2Y Dust Sensor 库（适用于 GP2Y1014AU0F 传感器）
- 或使用 Arduino 标准库的 analogRead 和 pulseIn 函数

### 其他

- Adafruit NeoPixel (RGB LED控制)
- PubSubClient (MQTT通信)
- ArduinoJson (JSON数据处理)

## 安装库方法

1. 通过 Arduino IDE: 
   - 工具 -> 管理库 -> 搜索上述库名
   - 选择合适的版本安装

2. 通过 PlatformIO:
   - 编辑 platformio.ini 文件
   - 在 lib_deps 下添加需要的库
   - 例如: `lib_deps = adafruit/Adafruit SSD1306@^2.5.13`

## 快速开始

1. 使用PlatformIO或Arduino IDE打开项目
2. 修改WiFi配置和华为云IoT平台连接信息
3. 按照针脚分配连接相关硬件
4. 编译并上传到ESP32-S3
5. 加水到水箱并通电启动系统

## 目录结构

- **src/**：源代码文件夹
  - main.cpp：主程序入口
  - huawei_iot.cpp：华为云IoT平台连接模块
  - sensors.cpp：传感器管理模块
  - wifi_manager.cpp：WiFi连接管理
  - oled_display.cpp：OLED显示控制
  - audio.cpp：音频处理
  - ai_interface.cpp：AI服务接口
  - motor_control.cpp：小车电机控制
  - humidifier.cpp：加湿器控制
  - voice_recognition.cpp：语音识别处理
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
- 使用前确保水箱中有足够的水，避免加湿器干烧
- 保持针脚连接正确，避免短路

## 许可证

MIT

## 贡献

欢迎提交Issue和Pull Request！

## 开发者

Tianpei-Shi
