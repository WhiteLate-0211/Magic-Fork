/**
 **********************************************************************
 *@Copyright(c)		Lance Zhang
 *@Identification	CHIC
 *@Describtion		Template
 *
 *@PresentVersion	1.0
 *@Writer			Lance Zhang
 *@Date				7/20/2019
 **********************************************************************/
/**********************************************************************
 *@Instruction
 *ʹ�÷�����
 *
 *1����ʼ��debug_timer��
 *2������TIM_SetCounter(TIMx,0);���ü�������ֵ
 *3������TIM_GetCounter(TIM6);	��ȡ��������ǰ�ļ���ֵ
 **********************************************************************/
/**********************************************************************
 *@Attention
 *�������ӷ�ʽ
 *SCLK 			PB3
 *CS			PB7
 *DOUT 			PB5 	
 *DIN  			PB6		���ն�
 **********************************************************************
 *@Compatibility	�����˶���ʱ�ĳ�ʼ��������������Ҫdelay���ܵ�ģ��
 *					����ʱ�п��ܳ�������
 *
 *��������ԣ���
 *********************************************************************
 *@Characteristic	SPI�����������ڳ�ʼ���Ĺ�����ʹ������ʱ
 *					ʱ�䲻��û�з�װÿһλ����ָ������Ҫ�ı�
 *					�Ĵ���������ο�оƬ�ֲ�
 *********************************************************************/
#ifndef	__AD7190_H
#define __AD7190_H
extern "C"{
	#include "stm32f4xx_gpio.h"
	#include "sys.h"
	#include "delay.h"
}

#define	 AD7190_ACC        	16777216
#define  SCLK    			PBout(3)
#define  DOUT   			PBout(5)
#define  DIN    			PBin(6) 
#define  CS     			PBout(7)    

#define	RCC_CS1				RCC_AHB1Periph_GPIOB
#define	RCC_SPI_CLK			RCC_AHB1Periph_GPIOB
#define	RCC_SPI_DIN			RCC_AHB1Periph_GPIOB
#define	RCC_SPI_DOUT		RCC_AHB1Periph_GPIOB

#define PIN_CS1        		GPIO_Pin_7
#define PIN_SPI_CLK    		GPIO_Pin_3
#define PIN_SPI_DIN    		GPIO_Pin_6
#define PIN_SPI_DOUT   		GPIO_Pin_5

#define GPIO_CS1       		GPIOB
#define GPIO_SPI_CLK   		GPIOB
#define GPIO_SPI_DIN      	GPIOB
#define GPIO_SPI_DOUT  		GPIOB

class AD7190
{
public:
	AD7190();
	~AD7190();
//�û��ӿ�
public:
	void AD7190_Init(void);
	//��ȡת��ǰ��ĵ�ѹֵ
	u32 Get_Register_Val(void);
	double Get_Voltage(void);
//���ú���
private:
	void IO_Init(void);
	void data_write(u8 data);
	u8 data_read(void);
	//��ȡAD7190����
	u32 AD7190_Read_Data(u16 ID);		
};

#endif
