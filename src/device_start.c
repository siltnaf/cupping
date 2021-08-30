
#include <start.h>

<<<<<<< HEAD
unsigned long temperature[] = {0, 0x00490000, 0x00400000, 0x00300000};
unsigned long pressure[]={0,0x00400000,0x00500000,0x00600000};
Button_type Key_pressed;
Button_Status Key; //初始化按键
Level Power, Vibration, Suction, Heating;
PWM_Status PWM;
Timer_Status Time;
Treatment_time duration;

channel hx711channel, *hxsensor;
AD_sensor sensor;
unsigned long sensor_reading;
=======
//自定义枚举4个按键按下时的值；发热键，电源键，气泵键，振动键

//Setting level
/* enum Setting_level
{
    Level0 = 0,
    Level1 = 1,
    Level2 = 2
} Levels;
 */


//hx711通道；
union
{
    unsigned char P128;
    unsigned char T32;
    unsigned char P64; /*channel data */
} channel;

BUTTON Key_pressed;
BUTTON_STAT Key; //初始化按键
LEVEL_STAT Power, Vibration, Suction, Heating;
PWM_STAT PWM;
TIMER_STAT Time;
COUNTER duration;
STATE state;
SENSOR sensor;



>>>>>>> 6929422cb2dec685163bedc43dc510772f404633

void Start(void)
{

    DeviceInit();

    //hx711通道初始化
<<<<<<< HEAD
    hx711channel.P128 = 24; //A通道128
    hx711channel.T32 = 25;  //B通道32
    hx711channel.P64 = 26;  //A通道64
    hxsensor = &hx711channel;

    sensor.temperature = HX711_Read(hxsensor->T32);
=======
    channel.P128 = 24; //A通道128  pressure
    channel.T32 = 25;  //B通道32   ntc
    channel.P64 = 26;  //A通道64   pressure

    HX711_Read(channel.T32);
>>>>>>> 6929422cb2dec685163bedc43dc510772f404633

#if (Seril_Debug == 1)
    Send1_String("STC15W204S\r\nUart is ok !\r\n");      //发送字符串检测是否初始化成功
  
#endif
    Key.update = 0;
    Key.long_press_state = 0;

    while (1)
    {
        EA = 1;
<<<<<<< HEAD
        TR0 = 1;
        ET0 = 1;

        // Dump_ad(sensor_reading);

       service();

        if (Time.update)
        {
            if (Time.sec%2==0)
                sensor.temperature = HX711_Read(hxsensor->T32);
                else 
                sensor.pressure=HX711_Read(hxsensor->P64);
            Time_handler();
           
        }
        else
=======
        TR0=1;
        ET0=1;
        
 
         sensor.ntc=0x12345678;
       // Dump_AD(sensor.ntc); 

        if (Time.update)
            Time_handler();
        if (IN_Power&&IN_PTC&&IN_Vibration&&IN_Pump) Key.pressed=0;
        if ((Key.update)&&(Key.pressed==0)) //按键中断flag;
        {
            EA = 0;

            Key_handler();
            IO_handler();
            Display_handler();
            delay_ms(100);
            Key.update = 0;
            Key.long_press_state = 0;
        }

        

        switch (state)
>>>>>>> 6929422cb2dec685163bedc43dc510772f404633
        {

  

        }


<<<<<<< HEAD

      //  else IO_handler();

        if ((Key.update) || (Key.long_press_state)) //按键中断flag;
        {
            EA = 0;
            Key.which_press = Key_pressed;

            Key_handler();
            IO_handler();

            

            Display_handler();
=======
            DeviceInit();
            Power.level=0;
            EA = 1;
            EX1 = 0;
            //disable other key interrupt
            WAKE_CLKO &= 0xef; //disable other key interrupt
            WAKE_CLKO &= 0xdf; //disable other key interrupt
            PCON |= 0x02;      //sleep mode only power key can wake up

            WAKE_CLKO |= 0x20;
            WAKE_CLKO |= 0x10;
           
            
            state = Power_on;
            break;

case Power_on:
            IO_Power=1;
            Power.on = 1;
       //     Key.update=1;
          
            LED1=0xff;
            LED2=0xff;
            
            display(LED2,GIRD2);
            display(LED1,GIRD1);
            delay_ms(10000);
            LED1=0x00;
            LED2=0x00;
           
            display(LED2,GIRD2);
            display(LED1,GIRD1);
            
            
            Display_handler();

            EX1 = 1;
            EA = 0;
            Key.pressed=1;
            Key.update=0;

            state = idle_mode;
            break;
>>>>>>> 6929422cb2dec685163bedc43dc510772f404633

            Key.update = 0;
            Key.long_press_state = 0;
        }

        state_machine();
    }

    //
}
