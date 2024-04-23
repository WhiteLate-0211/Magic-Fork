#include "Capture.h"

Capture::Capture()
{
	arr = 0XFFFFFFFF;
	psc = 84-1;
	Capture_Init(psc,arr);
}

Capture::~Capture()
{
	
}

void Capture::Capture_Init(u32 psc,u16 arr)
{
	GPIO_InitTypeDef 			GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	NVIC_InitTypeDef 			NVIC_InitStructure;

	//TIM5ʱ��ʹ��    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	
	//ʹ��PORTAʱ��	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	
	//GPIOA0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	//���ù���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	//���츴�����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	//����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 
	//��ʼ��PA0
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//PA0����λ��ʱ��5
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); 
	//��ʱ����Ƶ������Ϊ0xffffffffʱΪ��1us
	TIM_TimeBaseStructure.TIM_Prescaler = psc;  
	//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	//�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_Period = arr;   
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	
	//��ʼ��TIM5���벶�����
	TIM_ICInitTypeDef  				TIM5_ICInitStructure;
	//CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; 
	//�����ز���
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	
	//ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
	//���������Ƶ,����Ƶ 
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 
	//IC1F=0000 ���������˲��� ���˲�
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	//��������ж� ,����CC1IE�����ж�	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);
	//ʹ�ܶ�ʱ��5
	TIM_Cmd(TIM5,ENABLE ); 		


	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		
	//IRQͨ��ʹ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	NVIC_Init(&NVIC_InitStructure);	
}
