/*
 **********************************************************************
 *Copyright(c)		Lance Zhang
 *Identification	CHIC
 *Describtion		Template
 *
 *Present Version	1.0
 *Writer			Lance Zhang
 *Date				8/16/2018
 **********************************************************************
**�жϷ������ļ�
 **********************************************************************
 *�жϷ��������ܷ���.cpp�ļ��У������Ҫ���⽨��һ��.c�ļ�
 **********************************************************************
**/
#include "Interrupt.h"
#include "usart.h"
#include "stm32f4xx_adc.h"

u16 VOLTAGE;
u16 VOL_BUF[1024];
u16 count = 0;
u8 flag = 0;
//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		if(flag == 0)
		{
			VOL_BUF[count] = ADC_GetConversionValue(ADC1);
			VOLTAGE = VOL_BUF[count];
			count++;			
		}

		if(count == 1024)
		{
			count = 0;
			//flag = 1;
		}						
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}
