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

//ALIENTEKս��STM32��������չʵ��6
//ATK-ESP8266 WIFIģ�����ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 

int idlecnt=0;
int workcnt=0;
int checkcnt=10;


int main(void)
{
	//��ʼ������
	u8 *Message; 
	u8 *data;   //��Uart1��Uart2�Ľ��ܼĴ����ֿ�
	u8 MacVaule[17];
	u8 GPRS_IMEI[17];  
	u8 wifi_isconnect=0;
	char i[10];    //�ն��Ϸ�������message������
	int k=0;	
	u8 key,fontok=0; 
	u8 t;
	u8 a[5] = {1,2,3,4,5};
	
  Stm32_Clock_Init(9);	//ϵͳʱ������
	delay_init(72);			//��ʱ��ʼ��
	uart_init(72,19200); 	//����1��ʼ�� 
	USART2_Init(36,115200);  //����2��ʼ��
	GPIO_Init();     //IO��ʼ��

	//���ܳ�ʼ�������
	
		//wifiģ�����
		//esp8266_init(MacVaule);
		

		//sim900aģ�����
		sim900a_init(GPRS_IMEI);

	
	//wifiSmartģ�����
		//	esp8266_smart_init(MacVaule);
	
		while(1)
		{
				//wifiģ�����

				
			
//				Message = device_msg_back(1);
//				dev_to_ser(Message,MacVaule);
//				data = service_to_device(1);			 //���ܷ�������������Ȼ����Ħ�ΰ���
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
//					while(atk_8266_quit_trans()!=0);//�˳�͸��		
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
				
				
				
				
				
				
				
				

			//sim900aģ�����
		
			Message = device_msg_back(1);
			if(StringGet(data,"CLOSED") == 0)
			{
				delay_s(1);				
				dev_to_ser_gprs(Message,GPRS_IMEI);
				data = service_to_device(1);			//���ܷ�������������Ȼ����Ħ�ΰ���

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
//					atk_8266_quit_trans();//�˳�͸��
//					atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //�ر�͸��ģʽ	
//					atk_8266_send_cmd("AT+CIPCLOSE","",200);  //�ر�TCP	
//					delay_ms(800);
//			
//			}
