
#include <start.h>

unsigned char LED1, LED2;

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
        Time.error=0;
    }
    if ((sensor.pressure) > (keep_pressure + lower_bound)) //reactivate the pump if pressure is below the lower boundary
        sensor.pressure_inrange = 0;
}

void service(void) //background service running all the time

{

    if (Heating.on)
    {
        if (Heating.level == 1) //set duty for heating PTC power
             if (sensor.temperature < Low_heat)
                Heating.duty = 20;
            else
                Heating.duty = 100;

        if (Heating.level == 2)
             if (sensor.temperature < Med_heat)
                Heating.duty = 20;
            else 
                Heating.duty = 100;

        if (Heating.level == 3)
             if (sensor.temperature < High_heat)
                Heating.duty = 20;
            else 
                Heating.duty = 100;

        IO_PTC = Heating.output;
    }

    else
    {
        IO_PTC = 0; // if Heating is off , reset the level to 0
        Heating.level = 0;
    }

    if (Vibration.on)
    {
        if (Vibration.level == 1) //use Timer2, create 50 HZ pulse
            Time.Hzmax = Hz_50;
        if (Vibration.level == 2)
            Time.Hzmax = Hz_30; //use Timer2, create 30 HZ pulse
        if (Vibration.level == 3)
            Time.Hzmax = Hz_20; //use Timer2, create 20 HZ pulse

        IO_Vibration = Time.Hzout;
    }

    else
    {
        IO_Vibration = 0; // if Vibration is off , reset the level to 0
        Vibration.level = 0;
    }

    if (Suction.level == 0)  // if suction is off , release the pressure through valve
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

    if (Time.count > 19)
    {
        Time.sec++;
        Time.count = 0;
    }

    if ((Time.sec % 4 == 0) && (Time.count == 10))
        HX711_Read(hxsensor->P64);

    if ((Time.sec % 4 == 1) && (Time.count == 10))
        sensor.pressure = HX711_Read(hxsensor->P64) >> 16;

    if ((Time.sec % 4 == 2) && (Time.count == 10))
        HX711_Read(hxsensor->T32);

    if ((Time.sec % 4 == 3) && (Time.count == 10))

    {
        sensor.data = HX711_Read(hxsensor->T32)&0x000fffff-0x00080000;
        sensor.temperature = (sensor.data >> 11);

        
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
    if ((Power.level == 3) && (Time.min >= Time3)) //update timer counter if time drop to Time2
    {
        Power.level = 2;
        Time.min = 0;
        Display_handler(); //refresh display
    }
    if ((Power.level == 2) && (Time.min >= Time2)) //update timer counter if time drop to Time1
    {
        Power.level = 1;
        Time.min = 0;
        Display_handler(); //refresh display
    }
    if ((Power.level == 1) && (Time.min >= Time1)) //update timer counter if time drop to 0
    {
        state = Timer_end;
        Display_handler();
        Time.min = 0;
    }

    if ((!INT0) || (!INT1) || (!INT2) || (!INT3))
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

    //  Timer_Reset();

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

            break;
        case Key_Power:

            Power.on = 0;
            Power.level = 0;

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
        case Key_Power:

            key_up(&Power);
            if (Power.on == 0)
            {
                state = Timer_end;
                Power.level = 0;
            }

            break;

        default:
            break;
        }

    Key.update = 0;
    Key.long_press_state = 0;
}

void IO_handler(void)
{

    service(); //update key status

    //update key status


    if (Suction.level == 0)  // if suction is off , release the pressure through valve
    {

        Valve_open;
        delay_ms(30000);
        delay_ms(30000);
        delay_ms(30000);
        Valve_close;
    }


    if (Power.on)
    {
        IO_Power = 1;

        switch (Power.level)
        {
        case 0:
            Power.level = 1;
        case 1:
            duration = Time1;

            break;

        case 2:
            duration += Time2;

            break;
        case 3:
            duration += Time3;

            break;

        default:
            break;
        }
    }

    else
    {

        state = Timer_end;
    }
}

void Display_handler(void)
{
    u8 level_val[Max_key] = {0, 1, 3, 7};
    u8 display_val = 0;

    display_val = (level_val[Vibration.level]) & 0b0000111;

    display_val |= (level_val[Suction.level] << 3) & 0b0111000;

    LED1 = display_val;

    display_val = (level_val[Heating.level]) & 0b0000111;
    display_val |= (level_val[Power.level] << 3) & 0b0111000;

    LED2 = display_val;

    display(LED2, GIRD2);
    display(LED1, GIRD1);
}
