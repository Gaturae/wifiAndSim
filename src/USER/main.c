#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"  
#include "key.h"     
#include "usmart.h"  
#include "malloc.h"
#include "mmc_sd.h"  
#include "flash.h"    
#include "ff.h"  
#include "exfuns.h"   
#include "text.h"
#include "usart2.h"
#include "touch.h"
#include "common.h"

//ALIENTEK战舰STM32开发板扩展实验6
//ATK-ESP8266 WIFI模块测试实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 

int idlecnt=0;
int workcnt=0;
int checkcnt=10;


int main(void)
{
	//初始化配置
	u8 *Message; 
	u8 *data;   //将Uart1和Uart2的接受寄存器分开
	u8 MacVaule[17];
	u8 GPRS_IMEI[17];  
	u8 wifi_isconnect=0;
	char i[10];    //终端上发服务器message的容器
	int k=0;	
	u8 key,fontok=0; 
	u8 t;
	u8 a[5] = {1,2,3,4,5};
	
  Stm32_Clock_Init(9);	//系统时钟设置
	delay_init(72);			//延时初始化
	uart_init(72,19200); 	//串口1初始化 
	USART2_Init(36,115200);  //串口2初始化
	GPIO_Init();     //IO初始化

	//功能初始化代码段
	
		//wifi模块代码
		//esp8266_init(MacVaule);
		

		//sim900a模块代码
		sim900a_init(GPRS_IMEI);

	
	//wifiSmart模块代码
		//	esp8266_smart_init(MacVaule);
	
		while(1)
		{
				//wifi模块代码

				
			
//				Message = device_msg_back(1);
//				dev_to_ser(Message,MacVaule);
//				data = service_to_device(1);			 //接受服务器发的数据然后发向按摩椅板子
//				delay_s(1);	
//			
//				if(k==0)
//				{
//					PC6_Led(1);
//					k=1;
//				}
//				else
//				{
//					
//					PC6_Led(0);
//					k=0;
//				}
//				
//				checkcnt--;
//				if(checkcnt==0)
//				{
//					checkcnt=30;
//					while(atk_8266_quit_trans()!=0);//退出透传		
//					if(atk_8266_send_cmd("AT+CIPSTATUS","TCP",200)==0)
//					{					
//							atk_8266_send_cmd("AT+CIPSEND","",200);
//					}
//					else
//					{
//					
//							esp8266_init(MacVaule);
//			
//					}
//				}
		

				
//				PC6_Led(1);
//				delay_ms(500);
//				PC6_Led(0);
//				delay_ms(500);
//				if(wifiMode())
//				{
//					
//					PC6_Led(1);
//				
//				
//				}
//				else
//				{
//					
//					PC6_Led(0);
//				
//				}
				
				
				
				
				
				
				
				

			//sim900a模块代码
		
			Message = device_msg_back(1);
			if(StringGet(data,"CLOSED") == 0)
			{
				delay_s(1);				
				dev_to_ser_gprs(Message,GPRS_IMEI);
				data = service_to_device(1);			//接受服务器发的数据然后发向按摩椅板子

			}
			else
			{
				delay_s(10);
				sim900a_init(GPRS_IMEI);		
			}

			

		}
	
	}

	
	
//------------code tomb--------------------//////	
//			i[0] = *(++data);
//			i[1] = *(++data);
//			i[2] = *data;				
//			if(StringCmp(i, "+++") == 0)
//			{
//					atk_8266_quit_trans();//退出透传
//					atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //关闭透传模式	
//					atk_8266_send_cmd("AT+CIPCLOSE","",200);  //关闭TCP	
//					delay_ms(800);
//			
//			}
