#include "usart.h"
#include "usart2.h"
#include "delay.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化（适合STM32F10x系列）		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2010/1/1
//版本：V1.7
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
//V1.6修改说明 20150109
//uart_init函数去掉了开启PE中断
//V1.7修改说明 20150322
//修改OS_CRITICAL_METHOD宏判断为：SYSTEM_SUPPORT_OS
////////////////////////////////////////////////////////////////////////////////// 	  
 
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定向fputc函数
//printf的输出，指向fputc，由fputc输出到串口
//这里使用串口1(USART1)输出printf信息
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//等待上一次串口数据发送完成  
	USART1->DR = (u8) ch;      	//写DR,串口1将发送数据
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////

#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
  
void USART1_IRQHandler(void)
{
	u8 res;	
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
//	if(USART1->SR&(1<<5))	//接收到数据
//	{	 
//		res=USART1->DR; 
//		if((USART_RX_STA&0x8000)==0)//接收未完成
//		{
//			if(USART_RX_STA&0x4000)//接收到了0x0d
//			{
//				if(res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
//				else USART_RX_STA|=0x8000;	//接收完成了 
//			}else //还没收到0X0D
//			{	
//				if(res==0x0d)USART_RX_STA|=0x4000;
//				else
//				{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
//				}		 
//			}
//		}  		
//}
	if(USART1->SR&(1<<5))//接收到数据
	{	 
		res=USART1->DR; 			 
		if((USART_RX_STA&(1<<15))==0)		//还可以接收数据
		{
			TIM3->CNT=0;         					//计数器清空
			if(USART_RX_STA==0)TIM3_Set(1);	 	//使能定时器3的中断 
			USART1_RX_BUF[USART_RX_STA++]=res;		//记录接收到的值	 
		}else 
		{
			USART_RX_STA|=1<<15;					//强制标记接收完成
		} 
	}
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
}
#endif						

//串口发送缓存区 	
__align(8) u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 	//发送缓冲,最大USART1_MAX_SEND_LEN字节 	
//串口接收缓存区 	
u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; 				//接收缓冲,最大USART1_MAX_RECV_LEN个字节.


//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率 
void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB2ENR|=1<<14;  //使能串口时钟 
	GPIOA->CRH&=0XFFFFF00F;//IO状态设置
	GPIOA->CRH|=0X000008B0;//IO状态设置 
	RCC->APB2RSTR|=1<<14;   //复位串口1
	RCC->APB2RSTR&=~(1<<14);//停止复位	   	   
	//波特率设置
 	USART1->BRR=mantissa; // 波特率设置	 
	USART1->CR1|=0X200C;  //1位停止,无校验位.
#if EN_USART1_RX		  //如果使能了接收
	//使能接收中断 
	USART1->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//组2，最低优先级 
	TIM3_Init(999,7199);		//100ms中断
	USART_RX_STA=0;		//清零
	TIM3_Set(0);			//关闭定时器4	
	
#endif
}

//串口1,printf 函数
//确保一次发送数据不超过USART1_MAX_SEND_LEN字节
void u1_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART1_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART1_TX_BUF);		//此次发送数据的长度 
	for(j=0;j<i;j++)							//循环发送数据
	{
		while((USART1->SR&0X40)==0);			//循环发送,直到发送完毕   
		USART1->DR=USART1_TX_BUF[j];  
	}
}

//usmart支持部分
//将收到的指令应答数据返回给电脑串口
//mode:0,不清零USART_RX_STA;
//     1,清零USART_RX_STA;
u16 USART1_RX_STA=0; 
u8* u1_response(u8 mode)
{
	u8* data;
	if(USART_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART1_RX_BUF[USART_RX_STA&0X7FFF]=0;//添加结束符
		u2_printf("%s",USART1_RX_BUF);
		if(mode)USART_RX_STA=0;
	} 
	return data;
}	

void TIM3_IRQHandler(void)
{ 	
	if(TIM3->SR&0X01)//是更新中断
	{	 			   
		USART_RX_STA|=1<<15;	//标记接收完成
		TIM3->SR&=~(1<<0);		//清除中断标志位		   
		TIM3_Set(0);			//关闭TIM4  
	}	    
}
//设置TIM4的开关
//sta:0，关闭;1,开启;
void TIM3_Set(u8 sta)
{
	if(sta)
	{
    TIM3->CNT=0;         //计数器清空
		TIM3->CR1|=1<<0;     //使能定时器4
	}else TIM3->CR1&=~(1<<0);//关闭定时器4	   
}
//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM3_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;	//TIM4时钟使能    
 	TIM3->ARR=arr;  	//设定计数器自动重装值   
	TIM3->PSC=psc;  	//预分频器
 	TIM3->DIER|=1<<0;   //允许更新中断				
 	TIM3->CR1|=0x01;  	//使能定时器4	  	   
  MY_NVIC_Init(1,2,TIM3_IRQn,2);//抢占2，子优先级2，组2	在2中优先级最低								 
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

