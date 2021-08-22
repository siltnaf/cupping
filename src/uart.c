#include "start.h"

#if (Seril_Debug == 1)
char UAR[20], temp, rec_flag = 0;
//定时器2，波特率9600
void Uart1_Init(void) //@11.0592MHz
{
	EA = 0;
	ACC = P_SW1;
	· ACC &= ~(S1_S0 | S1_S1); //S1_S0=0 S1_S1=0
	P_SW1 = ACC;			   //(P3.0/RxD, P3.1/TxD)
	SCON = 0x50;			   //8位数据,可变波特率
	AUXR |= 0x01;			   //串口1选择定时器2为波特率发生器
	AUXR |= 0x04;			   //定时器时钟1T模式
	T2L = 0xE0;				   //设置定时初始值
	T2H = 0xFE;				   //设置定时初始值
	AUXR |= 0x10;			   //定时器2开始计时
	ES = 1;
	EA = 1;
}
/***************************************************************************/
void send1_Byte(unsigned char c)
{
	SBUF = c;
	while (!TI)
		; //发送完会自动置1
	TI = 0;
}
/*----------------------------
发送字符串
----------------------------*/
void Send1_String(char *s)
{
	while (*s)
	{ //检测字符串结束标志
		send1_Byte(*s++);
	}
}
/*************************************************************
串口接收到的数据存入UAR数组中，当接收到0x0D时表示数据接收结束
**************************************************************/
void UART1_Interrupt(void) __interrupt UART1_VECTOR //串口中断函数接收程序
{
	static unsigned char i;
	if (RI)
	{
		RI = 0;
		temp = SBUF;
		UAR[i] = temp; //接收到的数据存入数组UAR中
		i++;
		if (temp == 0x0D)
		{
			i = 0;
			rec_flag = 1; //定义接收完成标志位
		}
	}
}
/***********************************************************************/
void UART1_config()
{
	Uart1_Init();									//串口一初始化
	Send1_String("STC15W204S\r\nUart is ok !\r\n"); //发送字符串检测是否初始化成功
}
#endif
//串口打印函数不要开启，占内存严重
//*
//void UART1_printf(const char *fmt,...)
//{
//   va_list ap;
//	char teststring[256];
//   va_start(ap,fmt);
//   vsprintf(teststring,fmt,ap);
//  Send1_String(teststring);
// va_end(ap);
//}

/***********************************************************************
* 功能：将一个十六进制字节串转换成ASCII码表示的十六进制字符串
* 输入参数：pHex----十六进制数字节串首地址
*                   pAscii---转换后ASCII码表示的十六进制字符串首地址
*                   nLen---要转换的十六进制数长度（字节数）
* 注：转换后的结果全部是大写ASCII码表示的十六进制数
************************************************************************/
void HexToAscii(unsigned char *pHex, unsigned char *pAscii, unsigned char nLen)
{
    unsigned char Nibble[2];
    unsigned char i,j;
    for (i = 0; i < nLen; i++){
        Nibble[0] = (pHex[i] & 0xF0) >> 4;
        Nibble[1] = pHex[i] & 0x0F;
        for (j = 0; j < 2; j++){
            if (Nibble[j] < 10){            
                Nibble[j] += 0x30;
            }
            else{
                if (Nibble[j] < 16)
                    Nibble[j] = Nibble[j] - 10 + 'A';
            }
            *pAscii++ = Nibble[j];
        }               // for (int j = ...)
    }           // for (int i = ...)
}

