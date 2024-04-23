#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_spi.h"

#include "delay.h"

#include "spi.h"

#include "AD5689.h"

AD5689_HandleTypeDef AD5689_1;
extern SPI_HandleTypeDef SPI1Handler;
#define AD5689_SPI SPI1Handler

/**
  * @brief   AD5689���ź�SPI��ʼ��
  * @param   	 
  * @retval  
  */
void AD5689_Conn_Init(void)
{
	/*SPI���߳�ʼ��*/
	#ifndef __SPI1_ENABLED_
	SPI1_Init();
	#endif
	
	AD5689_1.hspi=&AD5689_SPI;
	
	/*����SYNC��GPIO*/
	__HAL_RCC_GPIOB_CLK_ENABLE();
	AD5689_1.SYNC_GPIO.Mode=GPIO_MODE_OUTPUT_PP;
	AD5689_1.SYNC_GPIO.Pin=GPIO_PIN_9;
	AD5689_1.SYNC_GPIO.Pull=GPIO_PULLUP;
	AD5689_1.SYNC_GPIO.Speed=GPIO_SPEED_FREQ_HIGH;
	AD5689_1.SYNC_GPIO_GROUP=GPIOB;
	
	HAL_GPIO_Init(AD5689_1.SYNC_GPIO_GROUP,&(AD5689_1.SYNC_GPIO));
	HAL_GPIO_WritePin(AD5689_1.SYNC_GPIO_GROUP, AD5689_1.SYNC_GPIO.Pin, GPIO_PIN_SET);//����SYNC
}

/**
  * @brief  AD5689��ʼ��
  * @param  *hAD5689	��	������ 
  * @retval ��
  */
void AD5689_Init(AD5689_HandleTypeDef *hAD5689)
{
	AD5689_Conn_Init();
  delay_ms(5);
  AD5689_Reset(hAD5689);
	AD5689_DAISYCHAIN_OPERATION(hAD5689,Standalone_mode);
  AD5689_LDAC_MASK(hAD5689,LDAC_Disable,LDAC_Disable);
}

/**
  * @brief   ����AD5689�Ĵ���ֵ
  * @param   *hAD5689	��	������   
	*          command	��	ָ��
  *          channel	��	ͨ��
  *          data  	 	��	����
  * @retval 
  */
void AD5689_SetRegisterValue(AD5689_HandleTypeDef *hAD5689,uint8_t command,uint16_t channel,uint16_t data)
{
  uint8_t reg[3]={0};
  
  reg[0]=(command<<4)|channel;
  reg[1]=(data>>8)&0xFF;
  reg[2]=data&0xFF;
	
	//����SYNC
	HAL_GPIO_WritePin(hAD5689->SYNC_GPIO_GROUP, hAD5689->SYNC_GPIO.Pin, GPIO_PIN_RESET);//����SYNC
	delay_us(10);
	//��������
	HAL_SPI_Transmit(hAD5689->hspi,reg,3,0xFFFF);
	delay_us(10);
	//����SYNC
	HAL_GPIO_WritePin(hAD5689->SYNC_GPIO_GROUP, hAD5689->SYNC_GPIO.Pin, GPIO_PIN_SET);//����SYNC
	delay_us(10);
}

/**
  * @brief  LDAC���μĴ�������
  * @param  LDAC����ѡ��
  * @retval ��
  */
void AD5689_LDAC_MASK(AD5689_HandleTypeDef *hAD5689,uint16_t ldac1,uint16_t ldac2)
{
  uint8_t data=ldac1;
  data |=(ldac2<<3);
  AD5689_SetRegisterValue(hAD5689,CMD_LDAC_MASK,DAC_ALL,data);
}

/**
  * @brief  �����λ
  * @param  *hAD5689��	������ 
  * @retval ��
  */
void AD5689_Reset(AD5689_HandleTypeDef *hAD5689)
{
  AD5689_SetRegisterValue(hAD5689,CMD_RESET,DAC_ALL,0);
}

/**
  * @brief  ��������
  * @param  *hAD5689�������� 
	*         channel1��ͨ��1�ĵ���ģʽ
  *         channel2��ͨ��2�ĵ���ģʽ
  * @retval ��
  */
void AD5689_PowerDown(AD5689_HandleTypeDef *hAD5689,uint16_t channel1,uint16_t channel2)
{
  uint8_t data=0x3C;
  data |=((channel2<<6)|channel1);
  AD5689_SetRegisterValue(hAD5689,CMD_POWER_DOWN,DAC_ALL,data);
}


/**
  * @brief  д��͸���DACͨ��n(��LDAC�޹�)
  * @param  *hAD5689�������� 
	*         channel	��ͨ��
  *         data   	����ֵ
  * @retval ��
  */
void AD5689_WriteUpdate_DACREG(AD5689_HandleTypeDef *hAD5689,uint16_t channel,uint16_t data)
{
  AD5689_SetRegisterValue(hAD5689,CMD_WRITE_UPDATEn,channel,data);
}

/**
  * @brief  д������Ĵ���n(ȡ����LDAC)
  * @param  *hAD5689�������� 
						channel	��ͨ��
  *         data   	����ֵ
  * @retval ��
  */
void AD5689_Write_InputREG(AD5689_HandleTypeDef *hAD5689,uint16_t channel,uint16_t data)
{
  AD5689_SetRegisterValue(hAD5689,CMD_WRITEn,channel,data);
}

/**
  * @brief  ������Ĵ���n�����ݸ���DAC�Ĵ���n
  * @param  *hAD5689��������
	*         channel��ͨ��
  *         data   ����ֵ
  * @retval ��
  */
void AD5689_InputREG_toUpdate_DACREG(AD5689_HandleTypeDef *hAD5689,uint16_t channel,uint16_t data)
{
  AD5689_SetRegisterValue(hAD5689,CMD_WRITEn_UPDATEn,channel,data);
}

/**
  * @brief  �ջ���ʹ��
  * @param  �ջ�������
  * @retval ��
  */
void AD5689_DAISYCHAIN_OPERATION(AD5689_HandleTypeDef *hAD5689,uint16_t decn)
{
  AD5689_SetRegisterValue(hAD5689,CMD_DCEN_REG,DAC_ALL,decn);
}
