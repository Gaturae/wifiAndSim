#include "usart.h"
#include "usart2.h"
#include "delay.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ�����ʺ�STM32F10xϵ�У�		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2010/1/1
//�汾��V1.7
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
//V1.6�޸�˵�� 20150109
//uart_init����ȥ���˿���PE�ж�
//V1.7�޸�˵�� 20150322
//�޸�OS_CRITICAL_METHOD���ж�Ϊ��SYSTEM_SUPPORT_OS
////////////////////////////////////////////////////////////////////////////////// 	  
 
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc����
//printf�������ָ��fputc����fputc���������
//����ʹ�ô���1(USART1)���printf��Ϣ
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//�ȴ���һ�δ������ݷ������  
	USART1->DR = (u8) ch;      	//дDR,����1����������
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////

#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
  
void USART1_IRQHandler(void)
{
	u8 res;	
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
//	if(USART1->SR&(1<<5))	//���յ�����
//	{	 
//		res=USART1->DR; 
//		if((USART_RX_STA&0x8000)==0)//����δ���
//		{
//			if(USART_RX_STA&0x4000)//���յ���0x0d
//			{
//				if(res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
//				else USART_RX_STA|=0x8000;	//��������� 
//			}else //��û�յ�0X0D
//			{	
//				if(res==0x0d)USART_RX_STA|=0x4000;
//				else
//				{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//				}		 
//			}
//		}  		
//}
	if(USART1->SR&(1<<5))//���յ�����
	{	 
		res=USART1->DR; 			 
		if((USART_RX_STA&(1<<15))==0)		//�����Խ�������
		{
			TIM3->CNT=0;         					//���������
			if(USART_RX_STA==0)TIM3_Set(1);	 	//ʹ�ܶ�ʱ��3���ж� 
			USART1_RX_BUF[USART_RX_STA++]=res;		//��¼���յ���ֵ	 
		}else 
		{
			USART_RX_STA|=1<<15;					//ǿ�Ʊ�ǽ������
		} 
	}
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
}
#endif						

//���ڷ��ͻ����� 	
__align(8) u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 	//���ͻ���,���USART1_MAX_SEND_LEN�ֽ� 	
//���ڽ��ջ����� 	
u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; 				//���ջ���,���USART1_MAX_RECV_LEN���ֽ�.


//��ʼ��IO ����1
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������ 
void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	RCC->APB2ENR|=1<<14;  //ʹ�ܴ���ʱ�� 
	GPIOA->CRH&=0XFFFFF00F;//IO״̬����
	GPIOA->CRH|=0X000008B0;//IO״̬���� 
	RCC->APB2RSTR|=1<<14;   //��λ����1
	RCC->APB2RSTR&=~(1<<14);//ֹͣ��λ	   	   
	//����������
 	USART1->BRR=mantissa; // ����������	 
	USART1->CR1|=0X200C;  //1λֹͣ,��У��λ.
#if EN_USART1_RX		  //���ʹ���˽���
	//ʹ�ܽ����ж� 
	USART1->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//��2��������ȼ� 
	TIM3_Init(999,7199);		//100ms�ж�
	USART_RX_STA=0;		//����
	TIM3_Set(0);			//�رն�ʱ��4	
	
#endif
}

//����1,printf ����
//ȷ��һ�η������ݲ�����USART1_MAX_SEND_LEN�ֽ�
void u1_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART1_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART1_TX_BUF);		//�˴η������ݵĳ��� 
	for(j=0;j<i;j++)							//ѭ����������
	{
		while((USART1->SR&0X40)==0);			//ѭ������,ֱ���������   
		USART1->DR=USART1_TX_BUF[j];  
	}
}

//usmart֧�ֲ���
//���յ���ָ��Ӧ�����ݷ��ظ����Դ���
//mode:0,������USART_RX_STA;
//     1,����USART_RX_STA;
u16 USART1_RX_STA=0; 
u8* u1_response(u8 mode)
{
	u8* data;
	if(USART_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART1_RX_BUF[USART_RX_STA&0X7FFF]=0;//��ӽ�����
		u2_printf("%s",USART1_RX_BUF);
		if(mode)USART_RX_STA=0;
	} 
	return data;
}	

void TIM3_IRQHandler(void)
{ 	
	if(TIM3->SR&0X01)//�Ǹ����ж�
	{	 			   
		USART_RX_STA|=1<<15;	//��ǽ������
		TIM3->SR&=~(1<<0);		//����жϱ�־λ		   
		TIM3_Set(0);			//�ر�TIM4  
	}	    
}
//����TIM4�Ŀ���
//sta:0���ر�;1,����;
void TIM3_Set(u8 sta)
{
	if(sta)
	{
    TIM3->CNT=0;         //���������
		TIM3->CR1|=1<<0;     //ʹ�ܶ�ʱ��4
	}else TIM3->CR1&=~(1<<0);//�رն�ʱ��4	   
}
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM3_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;	//TIM4ʱ��ʹ��    
 	TIM3->ARR=arr;  	//�趨�������Զ���װֵ   
	TIM3->PSC=psc;  	//Ԥ��Ƶ��
 	TIM3->DIER|=1<<0;   //��������ж�				
 	TIM3->CR1|=0x01;  	//ʹ�ܶ�ʱ��4	  	   
  MY_NVIC_Init(1,2,TIM3_IRQn,2);//��ռ2�������ȼ�2����2	��2�����ȼ����								 
}	


void usart3_init(u32 pclk2,u32 bound)
{    
 float temp;
 u16 mantissa;
 u16 fraction;    
 temp=(float)(pclk2*1000000)/(bound*16);//??USARTDIV
 mantissa=temp;               //??????
 fraction=(temp-mantissa)*16;           //??????  
    mantissa<<=4;
 mantissa+=fraction;
 RCC->APB2ENR|=1<<3;  //??PORTB???
 RCC->APB1ENR|=1<<18;    //????3??
 GPIOB->CRH&=0XFFFF00FF; //???
 GPIOB->CRH|=0X00008B00; //PB11???????,PB10?????
 RCC->APB1RSTR|=1<<18; //????3
 RCC->APB1RSTR&=~(1<<18);//????
    USART3->BRR=mantissa;
 USART3->CR1|=0X200C;
}

