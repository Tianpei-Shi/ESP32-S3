开发板:ESP32 S3 N16R8
使用的模块:Max98357 音频，扬声器，立式微动开关，INMP441 麦克风，小车模块，加湿器模块，YW01 液位传感器，PM2.5 传感器，DHT11 温湿度检测，温湿度显示屏（0.96 寸 OLED），AI 对话显示屏（0.91 寸 OLED），状态显示屏（0.96 寸 OLED），RGB LED

针脚分配情况：
Max98357 音频模块 + 扬声器（I2S 接口）
VDD: 3.3V
GND: GND
BCLK: GPIO36 (SPII07)
LRCLK: GPIO35 (SPII06)
DIN: GPIO37 (SPIDOS)
GAIN: 悬空（12dB 增益）
SD/MODE: 3.3V（正常工作模式）

INMP441 麦克风 (I2S 接口)
VDD: 3.3V
GND: GND
SCK: GPIO47 (SPICLK_P)
WS: GPIO14 (GPIO14)
SD: GPIO38 (FSPIWP)
L/R: GND（左声道）

RGB LED
VDD: 5V
GND: GND
数据引脚: GPIO48 (GPIO48)

温湿度显示屏 - 0.96 寸 OLED (I2C 接口, 地址: 0x3C)
VDD: 3.3V
GND: GND
SDA: GPIO15 (GPIO15)
SCL: GPIO16 (GPIO16)

AI 对话显示屏 - 0.91 寸 OLED (I2C 接口, 地址: 0x3C)
VDD: 3.3V
GND: GND
SDA: GPIO9 (GPIO9)
SCL: GPIO10 (GPIO10)

状态显示屏 - 0.96 寸 OLED (I2C 接口, 地址: 0x3C)
VDD: 3.3V
GND: GND
SDA: GPIO11 (GPIO11)
SCL: GPIO12 (GPIO12)

立式微动开关
一端: GPIO4 (GPIO4)
另一端: GND

小车模块
电机 A 正转: GPIO1 (GPIO1)
电机 A 反转: GPIO2 (GPIO2)
电机 B 正转: GPIO3 (GPIO3)
电机 B 反转: GPIO8 (GPIO8)
VCC: 5V/3.3V（根据模块要求）
GND: GND

加湿器模块
控制引脚: GPIO5 (GPIO5)
VCC: 5V
GND: GND

YW01 液位传感器
数据引脚: GPIO6 (GPIO6)
VCC: 3.3V
GND: GND

PM2.5 传感器 (UART 接口)
TX: GPIO17 (U1TXD)
RX: GPIO18 (U1RXD)
VCC: 5V/3.3V（根据模块要求）
GND: GND

DHT11 温湿度传感器
数据引脚: GPIO7 (GPIO7)
VCC: 3.3V
GND: GND

---

需要安装的库：

OLED 显示屏:

- Adafruit SSD1306 (版本 2.5.13 或更高)
- Adafruit GFX Library (会作为依赖自动安装)
- Adafruit BusIO (会作为依赖自动安装)

DHT11 温湿度传感器:

- DHT sensor library (版本 1.4.6 或更高)
- Adafruit Unified Sensor (会作为依赖自动安装)

音频处理 (Max98357 和 INMP441):

- ESP32-audioI2S (https://github.com/schreibfaul1/ESP32-audioI2S)
- ESP32-A2DP (可选，用于蓝牙音频)

RGB LED:

- Adafruit NeoPixel (版本 1.12.5 或更高)

PM2.5 传感器:

- PMS Library (用于常见的 PMS5003 等型号)

WiFi 连接:

- WiFi (ESP32 内置，版本 2.0.0 或更高)

小车控制:

- ESP32 Arduino ESP32Servo (舵机控制)
- ESP32MotorControl (用于直流电机控制)

安装方法:

1. 通过 Arduino IDE: 工具 -> 管理库 -> 搜索上述库名
2. 通过 PlatformIO:
   - 编辑 platformio.ini 文件
   - 在 lib_deps 下添加需要的库
   - 例如: `lib_deps = adafruit/Adafruit SSD1306@^2.5.13`
