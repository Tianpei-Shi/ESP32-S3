#ifndef __USART_H
#define __USART_H

void delay(unsigned long uldata);  // 添加delay函数声明
void UartIni(void);
void PrintCom(uint8_t *str);  // 修改为uint8_t *类型
void UartSendByte(uint8_t dat);  // 修改为uint8_t类型

#endif
