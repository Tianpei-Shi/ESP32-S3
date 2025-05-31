#ifndef AUDIO_H
#define AUDIO_H

#include <Arduino.h>
#include <driver/i2s.h>

// 麦克风I2S引脚
#define I2S_SCK 47
#define I2S_WS 14
#define I2S_SD 38

// 音频设置
#define I2S_PORT I2S_NUM_0
#define SAMPLE_RATE 16000
#define BUFFER_SIZE 1024

// 初始化I2S麦克风
bool initMicrophone();

// 读取麦克风数据
bool readMicrophoneData(int16_t *buffer, size_t &bytes_read);

// 音频采集与转化为文本（后期集成语音识别）
String captureAndRecognizeSpeech();

#endif // AUDIO_H