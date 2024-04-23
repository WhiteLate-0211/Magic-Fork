/*
 **********************************************************************
 *Copyright(c)		CHIC
 *Identification	CHIC
 *Describtion		Template
 *
 *Present Version	1.0
 *Writer			Huan Liu
 *Date				7/19/2019
 **********************************************************************
 *@���߷�ʽ
 **********************************************************************
 *���ݿ� D[0 : 7] -----------------	PE[8 : 15]
 *WCLK		-----------------------	PC0
 *FQUP		-----------------------	PC1
 *REST		-----------------------	PC2
 *
 **********************************************************************
**/
#ifndef __AD9850_H
#define __AD9850_H
#include "sys.h"
#define  RCC_WCLK    	RCC_AHB1Periph_GPIOC
#define  RCC_FQUP    	RCC_AHB1Periph_GPIOC
#define  RCC_REST    	RCC_AHB1Periph_GPIOC

#define  PIN_WCLK    	GPIO_Pin_0
#define  PIN_FQUP    	GPIO_Pin_1
#define  PIN_REST    	GPIO_Pin_2

#define  GPIO_WCLK    	GPIOC
#define  GPIO_FQUP    	GPIOC
#define  GPIO_REST    	GPIOC

#define  WCLK_H     	GPIO_SetBits(GPIO_WCLK,PIN_WCLK)
#define  WCLK_L     	GPIO_ResetBits(GPIO_WCLK,PIN_WCLK)

#define  FQUP_H     	GPIO_SetBits(GPIO_FQUP,PIN_FQUP) 
#define  FQUP_L     	GPIO_ResetBits(GPIO_FQUP,PIN_FQUP) 

#define  REST_H     	GPIO_SetBits(GPIO_REST,PIN_REST) 
#define  REST_L     	GPIO_ResetBits(GPIO_REST,PIN_REST)

//AD9850��ʼ������
void AD9850_Init(void);
//AD9850��λ����
void AD9850_Rest(void);
//frequence:������Ƶ�ʣ�w0����λ��Ĭ��д0����
void AD9850_SetOut(double frequence,u8 w0);

//��ʼ�����������������ò���
void B8080_Init(GPIOMode_TypeDef mode);
void Data_Out(u8 data);
u8 Data_In(void);
#endif 
