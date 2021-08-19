
#include <start.h>

void IO_handler(void)
{
if ((PWM.on == 1) && (Heating.on == 1))
            IO_PTC = 1;
        else
            IO_PTC = 0;


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
