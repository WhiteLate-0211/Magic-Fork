/*AD7606��������
���Ӿ���ר��
ͨ��SPIЭ��

SCLK��̬�ߣ������½�����Ч

(C)LWNF 2015-2021
*/

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_spi.h"
#include "delay.h"

#include "spi.h"

#include "AD7606.h"

#include "stdio.h"


int16_t AD7606_ADCValue[8];//ADCֵ������
float AD7606_ADCValue_A[8];//ADCֵģ����

extern SPI_HandleTypeDef SPI1Handler;//AD760R6_SPI���

#define AD7606_SPI SPI1Handler 

/*io����
SPI1

SCK PA5
MISO PA6
CS PB2
BUSY PB10(�ⲿ�ж�)
RST PA12
CONV PA11
*/

void AD7606_Init(void)
{
	//ѡ������SPI
	#ifndef __SPI1_ENABLED_
	SPI1_Init();
	#endif
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef AD7606_GPIO;
	
	AD7606_GPIO.Pin = GPIO_PIN_11 | GPIO_PIN_12; //������ͨIO
	AD7606_GPIO.Mode = GPIO_MODE_OUTPUT_PP;
	AD7606_GPIO.Pull = GPIO_PULLUP;
	AD7606_GPIO.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOA, &AD7606_GPIO);
	
	AD7606_GPIO.Pin = GPIO_PIN_2; //������ͨIO
	AD7606_GPIO.Mode = GPIO_MODE_OUTPUT_PP;
	AD7606_GPIO.Pull = GPIO_PULLUP;
	AD7606_GPIO.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOB, &AD7606_GPIO);
	
	AD7606_GPIO.Pin = GPIO_PIN_10;//æ�źţ��ⲿ�жϣ�
	AD7606_GPIO.Mode=GPIO_MODE_IT_FALLING;
	AD7606_GPIO.Pull = GPIO_NOPULL;
	AD7606_GPIO.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOB, &AD7606_GPIO);
	
	HAL_NVIC_SetPriority(EXTI15_10_IRQn,0,0);
	
/*IO���IO�Ŷ���*/
/*IO��*/
#define AD7606_CS_GPIOG		GPIOB
#define AD7606_RST_GPIOG	GPIOA
#define AD7606_CONV_GPIOG	GPIOA
	
#define AD7606_CS_GPION		GPIO_PIN_2
#define AD7606_RST_GPION	GPIO_PIN_12
#define AD7606_CONV_GPION	GPIO_PIN_11
	
	AD7606_Reset();
	
}

void AD7606_Reset(void)
{
	HAL_GPIO_WritePin(AD7606_RST_GPIOG, AD7606_RST_GPION, GPIO_PIN_RESET);
	delay_us(10);
	HAL_GPIO_WritePin(AD7606_RST_GPIOG, AD7606_RST_GPION, GPIO_PIN_SET);
	delay_us(10);
	HAL_GPIO_WritePin(AD7606_RST_GPIOG, AD7606_RST_GPION, GPIO_PIN_RESET);
	delay_us(10);
}

void AD7606_StartConv(void)
{
	AD7606_Reset();
	HAL_GPIO_WritePin(AD7606_CS_GPIOG, AD7606_CS_GPION, GPIO_PIN_SET);
	HAL_GPIO_WritePin(AD7606_CONV_GPIOG, AD7606_CONV_GPION, GPIO_PIN_SET);
	delay_us(100);
	
	HAL_GPIO_WritePin(AD7606_CONV_GPIOG, AD7606_CONV_GPION, GPIO_PIN_RESET);//ת��
	delay_us(100);
	HAL_GPIO_WritePin(AD7606_CONV_GPIOG, AD7606_CONV_GPION, GPIO_PIN_SET);
	
	//�ȴ�BUSY����
//	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

//����BUSYæ�ź�
void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10); 
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_10)AD7606_ReadConvData();
}

	uint8_t AD7606_ConvDataBuf[16];

void AD7606_ReadConvData(void)
{
	uint8_t i;
	uint8_t ADCValue_buf[16];
	HAL_GPIO_WritePin(AD7606_CS_GPIOG, AD7606_CS_GPION, GPIO_PIN_RESET);//����Ƭѡ
	delay_us(100);
	
	//������������
	HAL_SPI_Receive(&AD7606_SPI,ADCValue_buf,16,0xFFFF);
	
	HAL_GPIO_WritePin(AD7606_CS_GPIOG, AD7606_CS_GPION, GPIO_PIN_SET);
	
	for(i=0;i<8;i++)
	{
		*(AD7606_ADCValue+i)=(int16_t)(ADCValue_buf[2*i]<<8)+ADCValue_buf[2*i+1];
		*(AD7606_ADCValue_A+i)=(float)RANGE*(*(AD7606_ADCValue+i))/32768;
	}
}
