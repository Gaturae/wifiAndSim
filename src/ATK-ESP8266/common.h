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
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//ATK-ESP8266 WIFIģ�� ������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/4/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
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



u8 atk_8266_apsta_test(void);	//WIFI AP+STAģʽ����
u8 atk_8266_wifista_test(void);	//WIFI STA����
u8 atk_8266_wifiap_test(void);	//WIFI AP����

//�û����ò���
extern const u8* portnum;			//���Ӷ˿�
 
extern const u8* wifista_ssid;		//WIFI STA SSID
extern const u8* wifista_encryption;//WIFI STA ���ܷ�ʽ
extern const u8* wifista_password; 	//WIFI STA ����

extern const u8* wifiap_ssid;		//WIFI AP SSID
extern const u8* wifiap_encryption;	//WIFI AP ���ܷ�ʽ
extern const u8* wifiap_password; 	//WIFI AP ����

extern u8* kbd_fn_tbl[2];
extern const u8* ATK_ESP8266_CWMODE_TBL[3];
extern const u8* ATK_ESP8266_WORKMODE_TBL[3];
extern const u8* ATK_ESP8266_ECN_TBL[5];
#endif

//��������
void esp8266_init(u8 MacVaule[17]);    //ESP8266������������
void Esp8226TcpLoad();
char StringCmp(char*i,char*j);
char CmdCmp(u8*i,char*j,int n);
u8 atk_8266_send_cmd_back(u8 *cmd,u8 *ack,u16 waittime,u8 MacName[6],u8 MacVaule[11]);
void SendTcpHello(u8 MacName[6],u8 MacVaule[17]);
int StringGet(u8 *data,char *keyword);
u8* device_msg_back(u8 mode);     //�����ն��ϱ�����������message
void macPrint(u8 MacVaule[17]);
void dev_to_ser(u8 *Message,u8 MacVaule[17]);   //�Ϸ�����������
u8* service_to_device(u8 mode);      //�·��豸message
void delay_s(u16 s);
u8* atk_gprs_check_cmd(u8 *str);   //gprs�Ĵ��ڽ��ܺ���
u8* atk_gprs_send_cmd_back(u8 *cmd,u8 *ack,u16 waittime);
void GPRS_send_cmd();
void GPRS_send_OK();
u8 sim900a_chr2hex(u8 chr);
void usart3_init(u32 pclk2,u32 bound);
void sim900a_init(u8 GPRS_IMEI[17]);
void IMEIprint(u8 GPRS_IMEI[17]);
void dev_to_ser_gprs(u8 *Message,u8 GPRS_IMEI[17]);
u8* clearmem(u8* data);   //��յ�ַ�ռ�������
void esp8266_smart_init(u8 MacVaule[17]);

//��������
extern  int workcnt;
extern  int idlecnt;
extern  int checkcnt;
