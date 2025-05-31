#include "sensor_readings.h"
#include "sensors.h"

// 传感器数据变量
static float currentTemperature = 25.0;
static float currentHumidity = 60.0;
static int currentPM25 = 15;
static int currentWaterLevel = 80;

bool initSensorReadings()
{
    Serial.println("初始化传感器读取模块...");
    // 调用真实的传感器初始化函数
    return initSensors();
}

void updateSensorReadings()
{
    // 读取真实的传感器数据
    float temp, hum;
    bool waterWarning;

    // 读取DHT11温湿度数据
    if (readDHTData(temp, hum))
    {
        currentTemperature = temp;
        currentHumidity = hum;
    }

    // 读取PM2.5数据
    currentPM25 = readPM25();

    // 读取水位数据
    currentWaterLevel = readWaterLevel(waterWarning);
}

float getTemperature()
{
    return currentTemperature;
}

float getHumidity()
{
    return currentHumidity;
}

int getPM25()
{
    return currentPM25;
}

int getWaterLevel()
{
    return currentWaterLevel;
}