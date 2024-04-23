/*TLC5615��������
���Ӿ���ר��
ͨ��SPIЭ��

SCLK��̬�ͣ�������������Ч

Uout=(Din/1024)*2*Vref

(C)LWNF 2015-2021
*/

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_spi.h"

#include "delay.h"

#include "spi.h"

#include "TLC5615.h"

TLC5615_HandleTypeDef TLC5615_1;//�ڴ˴�дTLC5615������
extern SPI_HandleTypeDef SPI3Handler;
#define TLC5615_SPI SPI3Handler
	
void TLC5615_Conn_Init(void)
{
	/*������ʹ�õ�SPI*/
	#ifndef __SPI3_ENABLED_
	SPI3_Init();
	#endif
	TLC5615_1.hspi=&TLC5615_SPI;
	TLC5615_1.Vref=2;//�ο���ѹ
	
	/*����Ƭѡ*/
	__HAL_RCC_GPIOA_CLK_ENABLE();
		
	TLC5615_1.CS_GPIO.Mode=GPIO_MODE_OUTPUT_PP;
	TLC5615_1.CS_GPIO.Pin=GPIO_PIN_15;
	TLC5615_1.CS_GPIO.Pull=GPIO_PULLUP;
	TLC5615_1.CS_GPIO.Speed=GPIO_SPEED_FREQ_HIGH;
	
	TLC5615_1.GPIO_GROUP=GPIOA;
	
	HAL_GPIO_Init(TLC5615_1.GPIO_GROUP,&(TLC5615_1.CS_GPIO));
	
	HAL_GPIO_WritePin(TLC5615_1.GPIO_GROUP, TLC5615_1.CS_GPIO.Pin, GPIO_PIN_SET);
}

void TLC5615_TransmitData(TLC5615_HandleTypeDef *hTLC5615,uint16_t dat)
{
	uint16_t tdat=((dat&0xFF)<<8)+((dat&0xFF00)>>8);
	//����Ƭѡ
	HAL_GPIO_WritePin(hTLC5615->GPIO_GROUP,hTLC5615->CS_GPIO.Pin, GPIO_PIN_RESET);
	//��������
	HAL_SPI_Transmit(hTLC5615->hspi,(uint8_t*)&tdat,2,0xFFFF);
	//����Ƭѡ
	HAL_GPIO_WritePin(hTLC5615->GPIO_GROUP,hTLC5615->CS_GPIO.Pin, GPIO_PIN_SET);
}

/*
	�������������
	[15:12]X X X X,[11:2]10-bits DAT,[1:0]X X
*/
void TLC5615_SetDigtalValue(TLC5615_HandleTypeDef *hTLC5615,uint16_t dat)
{
	if(dat>1023)dat=1023;
	uint16_t tdat=dat<<2;
	TLC5615_TransmitData(hTLC5615,tdat);
}

/*�������ģ���ѹ*/
void TLC5615_SetOutputVoltage(TLC5615_HandleTypeDef *hTLC5615,float voltage)
{
	if(voltage>(2*(hTLC5615->Vref)))voltage=2*(hTLC5615->Vref);
	else if(voltage<0)voltage=0;
	uint16_t Din=(1024*voltage)/(2*(hTLC5615->Vref));
	TLC5615_SetDigtalValue(hTLC5615,Din);
}
