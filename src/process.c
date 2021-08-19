
#include <start.h>

void IO_handler(void)
{
if ((PWM.on == 1) && (Heating.on == 1))
            IO_PTC = 1;
        else
            IO_PTC = 0;


}
