#include "key.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//�������� ��������		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/06
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									   
//////////////////////////////////////////////////////////////////////////////////	 
 	    
//������ʼ������ 
//PA0.15��PC5 ���ó�����
void KEY_Init(void)
{
	RCC->APB2ENR|=1<<2;     //ʹ��PORTAʱ��
	RCC->APB2ENR|=1<<4;     //ʹ��PORTCʱ��
	JTAG_Set(SWD_ENABLE);	//�ر�JTAG,����SWD
	GPIOA->CRL&=0XFFFFFFF0;	//PA0���ó�����	  
	GPIOA->CRL|=0X00000008;   
	GPIOA->CRH&=0X0FFFFFFF;	//PA15���ó�����	  
	GPIOA->CRH|=0X80000000; 			 
	GPIOA->ODR|=1<<15;	   	//PA15����,PA0Ĭ������
	GPIOC->CRL&=0XFF0FFFFF;	//PC5���ó�����	  
	GPIOC->CRL|=0X00800000;   
	GPIOC->ODR|=1<<5;	   	//PC5���� 
} 

void GPIO_Init()
{
	
	RCC->APB2ENR|=1<<3;     //ʹ��PORTBʱ��
	RCC->APB2ENR|=1<<4;     //ʹ��PORTCʱ��


	GPIOC->CRL&=0XF0FFFFFF;	//PC6���ó����	  
	GPIOC->CRL|=0X03000000;   
	//GPIOC->ODR|=1<<6;	   	//PC6���� 
	
	
	GPIOB->CRH&=0XFF0F00FF;	//PB11,PB13���ó����	PB10����  
	GPIOB->CRH|=0X00303800;   
	GPIOB->ODR|=1<<11;	   	//PB11���� 
	GPIOB->ODR|=1<<13;	   	//PB13����
	//GPIOB->ODR&=0XFBFF;	   	//PB10Ĭ������ 
	
}

char PC6_Led(char value)
{

		if(value)
		{
			
				GPIOC->BRR|=1<<6; 	//��0 ����
			
		}
		else
		{
		
				GPIOC->BSRR|=1<<6;	  //��1 ����

		}


}

char wifiMode()
{

		if(GPIOB->IDR&0x0400)
		{
			return 1;					//PB10��⵽�ߵ�ƽ
		}
		else
		{
			
			return 0;					
		
		}


}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//����ֵ��
//0��û���κΰ�������
//KEY0_PRES��KEY0����
//KEY1_PRES��KEY1����
//WKUP_PRES��WK_UP���� 
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>WK_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
		else if(WK_UP==1)return WKUP_PRES; 
	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1; 	     
	return 0;// �ް�������
}




















