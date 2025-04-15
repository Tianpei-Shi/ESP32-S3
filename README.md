# 基于 arduino 开发的智能语音控制加湿小车

## 硬件配置

**开发板型号**: ESP32 S3 N16R8

## 使用的模块

- Max98357 音频 + 扬声器
- 立式微动开关
- INMP441 麦克风
- 小车模块
- 加湿器模块
- YW01 液位传感器
- PM2.5 传感器 gp2y1014au0f
- DHT11 温湿度检测
- 温湿度显示屏（0.96 寸 OLED）
- AI 对话显示屏（0.91 寸 OLED）
- 状态显示屏（0.96 寸 OLED）
- RGB LED

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

### INMP441 麦克风 (I2S 接口)

| 引脚 | 连接到            | 说明     |
| ---- | ----------------- | -------- |
| VDD  | 3.3V              | 电源正极 |
| GND  | GND               | 电源负极 |
| SCK  | GPIO47 (SPICLK_P) | 时钟     |
| WS   | GPIO14 (GPIO14)   | 字选择   |
| SD   | GPIO38 (FSPIWP)   | 数据     |
| L/R  | GND               | 左声道   |

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

### 小车模块

| 引脚        | 连接到        | 说明                     |
| ----------- | ------------- | ------------------------ |
| 电机 A 正转 | GPIO1 (GPIO1) | 控制电机 A 正向旋转      |
| 电机 A 反转 | GPIO2 (GPIO2) | 控制电机 A 反向旋转      |
| 电机 B 正转 | GPIO3 (GPIO3) | 控制电机 B 正向旋转      |
| 电机 B 反转 | GPIO8 (GPIO8) | 控制电机 B 反向旋转      |
| VCC         | 5V/3.3V       | 电源正极（根据模块要求） |
| GND         | GND           | 电源负极                 |

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

## 需要安装的库

### OLED 显示屏

- Adafruit SSD1306 (版本 2.5.13 或更高)
- Adafruit GFX Library (会作为依赖自动安装)
- Adafruit BusIO (会作为依赖自动安装)

### DHT11 温湿度传感器

- DHT sensor library (版本 1.4.6 或更高)
- Adafruit Unified Sensor (会作为依赖自动安装)

### 音频处理 (Max98357 和 INMP441)

- ESP32-audioI2S (https://github.com/schreibfaul1/ESP32-audioI2S)
- ESP32-A2DP (可选，用于蓝牙音频)

### WiFi 连接

- WiFi (ESP32 内置，版本 2.0.0 或更高)

### 小车控制

- ESP32 Arduino ESP32Servo (舵机控制)
- ESP32MotorControl (用于直流电机控制)

### PM2.5 传感器

- Sharp GP2Y Dust Sensor 库（适用于 GP2Y1014AU0F 传感器）
- 或使用 Arduino 标准库的 analogRead 和 pulseIn 函数

## 安装方法

1. 通过 Arduino IDE: 工具 -> 管理库 -> 搜索上述库名
2. 通过 PlatformIO:
   - 编辑 platformio.ini 文件
   - 在 lib_deps 下添加需要的库
   - 例如: `lib_deps = adafruit/Adafruit SSD1306@^2.5.13`
