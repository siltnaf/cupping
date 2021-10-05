
#include <start.h>
unsigned char PWM_ON; //定义按键时间
#define	Timer0_Reload	(MAIN_Fosc / 100)//Timer 0 中断频率, 100次/秒
#define Power_switch P1_5
#define Pump_motor P5_4
#define  PTC P5_5
#define  Vibration_motor P5_4
#define  Valve P5_5
unsigned char unicode_number[4]={0};
unsigned char consumer_number[]=""; 
unsigned char button_number[]="";
//hx711通道；

 const unsigned char A128=24 ;
 const unsigned char B32=25 ;
const unsigned char A64=26 ;/*channel data */

 //自定义枚举4个按键按下时的值；发热键，电源键，气泵键，振动键
enum Key
    {
        Power_button,//0x00
        Pump_button,//0x01
        Vibration_button,//0x02
        PTC_button//0x03
     
    }Key_pressed;
//自定义一个int结构体位域来解析按键：单次，双击，长按，等功能
typedef struct button
{
unsigned int stat:1;
unsigned int times:2;
unsigned int which_press:8;
unsigned int long_press:4;
unsigned int long_press_state:1;
}HTI_button;

//Sensor_init
struct sensor
{
unsigned long pressure;
unsigned long ntc;
}HTI_sensor;

//初始化按键
HTI_button copping_button;
void DeviceInit(void)
{
//变量初始化
    Pump_motor=0;
    Vibration_motor=1;
    PTC=0;
    Valve=1;
    Power_switch=0;
    copping_button.stat=0;
PWM_ON=0;//按键延时
TR0 = 0;	//停止计数
ET0 = 0;	//停止计数中断
}

void Start(void)
{

	P0M1 = 0;	P0M0 = 0;	//设置为准双向口
	P1M1 = 0;	P1M0 = 0x30;	//设置为准双向口
	P2M1 = 0;	P2M0 = 0;	//设置为准双向口
    P3M1 = 0;	P3M0 = 0x03;	//设置为准双向口
	P4M1 = 0;	P4M0 = 0;	//设置为准双向口
	P5M1 = 0;	P5M0 = 0x30;	//设置为准双向口
    	
Int0_init();
Int1_init();
Int2_init();
Int3_init();

display(0x00,GIRD2);

#if(Serial_Debug==1) 
Send1_String("STC15W204S\r\nUart is ok !\r\n");//发送字符串检测是否初始化成功
Send1_String("gn1616_start\r\ndelay_ms(1000)!\r\n");//发送字符串检测是否初始化成功
#endif
EA=0;
DeviceInit();
EA=1;

    while (1) {

     HTI_sensor.pressure=(HX711_Read(25)&0x00ffffff);

       gn1616_ms(500);
       unicode_number[3]=HTI_sensor.pressure;
      // send1_Byte(unicode_number[0]);
       unicode_number[2]=HTI_sensor.pressure>>8;
    //send1_Byte(unicode_number[1]);
       unicode_number[1]=HTI_sensor.pressure>>16;
       // send1_Byte(unicode_number[2]);
       unicode_number[0]=HTI_sensor.pressure>>24;
        //  send1_Byte(unicode_number[3]);
       HexToAscii(unicode_number, consumer_number, 4);
       Send1_String("\r\nL+Hpressure=");//发送气压传感器 低 8位
       Send1_String(consumer_number);

       if(copping_button.stat)//按键中断flag;
       {
        EA=0;
        copping_button.stat=0;
        copping_button.which_press=Key_pressed;
      
        #if(Serial_Debug==1) 
        //这里定义的数组是将16进制的数转换为字符输出，方便打印调试
        Send1_String("\r\nButton=");//发送按键值
        button_number[0]=copping_button.which_press;
        HexToAscii(button_number, button_number, 1); 
        Send1_String(button_number);

        #endif

 
        
           switch(copping_button.which_press)
           {
        
            case PTC_button:
              // PTC_LED=!PTC_LED;
            
               PTC=!PTC;//启动PTC加热
               if(PTC) display(0xf0,GIRD1);
                else display(0x00,GIRD1);
               break;
            case Vibration_button:
              // Vibration_LED=!Vibration_LED;
               Vibration_motor=!Vibration_motor;
               if(!Vibration_motor)
               display(0x0f,GIRD1);
               else display(0x00,GIRD1);  /* code */
               break;
            case Pump_button:
              // Pump_LED=!Pump_LED;
              
               Pump_motor=!Pump_motor;//启动气泵开关
               if(Pump_motor) display(0xf0,GIRD2);
               else display(0x00,GIRD2);
               break;
            case Power_button:
               //Power_LED=!Power_LED;
               display(0x0f,GIRD2);
               Valve=!Valve;
               if(!Valve)display(0x0f,GIRD2);
               else display(0x00,GIRD2);
               break;
                // -------------------------------
               // Default event handler.
              default:
              break;
           }
           /*
           switch(copping_button.times)
           {
               case 1:
               
               break;
               case 2:
               
               break;
               case 3:
               
               break;

                // -------------------------------
               // Default event handler.
              default:
              break;
           }
           */

       }
  if(copping_button.long_press_state)//如果检测到长时间按键则打开电源及升压
  {
  EA=0;
Pump_motor=0;
copping_button.long_press_state=0;//reset press long time flag;
Power_switch=!Power_switch;//trun on the power switch;
if(Power_switch)
{display(0xff,GIRD1);display(0xff,GIRD2);}
else 
{
display(0x00,GIRD1);
display(0x00,GIRD2);
}
#if(Serial_Debug==1) 
Send1_String("long_press_state=1\r\n");//发送字符串检测是否初始化成功
#endif

  }
  EA=1;
    }//
    

    
}

void KeyTimer(void) __interrupt TIMER0_VECTOR
{

if((!INT0)||(!INT1)||(!INT2)||(!INT3))
PWM_ON++;
else
{
copping_button.stat=0;
PWM_ON=0;
TR0 = 0;	//停止计数
ET0 = 0;	//停止计数中断
copping_button.long_press=0;
}
   
if(PWM_ON%20==0)
{
if((!INT0)||(!INT1)||(!INT2)||(!INT3))
copping_button.long_press++;
else
{
copping_button.times++;
    copping_button.stat=1;
   // PTC_LED=!PTC_LED;
    copping_button.long_press=0;
    copping_button.long_press_state=0;
    PWM_ON=0;    
    TR0 = 0;	//停止计数
    ET0 = 0;	//停止计数中断
     }
if(copping_button.long_press>3) 
{
    copping_button.times++;
    copping_button.stat=0;
       //  Pump_LED=!Pump_LED;
     copping_button.long_press=0;
     copping_button.long_press_state=1;
    PWM_ON=0;    
    TR0 = 0;	//停止计数
    ET0 = 0;	//停止计数中断
}

     }
   
  //if(copping_button.long_press>30)
           // PTC_LED=!PTC_LED;
          //  LED1 = 0;
           // LED2 = 1; //灯灭

}

/********************* INT0中断函数 *************************/
void INT0_int(void)__interrupt INT0_VECTOR		//进中断时已经清除标志
{
    Timer0_init();//启动定时器：作用于消抖按键，判断按键功能
    copping_button.stat=0;
    Key_pressed=Power_button;
   // LED1=!LED1;
	//EX0 = 0;	//INT0 Disable
	//IE0 = 0;	//外中断0标志位
}

/********************* INT1中断函数 *************************/
void INT1_int(void)__interrupt INT1_VECTOR		//进中断时已经清除标志
{
    Timer0_init();//启动定时器：作用于消抖按键，判断按键功能
   copping_button.stat=0;
    Key_pressed=Pump_button;
    //LED2=!LED2;
	//EX1 = 0;	//INT1 Disable
	//IE1 = 0;	//外中断1标志位
}
/********************* INT2中断函数 *************************/
void INT2_int(void)__interrupt INT2_VECTOR		//进中断时已经清除标志
{
    Timer0_init();//启动定时器：作用于消抖按键，判断按键功能
   copping_button.stat=0;
    Key_pressed=Vibration_button;
   // LED2=!LED2;
	//EX1 = 0;	//INT1 Disable
	//IE1 = 0;	//外中断1标志位
}
/********************* INT2中断函数 *************************/
void INT3_int(void)__interrupt INT3_VECTOR		//进中断时已经清除标志
{
    Timer0_init();//启动定时器：作用于消抖按键，判断按键功能
    copping_button.stat=0;
    Key_pressed=PTC_button;
   // LED2=!LED2;
	//EX1 = 0;	//INT1 Disable
	//IE1 = 0;	//外中断1标志位
}
void Int0_init(void)
{
IE0  = 1;	//外中断0标志位
EX0 = 1;	//INT0 Enable
IT0 = 1;		//INT0 下降沿中断		
//	IT0 = 0;		//INT0 上升,下降沿中断		


}
void Int1_init(void)
{
IE1  = 1;	//外中断1标志位
EX1 = 1;	//INT1 Enable
IT1 = 1;		//INT1 下降沿中断		
//	IT1 = 0;		//INT1 上升,下降沿中断	
}
void Int2_init(void)
{
WAKE_CLKO |= 0x10;	//INT3 Enable 	//INT2 Enable
		//INT2 只能下降沿中断		

}
void Int3_init(void)
{
WAKE_CLKO |= 0x20;	//INT3 Enable
		//INT3 只能下降沿中断		
}

//========================================================================
// 函数: void	Timer0_init(void)
// 描述: timer0初始化函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2015-1-12
//========================================================================
void	Timer0_init(void)
{
		TR0 = 0;	//停止计数

	#if (Timer0_Reload < 64)	// 如果用户设置值不合适， 则不启动定时器
		#error "Timer0设置的中断过快!"

	#elif ((Timer0_Reload/12) < 65536UL)	// 如果用户设置值不合适， 则不启动定时器
		ET0 = 1;	//允许中断
	//	PT0 = 1;	//高优先级中断
		TMOD &= ~0x03;
		TMOD |= 0;	//工作模式, 0: 16位自动重装, 1: 16位定时/计数, 2: 8位自动重装, 3: 16位自动重装, 不可屏蔽中断
	//	TMOD |=  0x04;	//对外计数或分频
		TMOD &= ~0x04;	//定时
	//	INT_CLKO |=  0x01;	//输出时钟
		WAKE_CLKO &= ~0x01;	//不输出时钟

		#if (Timer0_Reload < 65536UL)
			AUXR |=  0x80;	//1T mode
			TH0 = (u8)((65536UL - Timer0_Reload) / 256);
			TL0 = (u8)((65536UL - Timer0_Reload) % 256);
		#else
			AUXR &= ~0x80;	//12T mode
			TH0 = (u8)((65536UL - Timer0_Reload/12) / 256);
			TL0 = (u8)((65536UL - Timer0_Reload/12) % 256);
		#endif

		TR0 = 1;	//开始运行

	#else
		#error "Timer0设置的中断过慢!"
	#endif
}