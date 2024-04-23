#include "PWM.h"

PWM::PWM()
{
	//��ʱ����Ƶ����Ϊ��84MHz
	delay_init(84);
	//�Զ���װ��ֵ��500
	arr = 500 - 1;
	//ʱ��Ԥ��Ƶϵ����84
	psc = 84 - 1;
	//������PWMƵ�ʣ�����ʱ����Ƶ/Ԥ��Ƶϵ����/�Զ���װ��ֵ
	//84/84 = 1MHz�� 1MHz/500 = 2KHz��
	TIMx			= 	TIM14;
	TIM_CLKBUS 		= 	RCC_APB1Periph_TIM14;
	
	GPIO_CLKBUS 	= 	RCC_AHB1Periph_GPIOA;
	GPIOx 			= 	GPIOA;
	GPIO_PinSource 	= 	GPIO_PinSource7;
	GPIO_AF_TIMx 	= 	GPIO_AF_TIM14;
	GPIO_Pin_x 		= 	GPIO_Pin_7;
}

PWM::~PWM()
{
	
}

void PWM::compare_set(TIM_TypeDef * TIMx,u16 pst)
{
	if(pst >= arr )
		TIM_SetCompare1(TIMx, arr/2);
	else
		TIM_SetCompare1(TIMx, pst);
}

void PWM::pwm_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	//TIM14ʱ��ʹ��    
	RCC_APB1PeriphClockCmd(TIM_CLKBUS,ENABLE);  		
	//ʹ��PORTFʱ��		
	RCC_AHB1PeriphClockCmd(GPIO_CLKBUS, ENABLE); 			
	//GPIOF9����Ϊ��ʱ��14
	GPIO_PinAFConfig(GPIOx,GPIO_PinSource,GPIO_AF_TIMx); 			
	//GPIO A9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
	//���ù���	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        	
	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		
	//���츴�����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    
	//����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	//��ʼ��PF9
	GPIO_Init(GPIOx,&GPIO_InitStructure);              				
	//��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_Prescaler = psc;  		
	//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	
	//�Զ���װ��ֵ	
	TIM_TimeBaseStructure.TIM_Period = arr;   						
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	//��ʼ����ʱ��14
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);					
	
	//��ʼ��TIM14 Channel1 PWMģʽ	
	//�Ƚ����ʹ��	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//�������:TIM����Ƚϼ��Ե�
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	//����Tָ���Ĳ�����ʼ������TIM1 4OC1	
	TIM_OC1Init(TIMx, &TIM_OCInitStructure); 
	//ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���	
	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
	//ARPEʹ�� 
	TIM_ARRPreloadConfig(TIMx,ENABLE);								
	//ʹ��TIM14
	TIM_Cmd(TIMx, ENABLE);  																	  	
}
