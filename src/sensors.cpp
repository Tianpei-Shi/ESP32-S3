#include "sensors.h"

// 创建DHT对象
DHT dht(DHT_PIN, DHT11);

// 记录上次有效的传感器数据
float lastValidTemperature = 0;
float lastValidHumidity = 0;

// 初始化所有传感器
bool initSensors()
{
    // 初始化DHT11
    dht.begin();
    Serial.println(F("DHT11 initialization complete"));

    // 初始化PM2.5传感器引脚
    pinMode(PM25_PUL_PIN, OUTPUT);
    pinMode(PM25_AO_PIN, INPUT);
    digitalWrite(PM25_PUL_PIN, HIGH); // 默认高电平
    Serial.println(F("PM2.5 sensor initialization complete"));

    // 初始化水位传感器引脚
    pinMode(WATER_AO_PIN, INPUT);
    pinMode(WATER_DO_PIN, INPUT);
    Serial.println(F("Water level sensor initialization complete"));

    return true;
}

// 读取DHT11温湿度数据
bool readDHTData(float &temperature, float &humidity)
{
    // 读取温度和湿度数据
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    // 检查是否读取失败
    if (isnan(humidity) || isnan(temperature))
    {
        Serial.println(F("DHT11 reading failed!"));
        // 如果读取失败，使用上次的有效值
        temperature = lastValidTemperature;
        humidity = lastValidHumidity;
        return false;
    }
    else
    {
        // 更新最后的有效值
        lastValidTemperature = temperature;
        lastValidHumidity = humidity;

        // 在串口打印数据
        Serial.print(F("Temperature: "));
        Serial.print(temperature);
        Serial.print(F("°C, Humidity: "));
        Serial.print(humidity);
        Serial.println(F("%"));
        return true;
    }
}

// 读取PM2.5数据
int readPM25()
{
    // GP2Y1014AU0F需要特定的时序来读取数据
    digitalWrite(PM25_PUL_PIN, LOW);
    delayMicroseconds(280);
    int sensorValue = analogRead(PM25_AO_PIN);
    delayMicroseconds(40);
    digitalWrite(PM25_PUL_PIN, HIGH);
    delayMicroseconds(9680);

    // 将模拟值转换为PM2.5浓度
    float voltage = sensorValue * (3.3 / 4095.0); // ESP32的ADC是12位(0-4095)
    float dustDensity = 0.17 * voltage - 0.1;
    if (dustDensity < 0)
    {
        dustDensity = 0;
    }

    int pmValue = (int)(dustDensity * 1000); // 返回μg/m3整数值
    Serial.print(F("PM2.5: "));
    Serial.print(pmValue);
    Serial.println(F(" μg/m3"));

    return pmValue;
}

// 读取水位数据
int readWaterLevel(bool &warning)
{
    // 读取模拟值
    int waterLevelValue = analogRead(WATER_AO_PIN);

    // 读取数字值判断是否有低水位警告
    warning = digitalRead(WATER_DO_PIN) == LOW; // 假设低电平表示水位警告

    // 将模拟值转换为1-100的百分比
    // 假设水位传感器模拟值范围：0(空) - 4095(满) （ESP32的ADC是12位）
    int waterLevelPercent = map(waterLevelValue, 0, 4095, 1, 100);

    // 确保百分比在1-100范围内
    waterLevelPercent = constrain(waterLevelPercent, 1, 100);

    Serial.print(F("Water level: "));
    Serial.print(waterLevelValue);
    Serial.print(F(" ("));
    Serial.print(waterLevelPercent);
    Serial.print(F("%), Warning: "));
    Serial.println(warning ? F("Yes") : F("No"));

    return waterLevelPercent; // 返回百分比值
}