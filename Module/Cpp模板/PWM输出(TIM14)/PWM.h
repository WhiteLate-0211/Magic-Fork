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
**/
/*
 **********************************************************************
 *Ƶ�ʺ�ռ�ձ�ͨ��arr��psc���ƣ�ʱ����Ƶ����Ϊ84MHz
 *����⣺
 *		stm32f4xx_gpio.h
 *		stm32f4xx_tim.h
 **********************************************************************
**/
#ifndef __PWM_
#define __PWM_

extern "C"{
	#include "sys.h"
	#include "usart.h"
	#include "delay.h"
};

class PWM
{
	private:
		
	public:
		PWM();
		~PWM();
		void pwm_init(void);								//��ʼ�����ڲ���������ɺ�������Ի��pwm��
		void compare_set(TIM_TypeDef * TIMx,u16 pst);		//ռ�ձ����ú������ڳ�ʼ����ɺ�ʹ��

		u32 arr;
		u32 psc;
		
		uint32_t	TIM_CLKBUS;				//����TIMʱ����
		uint32_t	GPIO_CLKBUS;			//����GPIOʱ����
		uint32_t	GPIO_PinSource;			//���ÿ������ù��ܵ�����
		uint16_t	GPIO_Pin_x;				//���ö�Ӧ��GPIO����
		uint8_t		GPIO_AF_TIMx;			//���ø��ù��ܶ�Ӧ�Ķ�ʱ��

		GPIO_TypeDef *	GPIOx;				//ѡȡ��Ӧ��GPIO�˿�
		TIM_TypeDef*	TIMx;				//ѡȡ��Ӧ��TIMͨ��
};

#endif
