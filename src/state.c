
#include <start.h>

State_name state;

void state_machine(void)
{

    switch (state)
    {
    case idle_mode:

        state = idle_mode;
        break;

    case normal_mode:
        state = normal_mode;
        break;

    case Timer_mode:

        break;

    case Power_down:
 
        
        DeviceInit();
        Key.lock=0;
        Key.update=0;
        Key.pressed=0;
        Key.debounce=0;
 
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

        delay_ms(30000);
       
        Power.on = 1;
        Power.level = 1;

        state = idle_mode;
        break;

    case Power_on:
        IO_Power = 1;

        state = idle_mode;
        break;

    default:
        break;
    }
}