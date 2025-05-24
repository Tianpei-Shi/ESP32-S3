#include "config.h"

uint8 idata nAsrStatus=0;
void MCU_init();
void ProcessInt0(); //ʶ��������
void delay(unsigned long uldata);
void User_handle(uint8 dat);//�û�ִ�в�������
void Delay200ms();
void Led_test(void);//��Ƭ������ָʾ
uint8_t G0_flag=DISABLE;//���б�־��ENABLE:���С�DISABLE:��ֹ����
sbit LED=P4^2;//�ź�ָʾ��

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
  UartIni(); /*���ڳ�ʼ��*/
  nAsrStatus = LD_ASR_NONE;		//	��ʼ״̬��û������ASR

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
      if (RunASR()==0)	/*	����һ��ASRʶ�����̣�ASR��ʼ����ASR���ӹؼ��������ASR����*/
      {
        nAsrStatus = LD_ASR_ERROR;
      }
      break;
    }
    case LD_ASR_FOUNDOK: /*	һ��ASRʶ�����̽�����ȥȡASRʶ����*/
    {
      nAsrRes = LD_GetResult();		/*��ȡ���*/
      User_handle(nAsrRes);//�û�ִ�к���
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
		PrintCom("shou dao");
    LED=0;
  }
  else if(ENABLE==G0_flag)
  {
    G0_flag=ENABLE;
    LED=1;
    switch(dat)
    {
        case CODE_1:   /*加湿*/
            PrintCom("yi jia shi\r\n");
            break;
        case CODE_2:   /*关闭加湿*/
            PrintCom("jia shi yi guan bi\r\n");
            break;
        case CODE_3:   /*前进*/
            PrintCom("qian jin\r\n");
            break;
        case CODE_4:   /*后退*/
            PrintCom("hou tui\r\n");
            break;
        default:
            break;
    }
  }
  else
  {
    PrintCom("请说出一级口令"); 
  }
}
