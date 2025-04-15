#include "audio.h"

// 音频缓冲区
int16_t sBuffer[BUFFER_SIZE];

// 初始化I2S麦克风
bool initMicrophone()
{
    // 配置I2S
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = BUFFER_SIZE,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0};

    // 配置I2S引脚
    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_SCK,
        .ws_io_num = I2S_WS,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = I2S_SD};

    // 安装并启动I2S驱动
    esp_err_t result = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
    if (result != ESP_OK)
    {
        Serial.println("I2S driver initialization failed");
        return false;
    }

    result = i2s_set_pin(I2S_PORT, &pin_config);
    if (result != ESP_OK)
    {
        Serial.println("I2S pin configuration failed");
        return false;
    }

    Serial.println("Microphone initialization complete");
    return true;
}

// 读取麦克风数据
bool readMicrophoneData(int16_t *buffer, size_t &bytes_read)
{
    // 从I2S读取数据
    esp_err_t result = i2s_read(I2S_PORT, buffer, BUFFER_SIZE * sizeof(int16_t), &bytes_read, portMAX_DELAY);
    return (result == ESP_OK);
}

// 音频采集与转化为文本（未来集成语音识别）
String captureAndRecognizeSpeech()
{
    Serial.println("Starting recording...");

    // 简单示例：录制1秒音频并返回模拟的识别结果
    // 在实际应用中，这里应该实现真正的语音识别功能

    size_t bytes_read = 0;
    unsigned long startTime = millis();
    int totalSamples = 0;

    // 录音1秒钟
    while (millis() - startTime < 1000)
    {
        if (readMicrophoneData(sBuffer, bytes_read))
        {
            totalSamples += bytes_read / sizeof(int16_t);
        }
    }

    Serial.print("Recording complete, collected ");
    Serial.print(totalSamples);
    Serial.println(" samples");

    // 这只是模拟语音识别，实际项目中应该集成真正的语音识别功能
    // 返回一个预定义的文本作为模拟识别结果
    String recognizedText = "Turn on humidifier";

    return recognizedText;
}