
#include <start.h>

State_name state;

void state_machine(void)
{

    switch (state)
    {
    case idle_mode:
        state = idle_mode;
        break;

    case breathe_mode:
        state = breathe_mode;
        break;

    case Timer_end:
        Heating.on = 0;
        Vibration.on = 0;
        Lock_pressure(Stay_suction);
        if ((Suction.level == 0)) // if suction is off , release the pressure through valve
        {
            IO_Power = 1;
            LED1 = 0;
            LED2 = 0;
            display(LED1, GIRD1);
            display(LED2, GIRD2);
            if (sensor.pressure < suction_release)
                Valve_open;
            else
            {
                Valve_close;
                IO_Power = 0;
                state = Power_down;
            }
        }
        else
        {

            if (Time.blink == 1)
                LED1 |= 0b0001000;
            else
                LED1 &= 0b0000000;
            LED2 = 0b00000000;
            display(LED1, GIRD1);
            display(LED2, GIRD2);

        } //blind LED

        break;

    case Power_down:

        DeviceInit();

        Key.update = 0;
        Key.pressed = 0;
        Key.debounce = 0;

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

        IO_Power = 1;
        LED1 = 0xff;
        LED2 = 0xff;

        display(LED2, GIRD2);
        display(LED1, GIRD1);

        delay_ms(30000);

        LED1 = 0x00;
        LED2 = 0x00;

        display(LED2, GIRD2);
        display(LED1, GIRD1);

        Power.on = 1;
        Power.level = 1;

        state = idle_mode;
        break;

    default:
        break;
    }
}