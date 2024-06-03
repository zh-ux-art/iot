#include "lsens.h"
#include "delay.h"
 

void Lsens_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTFʱ��	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;//PA4����Ϊ��������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
  Adc_Init();
}


u8 Lsens_Get_Val(void)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<LSENS_READ_TIMES;t++)
	{
		temp_val+=Get_Adc(LSENS_ADC_CHX);	//��ȡADCֵ
		delay_ms(5);
	}
	temp_val/=LSENS_READ_TIMES;//�õ�ƽ��ֵ 
	if(temp_val>4000)temp_val=4000;
	temp_val=temp_val/40;
	return (u8)(100-temp_val);
}



u16 Mq2_Get_Val(void)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<LSENS_READ_TIMES;t++)
	{
		temp_val+=Get_Adc(MQ2_ADC_CHX);	//��ȡADCֵ
		delay_ms(5);
	}
	temp_val/=LSENS_READ_TIMES;//�õ�ƽ��ֵ 
	if(temp_val>4000)temp_val=4000;
	temp_val=temp_val/4;
	return temp_val-1;
}








