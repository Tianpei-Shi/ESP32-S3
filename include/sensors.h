#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <DHT.h>

// 定义传感器引脚
#define DHT_PIN 7
#define PM25_AO_PIN 17
#define PM25_PUL_PIN 18
#define WATER_AO_PIN 6
#define WATER_DO_PIN 11

// 初始化所有传感器
bool initSensors();

// 读取DHT11温湿度数据
bool readDHTData(float &temperature, float &humidity);

// 读取PM2.5数据
int readPM25();

// 读取水位数据
int readWaterLevel(bool &warning);

#endif // SENSORS_H