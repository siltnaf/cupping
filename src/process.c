
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
            Vibration.duty = 40;
        if (Vibration.level == 2)
            Vibration.duty = 60;
        if (Vibration.level == 3)
            Vibration.duty = 100;

        IO_Vibration = Vibration.output;
    }

    else
    {
        IO_Vibration = 0;
        Vibration.level = 0;
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

    /*  if ((Time.count%2)==0)
        Time.quartersec = 1;
        else Time.quartersec = 0; */

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
        if (Key.lock == 0)
            Key.debounce++;

        if (Key.debounce > 30)
        {

            Key.update = 1;
            Key.long_press_state = 1;
            Key.debounce = 0;
        }
    }

    else
    {

        if ((Key.debounce > 4) && (Key.debounce < 50))

        {

            Key.update = 1;
            Key.pressed = 0;
            Key.debounce = 0;
            Key.long_press_state = 0;
        }
        else

        {

            Key.update = 0;
            Key.pressed = 0;
            Key.debounce = 0;
            Key.lock = 0;
            Key.long_press_state = 0;
        }
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
        Key.long_press_state = 0;
        Key.lock = 1;
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
