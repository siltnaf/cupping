#ifndef __STATE_MACHINE_H
#define __STATE_MACHINE_H

#include <start.h>

//State machine
typedef enum State_name
{
    Idle_mode,
    Breathe_mode,
    Timer_end,
    Power_down,
    BT_mode

} State_name;



void state_machine(void);

extern State_name state;

#endif