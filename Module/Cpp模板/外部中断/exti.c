#include "exti.h"

//�жϷ�����
void EXTI2_IRQHandler(void)
{
	delay_ms(10);	//����
	if(PEin(2)==0)	  
	{				 
   PAout(5)=!PAout(5); 
	}		 
	 EXTI_ClearITPendingBit(EXTI_Line2);//���LINE2�ϵ��жϱ�־λ 
}
//LED���Ժ��������п���
void LED_Init()
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	PAout(5) = 0;
}
//�ⲿ�ж϶˿�ѡ�������õ���PE2
void EXIO_Init()
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
}

void EXTIX_Init()
{
	//���LED��ʼ���������ڲ��ԣ�ʹ��ʱ����ע�͵�
	LED_Init();
	///////////
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	//��Ӧ���ж϶˿�
	EXIO_Init();
	//ʹ��SYSCFGʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	//PE2 ���ӵ��ж���2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
	//����EXTI_Line2
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	//�ж��¼�
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	//�½��ش���
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
	//�ж���ʹ��
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	//����
	EXTI_Init(&EXTI_InitStructure);
	
	//�ⲿ�ж�2
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	//ʹ���ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//����  
	NVIC_Init(&NVIC_InitStructure);
}












