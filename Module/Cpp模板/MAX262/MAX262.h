/*
 **********************************************************************
 *Copyright(c)		Lance Zhang
 *Identification	CHIC
 *Describtion		Template
 *
 *Present Version	1.0
 *Writer			Lance Zhang
 *Date				7/19/2019
 **********************************************************************
**/
/*
 **********************************************************************
 *��Ҫ��Ƭ���ṩ����������ʱ����Ƶ����Ϊ84MHz��λ��TIM14 CH1��
 *����⣺
 *		stm32f4xx_gpio.h
 *		stm32f4xx_tim.h
 **********************************************************************
 *���ܲ����ĳ�ͻ��
 *				����
 **********************************************************************
 *ʹ�÷�����
 *		������Ҫ������ļ�(MAX262.c | MAX262.h)
 *		��ʼ���� --- MAX262
 *		���� Filter_On()�������������˲�
 **********************************************************************
**/
#ifndef	__MAX262_H
#define __MAX262_H
extern "C"
{
	#include "stm32f4xx_gpio.h"
	#include "stm32f4xx_tim.h"
	#include "delay.h"
	#include "sys.h"
}
#define MAX262_Port		GPIOE
#define MAX262_IOCLK	
//[A0 : A3]
#define ADDR_Pin_0		GPIO_Pin_8
#define ADDR_Pin_1		GPIO_Pin_9
#define ADDR_Pin_2		GPIO_Pin_10
#define ADDR_Pin_3		GPIO_Pin_11
//[D0 D1]
#define DATA_Pin_0		GPIO_Pin_12
#define DATA_Pin_1		GPIO_Pin_13
//������ʹ������
#define EN_Pin			GPIO_Pin_14	

#define EN		PEout(14)

#define PI	3.1415926
extern enum 
{
	modeOne=0,
	modeTwo,
	modeThree,
	modeFour
}workMode;

extern enum 
{
	lowPass=0,
	highPass,
	bandPass,
	allPass,
	bandStop
}passMode; 

extern enum 
{
	channelA=0,
	channelB
}channel; 

//CLKʱ��λ��PA7��Ƶ�� 1MHz
class	MAX262
{
public:
	MAX262();
	~MAX262();
	float bs_WorkFclk(float f0 = 5000,float Q = 2,u8 workMode = modeOne,u8 channel = channelA);
	void Filter_On(float f0 = 5000,float Q = 2,u8 workMode = modeOne,u8 channel = channelA);
private:
	void IO_Init();
	void write_data(u8 addr, u8 data);
	void Data_Out(u8 data);
	u8	Data_In(void);
	void Set_Af(u8 data_f);
	void Set_Aq(u8 data_q);
	void Set_Bf(u8 data_f);
	void Set_Bq(u8 data_q);
//оƬ����
private:
	void HeartBeat_Init();	
	void CLK_set(float freq);

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
