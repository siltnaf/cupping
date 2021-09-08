#ifndef __START_H
#define __START_H
typedef unsigned char u8;
typedef unsigned int u16;
#define MAIN_Fosc 11059200L //晶振频率，每秒
#define Seril_Debug 0

#include "stc12.h"
#include "8051.h"
#include "delay.h"
#include "hx711.h"
#include "gn1616.h"
#include "init.h"
#include "interrupt.h"
#include "process.h"
#include "state.h"
#include "debug.h"

#if (Seril_Debug == 1)
#include "uart.h"
void UART1_Interrupt(void) __interrupt UART1_VECTOR;
#endif

//

#define Timer0_Reload (MAIN_Fosc /40)       //Timer 0 中断频率, 50 ms period
#define T2KHZ (65536 - MAIN_Fosc / 12 / 6000) //Timer2

#define IO_Power P1_5
#define IO_Pump P5_4
#define IO_PTC P5_5
#define IO_Vibration P3_0
#define IO_Valve P3_1

#define Max_key 4

#define Low_suction   0x58
#define Med_suction   0x38
#define High_suction  0x18

#define suction_bound 0x10
#define suction_release 0x78

#define Hz_50  32    //5 for 10ms which is half period of 50 Hz
#define Hz_30  52     //8  for 16ms which is half period of 30Hz
#define Hz_20  80    //13 for 25 ms which is half period of 20Hz

//Sensor_init
typedef struct HTI_sensor
{
unsigned char pressure;
unsigned char temperature;
unsigned char pressure_inrange:1;
unsigned char temperature_inrange:1;
}AD_sensor;

typedef struct hx711
{
unsigned char P128 ;
unsigned char T32;
unsigned char P64;/*channel data */
}channel;


//自定义一个int结构体位域来解析按键：单次，双击，长按，等功能
typedef struct Button_Setting
{
    unsigned char update : 1;
    unsigned char pressed : 1;
    unsigned char long_press_state : 1;
   
    unsigned char debounce;
    unsigned char which_press;

} Button_Status;

//program setting
typedef struct Program_Setting
{
    unsigned char on : 1;
    unsigned char level : 3;
    unsigned char output:1;
    unsigned char timer ;
    unsigned char duty ;


} Level;

//define 4 buttons
typedef enum Button_type
{
    Key_Power = 0x01,     //0x00
    Key_Pump = 0x02,      //0x01
    Key_Vibration = 0x03, //0x02
    Key_PTC = 0x04        //0x03

} Button_type;



typedef enum Treatment_time
{
    Time0 = 0,
    Time1 = 5,
    Time2 = 5,
    Time3 = 5

} Treatment_time;






typedef struct Timer_Setting
{
   
    unsigned char sec;
    unsigned char min;
    unsigned char count;
    unsigned char Hzcount;
    unsigned char Hzmax;
    unsigned char update : 1;
    unsigned char PWM : 1;
    unsigned char reading:1;
    unsigned char Hzout:1;
  

} Timer_Status;

void Start(void);
extern void Dump_value(u8 val);

//初始化按键
extern Button_Status Key;
extern Level Power, Vibration, Suction, Heating;
extern Button_type Key_pressed;
 
extern Timer_Status Time;
extern AD_sensor sensor;
extern Treatment_time duration;

extern channel hx711channel,*hxsensor;
extern unsigned long temperature[];

#endif
