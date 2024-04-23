#include "ADC.h"

ADC_Internal::ADC_Internal(bool interrupt_used)
{
	if(interrupt_used)
	{
		TIM3_Int_Init();
		extern u16	VOLTAGE;
		vol = &VOLTAGE;		
	}
	func_Init();
	Get_Adc();
}

ADC_Internal::~ADC_Internal()
{
	
}

void ADC_Internal::func_Init()
{
	GPIO_InitTypeDef  		GPIO_InitStructure;
	ADC_CommonInitTypeDef 	ADC_CommonInitStructure;
	ADC_InitTypeDef       	ADC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 

	//�ȳ�ʼ��ADC1ͨ��5 IO��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	//����������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

	//ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  
	//��λ����	 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	

	//����ģʽ
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	//���������׶�֮����ӳ�5��ʱ��
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	//DMAʧ��
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 
	//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	//��ʼ��
	ADC_CommonInit(&ADC_CommonInitStructure);
	//12λģʽ
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	//��ɨ��ģʽ	
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	//�ر�����ת��
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	//�Ҷ���	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	//����ADת����
	ADC_Cmd(ADC1, ENABLE);	
}

u16 ADC_Internal::Get_Adc(u8 ch)   
{
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��	
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );			    
	//ʹ��ָ����ADC1�����ת����������	
	ADC_SoftwareStartConv(ADC1);		
	//�ȴ�ת������
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
	//�������һ��ADC1�������ת�����
	return ADC_GetConversionValue(ADC1);	
}

u16 ADC_Internal::Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		//delay_ms(1);
	}
	return temp_val/times;
} 
