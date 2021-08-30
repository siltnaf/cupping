#ifndef __STATE_MACHINE_H
#define __STATE_MACHINE_H

#include <start.h>

//State machine
typedef enum State_name
{
    idle_mode,
    normal_mode,
    Timer_mode,
    Power_down,
    Power_on,
    BT_mode,

} State_name;



void state_machine(void);

extern State_name state;

#endif