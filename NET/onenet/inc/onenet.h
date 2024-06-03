#ifndef _ONENET_H_
#define _ONENET_H_
#include "sys.h"
#include "delay.h"

typedef struct 
{
	u8 temp;
	u8 humi;
	u16 mq2;
	u8 lsens;
	u8 temp_l;
	u8 temp_h;
	u8 humi_y;
	u8 lsens_y;
	u16 mq2_y;
	u8 mode_flag;
	u8 led_flag;
	u8 fs_flag;
	u8 jr_flag;
}data;


_Bool OneNet_DevLink(void);

void OneNet_SendData(void);

void OneNET_Subscribe(void);

void OneNet_RevPro(unsigned char *cmd);

#endif
