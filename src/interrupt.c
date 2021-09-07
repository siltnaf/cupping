#include <start.h>



void TimeReset(void)

{
    Key.pressed=1;
    Key.update=0;
    Time.sec=0;
    Time.min=0;

}

void KeyTimer(void) __interrupt TIMER0_VECTOR      //150ms period
{
    Time.update=1;
   
    if (Key.pressed==1)
        Key.debounce++;
/* 
    
       
    } */

   
}

void PWMTimer(void) __interrupt TIMER2_VECTOR
{
Time.PWM=1;
      IO_PTC = !IO_PTC; 
}


/********************* INT0中断函数 *************************/
void INT0_int(void) __interrupt INT0_VECTOR //进中断时已经清除标志
{
    
     TimeReset();
    if (Key.lock==0) Key_pressed = Key_Power; else Key_pressed=0;
    // LED1=!LED1;
    //EX0 = 0;	//INT0 Disable
    //IE0 = 0;	//外中断0标志位
}

/********************* INT1中断函数 *************************/
void INT1_int(void) __interrupt INT1_VECTOR //进中断时已经清除标志
{
    
    TimeReset();
    if (Key.lock==0) Key_pressed = Key_Pump; else Key_pressed=0;
    //LED2=!LED2;
    //EX1 = 0;	//INT1 Disable
    //IE1 = 0;	//外中断1标志位
}
/********************* INT2中断函数 *************************/
void INT2_int(void) __interrupt INT2_VECTOR //进中断时已经清除标志
{
    
     TimeReset();
    if (Key.lock==0) Key_pressed = Key_Vibration; else Key_pressed=0;
    // LED2=!LED2;
    //EX1 = 0;	//INT1 Disable
    //IE1 = 0;	//外中断1标志位
}

/********************* INT2中断函数 *************************/
void INT3_int(void) __interrupt INT3_VECTOR //进中断时已经清除标志
{
  
    TimeReset();
    
    if (Key.lock==0) Key_pressed = Key_PTC; else Key_pressed=0;
    // LED2=!LED2;
    //EX1 = 0;	//INT1 Disable
    //IE1 = 0;	//外中断1标志位
}
