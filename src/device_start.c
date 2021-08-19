
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
        IO_handler();
        
        //if ((MOTOR_FLAG==1)) IO_Vibration=1;else IO_Vibration=0;

        if (Key.service) //按键中断flag;
        {
            EA = 0;
#if (Seril_Debug == 1)
            Send1_String("copping_=1\r\n"); //有按键操作发送字符
            Send1_String("Button\r\n");     //发送按键值
            send1_Byte(Key.which_press);
#endif
            Key.service = 0;
            Key.which_press = Key_pressed;

            switch (Key.which_press)
            {

            case Key_PTC:
                // IO_PTC_LED=!IO_PTC_LED;
                PWM.value = 2000;
                LoadPWM(PWM.value);
                Timer2_init();
                Heating.on = !Heating.on; //启动IO_PTC加热
                if (Heating.on == 1)
                    display(0xf0, GIRD2);
                else
                    display(0x00, GIRD2);
                break;
            case Key_Vibration:
                // Vibration_LED=!Vibration_LED;

                IO_Vibration = !IO_Vibration;
                if (!IO_Vibration)
                    display(0x0f, GIRD1);
                else
                    display(0x00, GIRD1); /* code */
                break;
            case Key_Pump:
                // Pump_LED=!Pump_LED;

                IO_Pump = !IO_Pump; //启动气泵开关
                if (IO_Pump)
                    display(0xf0, GIRD1);
                else
                    display(0x00, GIRD1);
                break;
            case Key_Power:
                //Power_LED=!Power_LED;
                display(0x0f, GIRD2);
                IO_Valve = !IO_Valve;
                if (!IO_Valve)
                    display(0x0f, GIRD2);
                else
                    display(0x00, GIRD2);
                break;
                // -------------------------------
                // Default event handler.
            default:
                break;
            }
            /*
           switch(Key.times)
           {
               case 1:
               
               break;
               case 2:
               
               break;
               case 3:
               
               break;

                // -------------------------------
               // Default event handler.
              default:
              break;
           }
           */
        }
        if (Key.long_press_state) //如果检测到长时间按键则打开电源及升压
        {
            EA = 0;

            IO_Pump = 0;
            IO_Vibration = 1;
            IO_PTC = 0;
            IO_Valve = 1;

            Suction.on = 0;
            Heating.on = 0;

            Key.long_press_state = 0; //reset press long time flag;
            IO_Power = !IO_Power;     //trun on the power switch;
            if (IO_Power)
            {
                display(0xff, GIRD1);
                display(0xff, GIRD2);
                delay_ms(3000);
                display(0x00, GIRD1);
                display(0x00, GIRD2);
            }
            else
            {
                display(0x00, GIRD1);
                display(0x00, GIRD2);
                EA = 1;
                EX1 = 0;
                WAKE_CLKO &= 0xef;
                WAKE_CLKO &= 0xdf;
                PCON |= 0x02; //sleep mode

                WAKE_CLKO |= 0x20;
                WAKE_CLKO |= 0x10;
                EX1 = 1;
            }
#if (Seril_Debug == 1)
            Send1_String("long_press_state=1\r\n"); //发送字符串检测是否初始化成功
#endif
        }
        EA = 1;
    } //
}
