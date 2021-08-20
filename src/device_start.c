
#include <start.h>
unsigned char PWM_ON; //定义按键时间


//自定义枚举4个按键按下时的值；发热键，电源键，气泵键，振动键

//Setting level
enum Setting_level
{
    Level0 = 0,
    Level1 = 1,
    Level2 = 2
} Levels;



//hx711通道；
union
{
    unsigned char A128;
    unsigned char B32;
    unsigned char A64; /*channel data */
} channel;

Button_type Key_pressed;
Button_Status Key; //初始化按键
Level Power, Vibration, Suction, Heating;
PWM_Status PWM;
Timer_Status Time;
State_name pre_state,next_state;






void Start(void)
{

    DeviceInit();

    //hx711通道初始化
    channel.A128 = 24; //A通道128
    channel.B32 = 25;  //B通道32
    channel.A64 = 26;  //A通道64

    display(0x00, GIRD2);
    HX711_Read(channel.B32);

#if (Seril_Debug == 1)
    Send1_String("STC15W204S\r\nUart is ok !\r\n");      //发送字符串检测是否初始化成功
    Send1_String("gn1616_start\r\ndelay_ms(1000)!\r\n"); //发送字符串检测是否初始化成功
#endif

    EA = 1;
    while (1)
    {
        EA = 1;
        if (Time.update) Time_handler();
        
        //if ((MOTOR_FLAG==1)) IO_Vibration=1;else IO_Vibration=0;

        if ((Key.update)||(Key.long_press_state)) //按键中断flag;
        {
            EA = 0;
           
#if (Seril_Debug == 1)
            Send1_String("copping_=1\r\n"); //有按键操作发送字符
            Send1_String("Button\r\n");     //发送按键值
            send1_Byte(Key.which_press);
#endif

            Key_handler();
            IO_handler();
            Display_handler();

        }


        

#if (Seril_Debug == 1)
            Send1_String("long_press_state=1\r\n"); //发送字符串检测是否初始化成功
#endif
        }
        
     //
}
