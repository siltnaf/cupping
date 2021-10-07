
#include <start.h>

unsigned char LED1, LED2;

void release_pressure(void)
{

    Display_handler();
    Valve_open;
    delay_ms(30000);
    delay_ms(30000);
    delay_ms(30000);
    Valve_close;
}

void Lock_pressure(unsigned char keep_pressure)
{

    /*  if (sensor.pressure < (keep_pressure - upper_bound))
        Valve_open;
    else
        Valve_close; */

    if ((sensor.pressure > keep_pressure) && (sensor.pressure_inrange == 0)) //activate the pump if pressure is below the level
        Suction.duty = 100;                                                  //use 100% duty for pump power
    else
    {
        Suction.duty = 0; //stop the pump if pressure exceed the level
        sensor.pressure_inrange = 1;
        Time.error = 0;
    }
    if ((sensor.pressure) > (keep_pressure + lower_bound)) //reactivate the pump if pressure is below the lower boundary
        sensor.pressure_inrange = 0;
}

void service(void) //background service running all the time

{

volatile signed char delta;
unsigned char target_temperature[4] = {0,Low_heat, Med_heat, High_heat};


    if (Heating.on)
    {
        if (sensor.update_heating_PWM == 1)
        {

            
                delta=sensor.temperature-target_temperature[Heating.level];
             
        
                    Heating.duty += delta >> 1;
                
            
                    
                    if (Heating.duty > 99)
                        Heating.duty = 100;
                    if (Heating.duty<20)
                        Heating.duty=20;
                
            
           
        }
        IO_PTC = Heating.output;
        sensor.update_heating_PWM = 0;
    }

    else
    {
        IO_PTC = 0; // if Heating is off , reset the level to 0
        Heating.level = 0;
    }

     if (Vibration.on)
    {
        if (Vibration.level == 1) //use Timer2, create 50 HZ pulse
            Time.Hzmax = Hz_30;
        if (Vibration.level == 2)
            Time.Hzmax = Hz_50; //use Timer2, create 30 HZ pulse
        if (Vibration.level == 3)
            Time.Hzmax = Hz_max; //use Timer2, create 20 HZ pulse
    }

    else
    {

        Vibration.level = 0;
    }

    if (Suction.level == 0) // if suction is off , release the pressure through valve
    {

        Heating.on = 0;
        Vibration.on = 0;
        IO_Pump = 0;
    }

    if (Suction.on)
    {
        IO_Pump = Suction.output;
        if (Suction.level == 1)
            Lock_pressure(Low_suction);
        if (Suction.level == 2)
            Lock_pressure(Med_suction);
        if (Suction.level == 3)
            Lock_pressure(High_suction);
    }
    else
    {
        IO_Pump = 0; //when pump is off, turn off Heating and vibration as well
        Suction.level = 0;
        Heating.level = 0;
        Vibration.level = 0;
        Valve_close;
    }

    if (Time.beep == 1)

    {
        delay_ms(30000);
        Time.beep = 0;
        delay_ms(30000);
        Time.beep = 1;
        delay_ms(30000);
        Time.beep = 0;
        Timer2_init();
    }

    if (Battery.on==1)
        Battery.level=3;
        else Battery.level=0;

} 

void Timer_Reset(void)
{
    Time.count = 0;
    Time.sec = 0;
    Time.min = 0;
}

void Time_handler(void) //Timer 0 is 50ms period,
{
    Time.update = 0;
    Time.count++;

    if (Time.count < 10)
        Time.blink = 1;
    else
        Time.blink = 0;

    if (Time.count > 39)
    {
        //check battery level every second

        P1M1 |=0b00000100;     //P1.2 change to input
        if ((BAT_check==1)&& (Battery.level!=1))
            {

                Battery.level=1;

            }

        if ((BAT_check==0)&& (Battery.level!=3))
            {
                Battery.level=3;
            }
        P1M1 &=0b11111011;

        Time.sec++;
        Time.count = 0;
    }

    if ((Time.count==0))
        HX711_Read(hxsensor->P128);
        if (Time.count==10)
        {
        sensor.raw_pressure = (HX711_Read(hxsensor->P128) ^ 0x00800000);
        sensor.pressure = sensor.raw_pressure >> 12;
        sensor.update_suction_PWM = 1;
         Dump_value(sensor.pressure);     
        }
  
        

    if (Time.count==20)
            HX711_Read(hxsensor->T32);

    if (Time.count==30)        
        {
   
        sensor.raw_temperature = (HX711_Read(hxsensor->T32) ^ 0x00800000);

        sensor.temperature = sensor.raw_temperature >> 16;
        sensor.update_heating_PWM = 1;
         Dump_value(sensor.temperature);
        }

    if (Time.sec > 59)
    {
        Time.sec = 0;
        Time.min++;
    }
    if (Time.min > 59)
    {
        Time.min = 0;
    }

    if ((Time.min >= Time1) && (state != Timer_end))
    {
        BUZ_init();
        Time.beep = 1;
        state = Timer_end;
    }

    if ((!INT1) || (!INT2) || (!INT3))
    {
        //if one of the key is pressed

        if (Key.debounce > 30) //key hold for 2 sec
        {

            Key.update = 1;           //flag to activate key handler
            Key.long_press_state = 1; //key hold over 2 sec flag activated
            Key.debounce = 30;        //hold debounce time so it will wait key.pressed==0 then refresh the debounced value
        }
    }

    else
    { // key is release. now check the press time in valid range, the keyupdate will be enable

        if ((Key.debounce > 4) && (Key.debounce < 30))
            Key.update = 1;
        else
            Key.update = 0;

        Key.pressed = 0; //if no key is pressed , key press is cleared
        Key.debounce = 0;
        Key.long_press_state = 0; //if no key is pressed, long press state=0;
    }
}

void key_up(Level *this_key)
{

    this_key->level++; //loop the key level for 0->1->2->3 and back to 0
    if (this_key->level > (Max_key - 1))
        this_key->level = 0;
    if (this_key->level == 0)
        this_key->on = 0; //if the level is not = 0, assume the function for this key is power on stage
    else
        this_key->on = 1;
}

void Key_handler(void)

{

    Timer_Reset();

    if (Key.long_press_state)
    {

        switch (Key.which_press)
        {

        case Key_PTC:

            Heating.on = 0;
            break;
        case Key_Vibration:

            Vibration.on = 0;

            break;
        case Key_Pump:

            Suction.on = 0;
            Suction.level = 0;
            state = Timer_end;

            break;
        default:

            break;
        }
    }
    else

        switch (Key.which_press)
        {

        case Key_PTC:

            key_up(&Heating);

            break;
        case Key_Vibration:

            key_up(&Vibration);

            break;
        case Key_Pump:
             if (state == Timer_end)
            {
                Suction.on = 0;
                Suction.level = 0;
            }
            else 
                key_up(&Suction);

            break;

        default:
            break;
        }

    Key.update = 0;
    Key.long_press_state = 0;
}

void Key_Setting_handler(void)
{

    service(); //update key status

    //update key status

    if (Suction.level == 0) // if suction is off , release the pressure through valve
    {

        release_pressure();
        state = Timer_end;
    }

    if (Heating.level != 0)
        Heating.duty = 50;
}

void Display_handler(void)
{
    u8 level_val[Max_key] = {0, 1, 3, 7};
    u8 level_val2[Max_key]={0,1,2,4};
    u8 display_val = 0;

    display_val = (level_val[Vibration.level]) & 0b0000111;

    display_val |= (level_val[Suction.level] << 4) & 0b1110000;

    LED1 = display_val;

    display_val = (level_val[Heating.level]) & 0b0000111;
    display_val |= (level_val2[Battery.level] << 4) & 0b1110000;

    LED2 = display_val;

    display(LED2, GIRD2);
    display(LED1, GIRD1);
}

void Display_ring(void)
{
    LED1 = 0b1111111;
    LED2 = 0b1111111;
    display(LED2, GIRD2);
    display(LED1, GIRD1);
}
