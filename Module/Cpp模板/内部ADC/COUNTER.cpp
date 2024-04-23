#include "COUNTER.h"

COUNTER::COUNTER()
{
	//����ϵͳ�ж����ȼ�����2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				
	//TIM3_Int_Init();	
	//LED_Init();
}

COUNTER::~COUNTER()
{

}
/*TIM3��ʼ��	�Զ�װ�س�ֵ�趨��5000 - 1��
				��ʱ����Ƶ��8400 - 1
				��ʱ��ʱ�ӣ�84MHz��
*/
void COUNTER::TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//ʹ��TIM3ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  		
	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Period = arr; 				
	//��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  					
	//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	//��ʼ��TIM3
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	//����ʱ��3�����ж�
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 
	//ʹ�ܶ�ʱ��3
	TIM_Cmd(TIM3,ENABLE); 
	
	//��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; 
	//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; 
	//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}
/*LED��ʼ����������*/
void COUNTER::LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��

  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//LED2��LED3��ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO

  GPIO_ResetBits(GPIOA,GPIO_Pin_5);//���øߣ�����

}
