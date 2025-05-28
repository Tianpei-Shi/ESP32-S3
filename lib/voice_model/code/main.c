#include "config.h"

uint8 idata nAsrStatus=0;
void MCU_init();
void ProcessInt0(); //识别处理函数
void delay(unsigned long uldata);
void User_handle(uint8 dat);//用户执行操作函数
void Delay200ms();
void Led_test(void);//单片机工作指示
uint8_t G0_flag=DISABLE;//命令标志，ENABLE:允许。DISABLE:禁止命令
sbit LED=P4^2;//信号指示灯

sbit SRD1 = P1^7;
sbit SRD2 = P1^6;
sbit SRD3 = P1^5;
sbit SRD4 = P1^4;

void  main(void)
{
  uint8 idata nAsrRes;
  uint8 i=0;
  P1M0 = 0xFF;
  P1M1 = 0x00;
  SRD1 = SRD2 = SRD3 = SRD4 =0;
  Led_test();
  MCU_init();
  LD_Reset();
  UartIni(); /*初始化串口*/
  nAsrStatus = LD_ASR_NONE;		//	初始状态，没有检测到ASR

  while(1)
  {
    switch(nAsrStatus)
    {
    case LD_ASR_RUNING:
    case LD_ASR_ERROR:
      break;
    case LD_ASR_NONE:
    {
      nAsrStatus=LD_ASR_RUNING;
      if (RunASR()==0)	/*	检测到一个ASR，开始处理ASR，ASR开始处理，ASR处理结束*/
      {
        nAsrStatus = LD_ASR_ERROR;
      }
      break;
    }
    case LD_ASR_FOUNDOK: /*	检测到一个ASR，开始处理，处理结束，取走ASR*/
    {
      nAsrRes = LD_GetResult();		/*取结果*/
      User_handle(nAsrRes);//用户执行操作
      nAsrStatus = LD_ASR_NONE;
      break;
    }
    case LD_ASR_FOUNDZERO:
    default:
    {
      nAsrStatus = LD_ASR_NONE;
      break;
    }
    }// switch
  }// while

}

void Led_test(void)
{
  LED=~ LED;
  Delay200ms();
  LED=~ LED;
  Delay200ms();
  LED=~ LED;
  Delay200ms();
  LED=~ LED;
  Delay200ms();
  LED=~ LED;
}

void MCU_init()
{
  P0 = 0xff;
  P1 = 0x00;
  P2 = 0xff;
  P3 = 0xff;
  P4 = 0xff;


  LD_MODE = 0;		
  IE0=1;
  EX0=1;
  EA=1;
}

void Delay200us()		//@22.1184MHz
{
  unsigned char i, j;
  _nop_();
  _nop_();
  i = 5;
  j = 73;
  do
  {
    while (--j);
  }
  while (--i);
}

void  delay(unsigned long uldata)
{
  unsigned int j  =  0;
  unsigned int g  =  0;
  while(uldata--)
    Delay200us();
}

void Delay200ms()		//@22.1184MHz
{
  unsigned char i, j, k;

  i = 17;
  j = 208;
  k = 27;
  do
  {
    do
    {
      while (--k);
    }
    while (--j);
  }
  while (--i);
}


void ExtInt0Handler(void) interrupt 0
{
  ProcessInt0();
}

void 	User_handle(uint8 dat)
{
  if(0==dat)
  {
    G0_flag=ENABLE;
    PrintCom("shou dao\n");  // 收到
    LED=0;
  }
  else if(ENABLE==G0_flag)
  {
    G0_flag=ENABLE;
    LED=1;
    switch(dat)
    {
        case CODE_1:   /*加湿*/
            SRD1 = 1;  // Turn on humidifier relay
            PrintCom("yi jia shi\n");  // 已加湿
            break;
        case CODE_2:   /*关闭加湿*/
            SRD1 = 0;  // Turn off humidifier relay
            PrintCom("guan bi jia shi\n");  // 关闭加湿
            break;
        case CODE_3:   /*前进*/
            SRD2 = 1;  // Forward motor control
            SRD3 = 0;
            PrintCom("qian jin\n");  // 前进
            break;
        case CODE_4:   /*后退*/
            SRD2 = 0;  // Backward motor control
            SRD3 = 1;
            PrintCom("hou tui\n");  // 后退
            break;
        default:
            // Stop all motors
            SRD2 = 0;
            SRD3 = 0;
            break;
    }
  }
  else
  {
    PrintCom("Please say command\n"); 
  }
}
