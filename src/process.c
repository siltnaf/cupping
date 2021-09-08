
#include <start.h>

unsigned char LED1, LED2;

void service(void)

{

    if (Heating.on)
    {
        if (Heating.level == 1)
            Heating.duty = 50;
        if (Heating.level == 2)
            Heating.duty = 60;
        if (Heating.level == 3)
            Heating.duty = 70;

        IO_PTC = Heating.output;
    }

    else
    {
        IO_PTC = 0;
        Heating.level = 0;
    }

    if (Vibration.on)
    {
        if (Vibration.level == 1)
            Time.Hzmax = Hz_50;
        if (Vibration.level == 2)
            Time.Hzmax = Hz_30;
        if (Vibration.level == 3)
            Time.Hzmax = Hz_20;

        IO_Vibration = Time.Hzout;
    }

    else
    {
        IO_Vibration = 0;
        Vibration.level = 0;
    }

    if (Suction.level == 0)
    {
        if (sensor.pressure < suction_release)
            IO_Valve = 0;
        else
            IO_Valve = 1;
        IO_Pump = 0;
    }

    if (Suction.on)
    {
        IO_Pump = Suction.output;
        if (Suction.level == 1)
        {
            if ((sensor.pressure > Low_suction) && (sensor.pressure_inrange == 0))
                Suction.duty = 100;
            else
            {
                Suction.duty = 0;
                sensor.pressure_inrange = 1;
            }

            if ((sensor.pressure) > (Low_suction + suction_bound))
                sensor.pressure_inrange = 0;
        }
        if (Suction.level == 2)
        {
            if ((sensor.pressure > Med_suction) && (sensor.pressure_inrange == 0))
                Suction.duty = 100;
            else
            {
                Suction.duty = 0;
                sensor.pressure_inrange = 1;
            }

            if ((sensor.pressure) > (Med_suction + suction_bound))
                sensor.pressure_inrange = 0;
        }

        if (Suction.level == 3)
        {
            if ((sensor.pressure > High_suction) && (sensor.pressure_inrange == 0))
                Suction.duty = 100;
            else
            {
                Suction.duty = 0;
                sensor.pressure_inrange = 1;
            }

            if ((sensor.pressure) > (High_suction + suction_bound))
                sensor.pressure_inrange = 0;
        }
    }
    else
    {
        IO_Pump = 0;
        Suction.level = 0;
        Heating.level=0;
        Vibration.level=0;
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

    if ((Time.count % 9) == 0)
        Time.reading = 1;

    if (Time.count > 19)
    {
        Time.sec++;

        Time.count = 0;
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
    if ((Power.level == 3) && (Time.min >= Time3))
    {
        Power.level = 2;

        Time.min = 0;
        Display_handler();
    }
    if ((Power.level == 2) && (Time.min >= Time2))
    {
        Power.level = 1;

        Time.min = 0;
        Display_handler();
    }
    if ((Power.level == 1) && (Time.min >= Time1))
    {
        Power.level = 0;
        state = Power_down;
        Display_handler();
        Time.min = 0;
    }



   

        if ((!INT0) || (!INT1) || (!INT2) || (!INT3)) //if one of the key is release timer is reset
        {
            //if key is pressed and debounce time >3 s, keyupdate will be enable

            if (Key.debounce > 50) 
            {

            
                
                Key.update = 1;
                Key.long_press_state = 1;
                Key.debounce = 50;
            }
        }

        else
        { // key is release. now check the press time in valid range, the keyupdate will be enable

            if ((Key.debounce > 6) && (Key.debounce < 50))
                Key.update = 1;
            else
                Key.update = 0;

            Key.pressed=0;          
            
            Key.debounce = 0;
            Key.long_press_state = 0;
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

            break;
        case Key_Power:

            Power.on = 0;

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
  
            key_up(&Suction);

            break;
        case Key_Power:

            key_up(&Power);

            break;
            // -------------------------------
            // Default event handler.
        default:
            break;
        }

    Key.update = 0;
    Key.long_press_state = 0;
}

void IO_handler(void)
{

    service();

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

        state = Power_down;
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
