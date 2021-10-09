#ifndef __PROCESS_H
#define __PROCESS_H
#include <start.h>


void Time_handler(void);
void Key_handler(void);
void Key_Setting_handler(void);
void Display_handler(void);
void Display_off(void);
void Display_on(void);
void service(void);
void Lock_pressure(unsigned char keep_pressure);
void release_pressure(void);
extern unsigned char LED1,LED2;

#endif