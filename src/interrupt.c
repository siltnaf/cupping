#include <start.h>

void TimeReset(void)

{
    Key.pressed = 1;
    Key.update = 0;
    Time.sec = 0;
    Time.min = 0;
}

void KeyTimer(void) __interrupt TIMER0_VECTOR //150ms period
{
    Time.update = 1;

    if (Key.pressed == 1)
        Key.debounce++;
    /* 
    
       
    } */
}

void PWMTimer(void) __interrupt TIMER2_VECTOR
{
    Time.PWM = 1;
}

/********************* INT0中断函数 *************************/
void INT0_int(void) __interrupt INT0_VECTOR //进中断时已经清除标志
{


    if (Key.pressed == 0)
        Key_pressed = Key_Power;
    else
        Key_pressed = 0;

    TimeReset();

}

/********************* INT1中断函数 *************************/
void INT1_int(void) __interrupt INT1_VECTOR //进中断时已经清除标志
{

    
    if (Key.pressed == 0)
        Key_pressed = Key_Pump;
    else
        Key_pressed = 0;
    TimeReset();

}
/********************* INT2中断函数 *************************/
void INT2_int(void) __interrupt INT2_VECTOR //进中断时已经清除标志
{


    if (Key.pressed == 0)
        Key_pressed = Key_Vibration;
    else
        Key_pressed = 0;
    TimeReset();

}

/********************* INT2中断函数 *************************/
void INT3_int(void) __interrupt INT3_VECTOR //进中断时已经清除标志
{

 
    if (Key.pressed == 0)
        Key_pressed = Key_PTC;
    else
        Key_pressed = 0;

    TimeReset();

}
