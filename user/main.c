//单片机头文件
#include "stm32f10x.h"

//网络协议层
#include "onenet.h"

//网络设备
#include "esp8266.h"

//硬件驱动
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "dht11.h"
#include "oled.h"
#include "adc.h"
#include "lsens.h"
#include "led.h"
#include "key.h"

//C库
#include <string.h>


#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"
extern data upload_data;


/*
************************************************************
*	函数名称：	Hardware_Init
*
*	函数功能：	硬件初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		初始化单片机功能以及外接设备
************************************************************
*/
void Hardware_Init(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断控制器分组设置
	Delay_Init();									//systick初始化
	Usart1_Init(115200);							//串口1，打印信息用
	Usart2_Init(115200);							//串口2，驱动ESP8266用
	OLED_Init();
	LED_Init();
	KEY_Init();
	Lsens_Init();
	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	
}

/*
************************************************************
*	函数名称：	main
*
*	函数功能：	
*
*	入口参数：	无
*
*	返回参数：	0
*
*	说明：		
************************************************************
*/
int main(void)
{
	
	unsigned short timeCount = 0;	//发送间隔变量
	
	unsigned char *dataPtr = NULL;
	u8 key=0;
	u8 set_flag=0;
	Hardware_Init();				//初始化外围硬件
	 while(DHT11_Init())
	{
		delay_ms(200);
	}
	OLED_ShowCHinese(20,0,17);
	OLED_ShowCHinese(36,0,18);
	OLED_ShowString(52,0,"WIFI");
	OLED_ShowCHinese(88,0,23);
	ESP8266_Init();					//初始化ESP8266
	OLED_Clear();
	OLED_ShowCHinese(20,0,17);
	OLED_ShowCHinese(36,0,18);
	OLED_ShowString(52,0,"WIFI");
	OLED_ShowCHinese(84,0,19);
	OLED_ShowCHinese(100,0,20);
	OLED_ShowCHinese(20,3,17);
	OLED_ShowCHinese(36,3,18);
	OLED_ShowCHinese(52,3,21);
	OLED_ShowCHinese(68,3,22);
	OLED_ShowCHinese(84,3,23);
	UsartPrintf(USART_DEBUG, "Connect MQTTs Server...\r\n");
	while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
		delay_ms(500);
	UsartPrintf(USART_DEBUG, "NET_OK\r\n");
	
	while(OneNet_DevLink())			//接入OneNET
		delay_ms(500);
	OneNET_Subscribe();
	 OLED_Clear();
	 OLED_ShowCHinese(0,0,0);  //温
	 OLED_ShowCHinese(18,0,2); //度
	
	 OLED_ShowCHinese(0,2,1); //湿
	 OLED_ShowCHinese(18,2,2); //度
	
	 OLED_ShowCHinese(0,4,5);//亮
	 OLED_ShowCHinese(18,4,6);//度

	 OLED_ShowCHinese(0,6,24);//烟
	 OLED_ShowCHinese(18,6,25);//雾	
	
	 OLED_ShowCHinese(90,2,9); //自
	 OLED_ShowCHinese(106,2,10);  //动

	 upload_data.mode_flag=0;
	 DHT11_Read_Data(&upload_data.temp,&upload_data.humi);	//读取温湿度值	
	 upload_data.mq2=Mq2_Get_Val();
	 upload_data.lsens=Lsens_Get_Val();
	 upload_data.temp_l=15;
	 upload_data.temp_h=35;
	 upload_data.humi_y=75;
	 upload_data.mq2_y=500;
	 upload_data.lsens_y=40;
	while(1)
	{
		key=KEY_Scan(0);
		switch(key)
		{
			case KEY0_PRES:
				upload_data.mode_flag=!upload_data.mode_flag;
			break;
			case KEY1_PRES:
				if(upload_data.mode_flag==1)
					upload_data.jr_flag=!upload_data.jr_flag;
				else
				{
					set_flag++;
					if(set_flag>4)
						set_flag=0;
				}
			break;
			case KEY2_PRES:
				if(upload_data.mode_flag==1)
					upload_data.fs_flag=!upload_data.fs_flag;
				else
				{
					if(set_flag==0)
						upload_data.temp_l++;
					else if(set_flag==1)
						upload_data.temp_h++;
					else if(set_flag==2)
						upload_data.humi_y++;				
					else if(set_flag==3)
						upload_data.lsens_y++;
					else if(set_flag==4)
						upload_data.mq2_y+=10;			
				}
			break;	
			case KEY3_PRES:
				if(upload_data.mode_flag==1)
					upload_data.led_flag=!upload_data.led_flag;
				else
				{
					if(set_flag==0)
						upload_data.temp_l--;
					else if(set_flag==1)
						upload_data.temp_h--;
					else if(set_flag==2)
						upload_data.humi_y--;				
					else if(set_flag==3)
						upload_data.lsens_y--;
					else if(set_flag==4)
						upload_data.mq2_y-=10;	
				}
			break;	
			
		}
		if(set_flag==0)
		{
			OLED_ShowString(90,4,"T_L");
		}
		else if(set_flag==1)
		{
			OLED_ShowString(90,4,"T_H");
		}
		else if(set_flag==2)
		{
			OLED_ShowString(90,4,"H_H");
		}
		else if(set_flag==3)
		{
			OLED_ShowString(90,4,"L_H");
		}
		else if(set_flag==4)
		{
			OLED_ShowString(90,4,"M_H");
		}
		
		
			DHT11_Read_Data(&upload_data.temp,&upload_data.humi);	//读取温湿度值	
			upload_data.mq2=Mq2_Get_Val();
			upload_data.lsens=Lsens_Get_Val();
			OLED_ShowNum(40,0,upload_data.temp,2,16);//显示温度		
			OLED_ShowNum(40,2,upload_data.humi,2,16);//显示湿度	
			OLED_ShowNum(40,4,upload_data.lsens,2,16);	//显示亮度	
			OLED_ShowNum(40,6,upload_data.mq2,3,16);	//显示烟雾	
		
			OLED_ShowNum(60,0,upload_data.temp_l,2,16);
			OLED_ShowChar(76,0,'-');
			OLED_ShowNum(84,0,upload_data.temp_h,2,16);
			OLED_ShowNum(70,2,upload_data.humi_y,2,16);
			OLED_ShowNum(70,4,upload_data.lsens_y,2,16);
			OLED_ShowNum(70,6,upload_data.mq2_y,3,16);
		if(upload_data.mode_flag==0)
		{
			 OLED_ShowCHinese(90,2,9); //自
			 OLED_ShowCHinese(106,2,10);  //动
			if(upload_data.temp<upload_data.temp_l)
				upload_data.jr_flag=1;
			else
				upload_data.jr_flag=0;
			if(upload_data.lsens<upload_data.lsens_y)
				upload_data.led_flag=1;
			else
				upload_data.led_flag=0;
			if(upload_data.temp>upload_data.temp_h||upload_data.humi>upload_data.humi_y||upload_data.mq2>upload_data.mq2_y)
				upload_data.fs_flag=1;
			else
				upload_data.fs_flag=0;
		}
		else
		{
			 OLED_ShowCHinese(90,2,11); //手
			 OLED_ShowCHinese(106,2,12);  //动
		}
		if(upload_data.led_flag==1)
		{
			GPIO_SetBits(GPIOB,led);
		}
		else
		{
			GPIO_ResetBits(GPIOB,led);
		}
		if(upload_data.fs_flag==1)
		{
			GPIO_SetBits(GPIOB,fs);
		}
		else
		{
			GPIO_ResetBits(GPIOB,fs);
		}
		if(upload_data.jr_flag==1)
		{
			GPIO_SetBits(GPIOB,jr);
		}
		else
		{
			GPIO_ResetBits(GPIOB,jr);
		}
		if(++timeCount >= 30)									//发送间隔5s
		{
			
			UsartPrintf(USART_DEBUG, "OneNet_SendData\r\n");
			OneNet_SendData();									//发送数据
			
			timeCount = 0;
			ESP8266_Clear();
		}
		
		dataPtr = ESP8266_GetIPD(0);
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);
		
		delay_ms(10);
	}

}
