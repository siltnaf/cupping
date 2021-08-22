
#include <start.h>


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
State_name state;


signed char button_number[] = "0";


void Dump_value(u8 val)
{
#if (Seril_Debug == 1)
            Send1_String("debug value\r\n"); //有按键操作发送字

            button_number[0] = val;
            HexToAscii(button_number, button_number, 1);
            Send1_String(button_number);
            Send1_String("\r\n"); 

            // Send1_String(button_number);
#endif
}



void Start(void)
{

    DeviceInit();

    //hx711通道初始化
    channel.A128 = 24; //A通道128
    channel.B32 = 25;  //B通道32
    channel.A64 = 26;  //A通道64

    HX711_Read(channel.B32);

#if (Seril_Debug == 1)
    Send1_String("STC15W204S\r\nUart is ok !\r\n");      //发送字符串检测是否初始化成功
    Send1_String("gn1616_start\r\ndelay_ms(1000)!\r\n"); //发送字符串检测是否初始化成功
#endif
    Key.update=0;
    Key.long_press_state=0;

    while (1)
    {
        EA=1;
     
        if (Time.update)
            Time_handler();

        if ((Key.update) || (Key.long_press_state)) //按键中断flag;
        {
            EA = 0;

       
           
            
            Key_handler();
            IO_handler();
            Display_handler();
    
           

        Key.update = 0;
        Key.long_press_state = 0;

        }

        switch (state)
        {
        case idle_mode:

            state = idle_mode;
            break;

        case normal_mode:
            state = normal_mode;
            break;
       case Power_down:
            
            DeviceInit();
            
            EA = 1;
             EX1 = 0;   
                        //disable other key interrupt
             WAKE_CLKO &= 0xef;    //disable other key interrupt
             WAKE_CLKO &= 0xdf;     //disable other key interrupt
             PCON |= 0x02; //sleep mode only power key can wake up

            WAKE_CLKO |= 0x20;
            WAKE_CLKO |= 0x10;
            EX1=1;
            EA=0;
            Key.which_press=Key_Power;
            Power.on=1;
    
            state = idle_mode;
            break;
        default:
            break;
        }
    }

    //
}
