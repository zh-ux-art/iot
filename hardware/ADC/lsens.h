#ifndef __LSENS_H
#define __LSENS_H	
#include "sys.h"  
#include "adc.h" 
  
#define LSENS_READ_TIMES	5		//���崫������ȡ����,����ô���,Ȼ��ȡƽ��ֵ
#define LSENS_ADC_CHX		ADC_Channel_4	//���������������ڵ�ADCͨ�����
#define MQ2_ADC_CHX		ADC_Channel_5	//�����������������ڵ�ADCͨ�����    
void Lsens_Init(void);//��ʼ��������
u8 Lsens_Get_Val(void);	
u16 Mq2_Get_Val(void);	
#endif 

