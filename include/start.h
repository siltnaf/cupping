#ifndef __START_H
#define __START_H
typedef unsigned char u8;
typedef unsigned int	u16;
#define MAIN_Fosc 11059200L //晶振频率，每秒
#define Seril_Debug 0

#include "stc12.h"
#include "8051.h"

#include "delay.h"
#include "hx711.h"
#include "gn1616.h"
#if(Seril_Debug==1) 
#include "uart.h"
void UART1_Interrupt(void)__interrupt UART1_VECTOR;
#endif
void DeviceInit(void);
void Start(void);
void Timer0_init(void);
void Timer2_init(void);
void Int0_init(void);
void Int1_init(void);
void Int2_init(void);
void Int3_init(void);
void LoadPWM(u16 i);
void KeyTimer(void) __interrupt TIMER0_VECTOR;
void PWMTimer(void) __interrupt TIMER2_VECTOR;
void INT0_int(void) __interrupt INT0_VECTOR;
void INT1_int(void) __interrupt INT1_VECTOR;
void INT2_int(void) __interrupt INT2_VECTOR;
void INT3_int(void) __interrupt INT3_VECTOR;
extern volatile char PWM_Time,PWM_Time,PWM_FLAG, PTC_FLAG, MOTOR_FLAG;
extern volatile u16 PWM_low,PWM_high,PWM;




#endif
