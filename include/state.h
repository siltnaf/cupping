#ifndef __STATE_MACHINE_H
#define __STATE_MACHINE_H

#include <start.h>

//State machine
typedef enum State_name
{
    Normal_mode,
    Charging_mode,
    Timer_end,
    Power_down,
    BT_mode

} State_name;



void state_machine(void);

extern State_name state;

#endif