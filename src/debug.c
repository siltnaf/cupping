#include <start.h>





unsigned char unicode_number[4]={0};
unsigned char consumer_number[]=""; 
unsigned char button_number[]="";


void Dump_value(u8 val)
{
button_number[0] = val;

#if (Seril_Debug == 1)
    Send1_String("debug value\r\n"); //有按键操作发送字

    
    HexToAscii(button_number, button_number, 1);
    Send1_String(button_number);
    Send1_String("\r\n");

    // Send1_String(button_number);
#endif
}


void Dump_ad(unsigned long reading)
{

    
       unicode_number[3]=reading;

       unicode_number[2]=reading>>8;
    
       unicode_number[1]=reading>>16;
       
       unicode_number[0]=reading>>24;
       #if (Seril_Debug == 1)

       HexToAscii(unicode_number, consumer_number, 4);
       Send1_String("\r\nL+Hpressure=");//发送气压传感器 低 8位
       Send1_String(consumer_number);
       #endif
}




