#include "adc.h"
#include "delay.h"
#include "sys.h"  

void Adc_Init(void)  //ADC通道初始化
{
	ADC_InitTypeDef ADC_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE );	  //使能GPIOA,ADC1通道时钟
  
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //分频因子6时钟为72M/6=12MHz

 	ADC_DeInit(ADC1);  //将外设 ADC1 的全部寄存器重设为缺省值
 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器

	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1

	ADC_ResetCalibration(ADC1);	//重置指定的ADC1的复位寄存器

    while(ADC_GetResetCalibrationStatus(ADC1));	//获取ADC1重置校准寄存器的状态,设置状态则等待

	ADC_StartCalibration(ADC1);	 //开启AD校准

	while(ADC_GetCalibrationStatus(ADC1));		//获取指定ADC1的校准程序,设置状态则等待
}
//获得ADC某个通道的值
//ch:通道值 0~16
//返回值:转换结果
u16 Get_Adc(u8 ch)   
{
 
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道3,第一个转换,采样时间为239.5周期	  			    
	ADC_RegularChannelConfig(ADC1, ch, 2, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道3,第一个转换,采样时间为239.5周期	 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}


