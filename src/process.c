
#include <start.h>

void Time_handler(void)
{
    Time.update = 0;
    Time.count++;
    if (Time.count >= 250)
        Time.blink = !Time.blink;
    if (Time.count >= 1000)
    {
        Time.sec++;
        Time.count = 0;
    }
    if (Time.sec >= 60)
    {
        Time.sec = 0;
        Time.min++;
    }
    if (Time.min >= 60)
    {
        Time.min = 0;
    }
}

void key_up(Level *this_key)
{

    this_key->level++;
    if (this_key->level > (Max_key - 1))
        this_key->level = 0;
    if (this_key->level == 0)
        this_key->on = 0;
    else
        this_key->on = 1;
}

void Key_handler(void)

{

    Key.which_press = Key_pressed;
    if ((Key.which_press == Key_Power) && (Key.long_press_state))

    {

        next_state = Power_down;

        //如果检测到长时间按键则打开电源及升压

        // EA = 0;

        // IO_Pump = 0;
        // IO_Vibration = 1;
        // IO_PTC = 0;
        // IO_Valve = 1;

        // Suction.on = 0;
        // Heating.on = 0;
        // Vibration.on=0;

        // Key.long_press_state = 0; //reset press long time flag;
        // IO_Power = !IO_Power;     //trun on the power switch;
        // if (IO_Power)
        // {
        //     display(0xff, GIRD1);
        //     display(0xff, GIRD2);
        //     delay_ms(3000);
        //     display(0x00, GIRD1);
        //     display(0x00, GIRD2);
        // }
        // else
        // {
        //     display(0x00, GIRD1);
        //     display(0x00, GIRD2);
        //     EA = 1;
        //     EX1 = 0;
        //     WAKE_CLKO &= 0xef;
        //     WAKE_CLKO &= 0xdf;
        //     PCON |= 0x02; //sleep mode

        //     WAKE_CLKO |= 0x20;
        //     WAKE_CLKO |= 0x10;
        //     EX1 = 1;
    }
    else
    {

        switch (Key.which_press)
        {

        case Key_PTC:
            // IO_PTC_LED=!IO_PTC_LED;
            // PWM.value = 2000;
            // LoadPWM(PWM.value);
            // Timer2_init();
            // Heating.on = !Heating.on; //启动IO_PTC加热
            key_up(&Heating);

            break;
        case Key_Vibration:
            // Vibration_LED=!Vibration_LED;

            // IO_Vibration = !IO_Vibration;
            key_up(&Vibration);

            break;
        case Key_Pump:
            // Pump_LED=!Pump_LED;

            // IO_Pump = !IO_Pump; //启动气泵开关
            key_up(&Suction);

            break;
        case Key_Power:
            //Power_LED=!Power_LED;

            // IO_Valve = !IO_Valve;
            key_up(&Power);
            if (Power.on==0) next_state=Power_down;

            break;
            // -------------------------------
            // Default event handler.
        default:
            break;
        }
        Key.update = 0;
        Key.long_press_state = 0;
    }
}

void IO_handler(void)
{
    if (Heating.on)
        IO_PTC = 1;
    else
        IO_PTC = 0;

    if (Power.on == 0)
    {

        IO_Pump = 0;
        IO_Vibration = 1;
        IO_PTC = 0;
        IO_Valve = 1;
    }
    else 
        {
         Suction.on=1;
         next_state=normal_mode;

        }
       

    if (Vibration.on)
        IO_Vibration=0;
        else 
        IO_Vibration=1;

     if (Suction.on)
        IO_Pump=1;
        else
        IO_Pump=0;  


}

void Display_handler(void)
{
    if (Heating.on)
        display(0xf0, GIRD2);
    else
        display(0x00, GIRD2);

    if (Vibration.on)
        display(0x0f, GIRD1);
    else
        display(0x00, GIRD1); /* code */
    if (Suction.on)
        display(0xf0, GIRD1);
    else
        display(0x00, GIRD1);

    if (Power.on)
        display(0x0f, GIRD2);
    else
        display(0x00, GIRD2);
}
