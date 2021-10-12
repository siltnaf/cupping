
#include <start.h>

State_name state;

void state_machine(void)
{

    switch (state)
    {
    case Normal_mode:
        Valve_close;
        Battery.level = 3;
        Display_handler();
        state = Normal_mode;
        break;

    case Charging_mode:

        EA = 0;
        Battery.level = 1;
        LED1 = 0x00;
        LED2 = 0b0010000;
        display(LED2, GIRD2);
        display(LED1, GIRD1);

        P1M1 |= 0b00000100; //P1.2 change to input
        NOP();
        NOP();
        NOP();
        NOP();

        while (BAT_check)
            ;

        state = Normal_mode;
        break;

    case Timer_end:
        EA = 0;
        Heating.on = 0;
        Vibration.on = 0;
        if (Time.error == 1)
            state = Power_down;
        Lock_pressure(Stay_suction);
        if ((Suction.level == 0) && (Time.error == 0)) // if suction is off , release the pressure through valve
        {
            IO_Power = 1;
            Display_off();
            /*   if (sensor.pressure < suction_release)
                Valve_open;
            else
            {
                Valve_close;
                IO_Power = 0;
                state = Power_down;
            } */

            release_pressure();
            IO_Power = 0;
            state = Power_down;
        }

        else
        {

            if (Time.blink == 1)
                LED1 |= 0b1110000;
            else
                LED1 &= 0b0000000;
            LED2 = 0b00000000;
            display(LED1, GIRD1);
            display(LED2, GIRD2);

        } //blind LED

        break;

    case Power_down:

        //   DeviceInit();

        Key.update = 0;
        Key.pressed = 0;
        Key.debounce = 0;
        Valve_close;
        Power_off;
        while (KEY_INT1 == 0)
            ;

        Display_off();

        EA = 1;
        EX1 = 1;
        EX0 = 0;

        //disable other key interrupt
        WAKE_CLKO &= 0xef; //disable other key interrupt
        WAKE_CLKO &= 0xdf; //disable other key interrupt
        PCON |= 0x02;      //sleep mode only power key can wake up

        WAKE_CLKO |= 0x20;
        WAKE_CLKO |= 0x10;

        EX1 = 1;
        EX0 = 1;

        Power_on;

        Display_on();

        delay_ms(30000);
        Display_off();

        Power_on;
        Key.update = 0;
        Key_pressed = 0;
        Suction.on = 1;
        Suction.level = 1;
        Battery.on = 1;
        Valve_close;
        Time.error = 1;

        state = Normal_mode;
        break;

    default:
        break;
    }
}