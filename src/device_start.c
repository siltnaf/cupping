
#include <start.h>

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

void Start(void)
{

    DeviceInit();

    //hx711通道初始化
    hx711channel.P128 = 24; //A通道128
    hx711channel.T32 = 25;  //B通道32
    hx711channel.P64 = 26;  //A通道64
    hxsensor = &hx711channel;

    sensor.temperature = HX711_Read(hxsensor->T32);

#if (Seril_Debug == 1)
    Send1_String("STC15W204S\r\nUart is ok !\r\n");      //发送字符串检测是否初始化成功
  
#endif
    Key.update = 0;
    Key.long_press_state = 0;

    while (1)
    {
        EA = 1;
        TR0 = 1;
        ET0 = 1;

        // Dump_ad(sensor_reading);

      // service();

        if (Time.update)
        {
            /* if (Time.sec%2==0)
                sensor.temperature = HX711_Read(hxsensor->T32);
                else 
                sensor.pressure=HX711_Read(hxsensor->P64); */
            Time_handler();
           
        }
        else
        {

  

        }



      //  else IO_handler();

        if ((Key.update) || (Key.long_press_state)) //按键中断flag;
        {
            EA = 0;
            Key.which_press = Key_pressed;

            Key_handler();
            IO_handler();

            

            Display_handler();

            Key.update = 0;
            Key.long_press_state = 0;
        }

       // state_machine();
    }

    //
}
