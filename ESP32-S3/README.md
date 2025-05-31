# ESP32-S3 智能语音控制加湿小车

这是一个基于 ESP32-S3 的物联网平台项目，集成了多种传感器监控、AI 语音助手、小车控制和加湿功能。

## 功能特性

- **环境监测**：温度、湿度、PM2.5、水位监测
- **OLED 显示**：0.96 寸 OLED 实时显示传感器数据
- **语音识别与控制**：集成 LD3320 语音识别模块，支持语音指令识别
- **AI 交互**：AI 语音助手，显示在 0.91 寸 OLED 屏幕上
- **小车控制**：支持通过语音指令控制小车移动
- **加湿功能**：集成加湿器模块，可远程控制
- **华为云 IoT 连接**：数据上云，远程监控和控制

## 硬件要求

- **开发板型号**：ESP32 S3 N16R8
- **使用的模块**：
  - Max98357 音频模块 + 扬声器
  - 绿深 LD3320 语音识别模块
  - L298N 2.5A 双路电机驱动模块
  - 加湿器模块
  - YW01 液位传感器
  - PM2.5 传感器 GP2Y1014AU0F
  - DHT11 温湿度传感器
  - 温湿度显示屏（0.96 寸 OLED）
  - AI 对话显示屏（0.91 寸 OLED）
  - 状态显示屏（0.96 寸 OLED）
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

### 绿深 LD3320 语音识别模块（5 针脚）

| 引脚 | 连接到          | 说明        |
| ---- | --------------- | ----------- |
| VCC  | 3.3V            | 电源正极    |
| GND  | GND             | 电源负极    |
| RXD  | GPIO14 (GPIO14) | 串口接收    |
| TXD  | GPIO38 (FSPIWP) | 串口发送    |
| 5V   | 5V              | 5V 电源输入 |

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

### L298N 2.5A 双路电机驱动模块

| 引脚 | 连接到        | 说明                     |
| ---- | ------------- | ------------------------ |
| IN1  | GPIO1 (GPIO1) | 电机 A 方向控制 1        |
| IN2  | GPIO2 (GPIO2) | 电机 A 方向控制 2        |
| IN3  | GPIO3 (GPIO3) | 电机 B 方向控制 1        |
| IN4  | GPIO4 (GPIO4) | 电机 B 方向控制 2        |
| VCC  | 5V            | 逻辑电源（5V）           |
| GND  | GND           | 公共接地                 |
| +12V | 7-35V         | 电机电源（根据电机要求） |
| GND  | GND           | 电源接地                 |
| OUT1 | 电机 A+       | 电机 A 正极输出          |
| OUT2 | 电机 A-       | 电机 A 负极输出          |
| OUT3 | 电机 B+       | 电机 B 正极输出          |
| OUT4 | 电机 B-       | 电机 B 负极输出          |

**注意事项：**

- L298N 模块支持 7-35V 电机电源，请根据您的电机规格选择合适的电源电压
- VCC 接 5V 为逻辑控制电源，与电机电源分离
- 两个 GND 都需要连接到 ESP32 的 GND，确保共地
- 电机控制逻辑（双电机协调控制）：
  - 前进：IN1=HIGH, IN2=LOW, IN3=HIGH, IN4=LOW (双电机正转)
  - 后退：IN1=LOW, IN2=HIGH, IN3=LOW, IN4=HIGH (双电机反转)
  - 停止：所有 IN 引脚都设为 LOW
- 如需要 PWM 速度控制，可在 IN 引脚上使用 analogWrite() 函数

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

- LD3320 语音识别模块库 (https://github.com/BringBackSpace/LD3320)

### WiFi 连接

- WiFi (ESP32 内置，版本 2.0.0 或更高)

### 小车控制

- ESP32 Arduino ESP32Servo (舵机控制，如果有需要)
- ESP32MotorControl (用于直流电机控制)
- TB6612FNG 库 (可选，或直接使用 Arduino 的 digitalWrite 和 analogWrite 函数)

### PM2.5 传感器

- Sharp GP2Y Dust Sensor 库（适用于 GP2Y1014AU0F 传感器）
- 或使用 Arduino 标准库的 analogRead 和 pulseIn 函数

### 其他

- Adafruit NeoPixel (RGB LED 控制)
- PubSubClient (MQTT 通信)
- ArduinoJson (JSON 数据处理)

## 安装库方法

1. 通过 Arduino IDE:

   - 工具 -> 管理库 -> 搜索上述库名
   - 选择合适的版本安装

2. 通过 PlatformIO:
   - 编辑 platformio.ini 文件
   - 在 lib_deps 下添加需要的库
   - 例如: `lib_deps = adafruit/Adafruit SSD1306@^2.5.13`

## 快速开始

1. 使用 PlatformIO 或 Arduino IDE 打开项目
2. 修改 WiFi 配置和华为云 IoT 平台连接信息
3. 按照针脚分配连接相关硬件
4. 编译并上传到 ESP32-S3
5. 加水到水箱并通电启动系统

## 目录结构

- **src/**：源代码文件夹
  - main.cpp：主程序入口
  - huawei_iot.cpp：华为云 IoT 平台连接模块
  - sensors.cpp：传感器管理模块
  - wifi_manager.cpp：WiFi 连接管理
  - oled_display.cpp：OLED 显示控制
  - audio.cpp：音频处理
  - ai_interface.cpp：AI 服务接口
  - motor_control.cpp：小车电机控制
  - humidifier.cpp：加湿器控制
  - voice_recognition.cpp：语音识别处理
- **include/**：头文件文件夹
- **platformio.ini**：PlatformIO 项目配置文件

## 华为云 IoT 平台配置

本项目使用华为云 IoT 平台作为数据管理和设备控制中心。需要在华为云 IoT 平台创建产品和设备，并配置相应的物模型。

### 物模型配置

物模型服务：`environment`
属性：

- temperature：温度，浮点数
- humidity：湿度，整数
- PM2.5：PM2.5 值，整数
- water：水位，整数（百分比）

> 注意：GitHub 上的代码已将敏感信息替换为占位符。实际使用时需替换为您自己的连接信息。

## 注意事项

- 华为云 IoT 连接信息（设备 ID、密钥等）需要根据您的设备进行替换
- WiFi 信息需要替换为您自己的网络信息
- 首次连接华为云 IoT 平台时，设备需要激活过程
- 使用前确保水箱中有足够的水，避免加湿器干烧
- 保持针脚连接正确，避免短路

## 许可证

MIT

## 贡献

欢迎提交 Issue 和 Pull Request！

## 开发者

Tianpei-Shi

## LD3320 语音识别模块集成

项目已成功集成 LD3320 语音识别模块（5 针脚版本）。该模块内置了完整的语音识别固件，ESP32 通过串口接收识别结果。

### 硬件连接

LD3320 模块与 ESP32 的连接：

- VCC → 3.3V
- GND → GND
- TXD → GPIO 14 (ESP32 RX)
- RXD → GPIO 38 (ESP32 TX)

### 继电器连接

| 功能         | ESP32 引脚 | 说明           |
| ------------ | ---------- | -------------- |
| 加湿器继电器 | GPIO 5     | 控制加湿器开关 |
| 前进电机     | GPIO 33    | 控制小车前进   |
| 后退电机     | GPIO 25    | 控制小车后退   |

### 工作原理

1. **语音识别模块**: LD3320 模块已预烧录语音识别算法，能识别以下语音命令：

   - "小一" - 唤醒词
   - "加湿" - 开启加湿器
   - "关闭加湿" - 关闭加湿器
   - "前进" - 小车前进
   - "后退" - 小车后退

2. **通信方式**: LD3320 通过串口发送识别结果给 ESP32：

   - 波特率: 9600
   - 数据格式: 8N1
   - ESP32 接收字符串命令并执行相应操作

3. **默认状态**:
   - 加湿器默认关闭（GPIO 5 = LOW）
   - 所有电机默认停止

### 使用方法

1. **语音控制流程**:

   ```
   用户说话 -> LD3320识别 -> 串口发送结果 -> ESP32接收 -> 执行动作
   ```

2. **语音命令示例**:
   - 先说"小一"唤醒系统
   - 然后说"加湿"开启加湿器 (GPIO 5 输出 HIGH)
   - 说"关闭加湿"关闭加湿器 (GPIO 5 输出 LOW)

### 代码结构

- `lib/LD3320_ESP32/`: LD3320 驱动库
- `lib/voice_model/`: LD3320 模块内部固件代码（已烧录到模块）
- `src/voice_recognition.h/cpp`: 语音识别接口
- `src/main.cpp`: 主程序，调用语音识别更新函数

### 注意事项

- LD3320 模块的`lib/voice_model`目录包含的是已烧录到模块内部的固件代码
- ESP32 只需通过串口接收模块发送的识别结果，无需重新烧录 LD3320
- 建议语音命令间隔 3 秒以上，避免连续触发
- 加湿器通过 GPIO 5 控制，符合用户需求
