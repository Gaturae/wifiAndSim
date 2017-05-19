#ifndef __COMMON_H__
#define __COMMON_H__	 
#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h"   	 
#include "key.h"	 	 	 	 	 
#include "lcd.h"  	  
#include "flash.h" 	 
#include "touch.h" 	 
#include "malloc.h"
#include "string.h"    
#include "text.h"		
#include "usart2.h" 
#include "ff.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//ATK-ESP8266 WIFI模块 公用驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/4/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
	

/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void atk_8266_init(void);

u8 atk_8266_mode_cofig(u8 netpro);
u8* atk_8266_at_response(u8 mode);
u8* atk_8266_check_cmd(u8 *str);
u8 atk_8266_apsta_check(void);
u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime);
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8 atk_8266_quit_trans(void);
u8 atk_8266_consta_check(void);
void atk_8266_load_keyboard(u16 x,u16 y);
void atk_8266_key_staset(u16 x,u16 y,u8 keyx,u8 sta);
u8 atk_8266_get_keynum(u16 x,u16 y);
void atk_8266_get_wanip(u8* ipbuf);
void atk_8266_get_ip(u8 x,u8 y);
void atk_8266_msg_show(u16 x,u16 y,u8 wanip);
void atk_8266_wificonf_show(u16 x,u16 y,u8* rmd,u8* ssid,u8* encryption,u8* password);
u8 atk_8266_netpro_sel(u16 x,u16 y,u8* name);
void atk_8266_mtest_ui(u16 x,u16 y);

u8 atk_8266_ip_set(u8* title,u8* mode,u8* port,u8* ip);
void atk_8266_test(void);



u8 atk_8266_apsta_test(void);	//WIFI AP+STA模式测试
u8 atk_8266_wifista_test(void);	//WIFI STA测试
u8 atk_8266_wifiap_test(void);	//WIFI AP测试

//用户配置参数
extern const u8* portnum;			//连接端口
 
extern const u8* wifista_ssid;		//WIFI STA SSID
extern const u8* wifista_encryption;//WIFI STA 加密方式
extern const u8* wifista_password; 	//WIFI STA 密码

extern const u8* wifiap_ssid;		//WIFI AP SSID
extern const u8* wifiap_encryption;	//WIFI AP 加密方式
extern const u8* wifiap_password; 	//WIFI AP 密码

extern u8* kbd_fn_tbl[2];
extern const u8* ATK_ESP8266_CWMODE_TBL[3];
extern const u8* ATK_ESP8266_WORKMODE_TBL[3];
extern const u8* ATK_ESP8266_ECN_TBL[5];
#endif

//新增函数
void esp8266_init(u8 MacVaule[17]);    //ESP8266启动连接外网
void Esp8226TcpLoad();
char StringCmp(char*i,char*j);
char CmdCmp(u8*i,char*j,int n);
u8 atk_8266_send_cmd_back(u8 *cmd,u8 *ack,u16 waittime,u8 MacName[6],u8 MacVaule[11]);
void SendTcpHello(u8 MacName[6],u8 MacVaule[17]);
int StringGet(u8 *data,char *keyword);
u8* device_msg_back(u8 mode);     //接收终端上报到服务器的message
void macPrint(u8 MacVaule[17]);
void dev_to_ser(u8 *Message,u8 MacVaule[17]);   //上发服务器函数
u8* service_to_device(u8 mode);      //下发设备message
void delay_s(u16 s);
u8* atk_gprs_check_cmd(u8 *str);   //gprs的串口接受函数
u8* atk_gprs_send_cmd_back(u8 *cmd,u8 *ack,u16 waittime);
void GPRS_send_cmd();
void GPRS_send_OK();
u8 sim900a_chr2hex(u8 chr);
void usart3_init(u32 pclk2,u32 bound);
void sim900a_init(u8 GPRS_IMEI[17]);
void IMEIprint(u8 GPRS_IMEI[17]);
void dev_to_ser_gprs(u8 *Message,u8 GPRS_IMEI[17]);
u8* clearmem(u8* data);   //清空地址空间内数据
void esp8266_smart_init(u8 MacVaule[17]);

//新增参数
extern  int workcnt;
extern  int idlecnt;
extern  int checkcnt;
