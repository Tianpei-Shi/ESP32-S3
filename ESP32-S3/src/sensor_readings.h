#ifndef SENSOR_READINGS_H
#define SENSOR_READINGS_H

#include <Arduino.h>

// 初始化传感器读取模块
bool initSensorReadings();

// 更新传感器读取
void updateSensorReadings();

// 获取温度
float getTemperature();

// 获取湿度
float getHumidity();

// 获取PM2.5值
int getPM25();

// 获取水位
int getWaterLevel();

#endif // SENSOR_READINGS_H