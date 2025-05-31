// #include <reg51.h> // Removed to avoid redefinition errors, STC11XX.H from config.h should suffice
// #include <stdint.h> // Removed as it's not found and types are in config.h
#include "../code/config.h" // Corrected path to config.h


/************************************************************************
   串口初始化
函数名称：   STC10L08XE 单片机串口初始化函数
返回函数：   none
说明：       none
**************************************************************************/
void UartIni(void)
{
    SCON = 0x50;            //8-bit variable UART
    PCON |= 0x80;           //SMOD = 1, 波特率倍增
    TMOD &= 0x0F;           //清除T1的模式位
    TMOD |= 0x20;           //Set Timer1 as 8-bit auto reload mode
    TH1 = 0xFF;             // 波特率 115200bps @22.1184MHz, SMOD=1
    TL1 = 0xFF;             // 波特率 115200bps @22.1184MHz, SMOD=1
    TR1 = 1;                //Timer1 start run
    ES = 1;                 //Enable UART interrupt
    EA = 1;                 //Open master interrupt switch
}

void UARTSendByte(uint8_t DAT)
{
    ES = 0;                 //Disable UART interrupt while sending
    TI = 0;                 //Clear transmit flag
    SBUF = DAT;            //Load data to send
    while(!TI);            //Wait for transmission complete
    TI = 0;                //Clear transmit flag again
    ES = 1;                //Re-enable UART interrupt
}

void PrintCom(uint8_t *DAT)
{
    while(*DAT)            //Send until end of string
    {
        if(*DAT == '\n')  // 如果是换行符
        {
            UARTSendByte('\r');  // 先发送回车
            UARTSendByte('\n');  // 再发送换行
        }
        else
        {
            UARTSendByte(*DAT);  // 发送普通字符
        }
        DAT++;
    }
}

