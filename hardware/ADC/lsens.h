#ifndef __LSENS_H
#define __LSENS_H	
#include "sys.h"  
#include "adc.h" 
  
#define LSENS_READ_TIMES	5		//定义传感器读取次数,读这么多次,然后取平均值
#define LSENS_ADC_CHX		ADC_Channel_4	//定义烟雾传感器所在的ADC通道编号
#define MQ2_ADC_CHX		ADC_Channel_5	//定义土壤传感器所在的ADC通道编号    
void Lsens_Init(void);//初始化传感器
u8 Lsens_Get_Val(void);	
u16 Mq2_Get_Val(void);	
#endif 

