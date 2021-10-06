#include <start.h>

void TimeReset(void)

{
    Key.pressed = 1;                //interrupt for INT0~3
    Key.update = 0;                 //no update until time handler check the key status
    Time.sec = 0;                   //reset second
    Time.min = 0;                   //reset minutes
}

void KeyTimer(void) __interrupt TIMER0_VECTOR //150ms period
{
    Time.update = 1;

    if (Key.pressed == 1)                              //count key press time
        Key.debounce++;

}

void PWMTimer(void) __interrupt TIMER2_VECTOR
{
   // create 30/50 Hz for vibration
     Time.Hzcount++;
            if (Time.Hzcount < Time.Hzmax)
                Time.Hzout = 1;
            else
                Time.Hzout = 0;
            if (Time.Hzcount >= (Time.Hzmax << 1))
                Time.Hzcount = 0;

            if (Time.Hzmax == Hz_max)
                Time.Hzout = 1;


    
#if (Serial_Debug == 0)
    if (Vibration.on) IO_Vibration=Time.Hzout;else IO_Vibration=0;
#endif

    if (Time.beep==1) 
        {if (IO_BUZ==1) IO_BUZ=0;
            else IO_BUZ=1;
        }
        else IO_BUZ=0;
    Time.PWM = 1;                                       //set PWM trigger
}

/********************* INT0中断函数 *************************/
void INT0_int(void) __interrupt INT0_VECTOR //进中断时已经清除标志
{


   
}

/********************* INT1中断函数 *************************/
void INT1_int(void) __interrupt INT1_VECTOR //进中断时已经清除标志
{

    
                      //if key is previously not holding       
        Key_pressed = Key_Pump;

    TimeReset();

}
/********************* INT2中断函数 *************************/
void INT2_int(void) __interrupt INT2_VECTOR //进中断时已经清除标志
{


                    //if key is previously not holding 
        Key_pressed = Key_PTC;
       
   
    TimeReset();

}

/********************* INT2中断函数 *************************/
void INT3_int(void) __interrupt INT3_VECTOR //进中断时已经清除标志
{
   
                          //if key is previously not holding  
        Key_pressed = Key_Vibration;
      



    TimeReset();

}
