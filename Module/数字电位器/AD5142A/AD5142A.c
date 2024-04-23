/*AD5142A��������
���Ӿ���ר��
ͨ��IICЭ��

Rwb=Rab*(D/255)+Rw;

(C)LWNF 2015-2021
*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_i2c.h"

#include "delay.h"

#include "i2c.h"

#include "AD5142A.h"

/**/

extern I2C_HandleTypeDef I2C3Handler;//AD5142A_I2C���

AD5142A_HandleTypeDef AD5142A_1;//�ڴ˴�дAD5142������
	
void AD5142A_Conn_Init(void)
{
	/*������ʹ�õ�I2C*/
	#ifndef __I2C3_ENABLED_
	I2C3_Init();
	#endif
	AD5142A_1.hi2c=&I2C3Handler;
	AD5142A_1.I2C_Address=(0x2A)<<1;
	AD5142A_1.Max_Resistance=96700;
}

void AD5142A_TransmitData(AD5142A_HandleTypeDef *hAD5142A,uint16_t dat)
{
	uint16_t tdat=((dat&0xFF)<<8)+((dat&0xFF00)>>8);
	HAL_I2C_Master_Transmit(hAD5142A->hi2c,hAD5142A->I2C_Address,(uint8_t*)&tdat,2,0xFFFF);
}

uint8_t AD5142A_ReadData(AD5142A_HandleTypeDef *hAD5142A,uint8_t *datSource)
{
	uint8_t dat;
	HAL_I2C_Master_Receive(hAD5142A->hi2c,hAD5142A->I2C_Address,&dat,1,0xFFFF);
	return dat;
}

/*
	CmdNo 0
	��ָ��
	[15:8]0 0 0 0 0 0 0 0,[7:0]0 0 0 0 0 0 0
*/
void AD5142A_Nop(AD5142A_HandleTypeDef *hAD5142A)
{
	const uint16_t CmdNo0=0x0000;
	AD5142A_TransmitData(&AD5142A_1,CmdNo0);
}

/*
	CmdNo 1
	�����мĴ�����������д��RDAC
	[15:8]0 0 0 1 0 0 0 A0,[7:0]RADC_Vlue
*/
void AD5142A_WriteChannel(AD5142A_HandleTypeDef *hAD5142A,uint8_t RADC_Channel,uint8_t RADC_Value)
{
	uint8_t Control_Address = 0x10|RADC_Channel;//����λ�͵�ַλ
	AD5142A_TransmitData(hAD5142A,(((uint16_t)Control_Address)<<8)+RADC_Value);
}

/*
	CmdNo 2
	�����мĴ�����������д������Ĵ���
	[15:8]0 0 1 0 0 0 0 A0,[7:0]RADC_Vlue
*/
void AD5142A_WriteInputReg(AD5142A_HandleTypeDef *hAD5142A,uint8_t RADC_Channel,uint8_t RADC_Value)
{
	uint8_t Control_Address = 0x20|RADC_Channel;//����λ�͵�ַλ
	AD5142A_TransmitData(hAD5142A,(((uint16_t)Control_Address)<<8)+RADC_Value);
}

/*
	CmdNo 3
	�ض�����
	[15:8]0 0 1 1 X 0 A1 A0,[7:0]datsource
*/
uint8_t AD5142A_ReadBackValue(AD5142A_HandleTypeDef *hAD5142A,uint8_t ReadChannel,uint8_t DataSourece)
{
	uint8_t Control_Address = 0x30|ReadChannel;//����λ�͵�ַλ
	AD5142A_TransmitData(hAD5142A,(((uint16_t)Control_Address)<<8)+(DataSourece&0x03));
	uint8_t AD5142A_ReadValue;
	AD5142A_ReadData(hAD5142A,&AD5142A_ReadValue);
	return AD5142A_ReadValue;
}

/*
	CmdNo 9
	����RDAC�Ĵ���������EEPROM
	[15:8]0 1 1 1 0 0 0 A0,[7:0]0 0 0 0 0 0 0 1
*/
void AD5142A_SaveChannel(AD5142A_HandleTypeDef *hAD5142A,uint8_t RADC_Channel)
{
	uint8_t Control_Address = 0x20|RADC_Channel;//����λ�͵�ַλ
	AD5142A_TransmitData(hAD5142A,(((uint16_t)Control_Address)<<8)+0x01);
	delay_ms(20);
}

/*
	CmdNo 10
	����EEPROM������RDAC
	[15:8]0 1 1 1 0 0 0 A0,[7:0]0 0 0 0 0 0 0 0
*/
void AD5142A_SetSaveChannel(AD5142A_HandleTypeDef *hAD5142A,uint8_t RADC_Channel)
{
	uint8_t Control_Address = 0x20|RADC_Channel;//����λ�͵�ַλ
	AD5142A_TransmitData(hAD5142A,(((uint16_t)Control_Address)<<8)+0x00);
	delay_ms(20);
}

/*
	CmdNo 14
	�����λ
	[15:8]1 0 1 1 X X X X,[7:0]X X X X X X X X
*/
void AD5142A_sRST(AD5142A_HandleTypeDef *hAD5142A)
{
	AD5142A_TransmitData(hAD5142A,0xB000);
}

/*
	CmdNo 15
	����ض�
	[15:8]1 0 1 1 X X X X,[7:0]X X X X X X X X
	ENABLE:0-normal;1-shutdown,�ڹض�ģʽ�£�Raw��·��RwbΪ�ײ����裬Լ��ʮŷķ
*/
void AD5142A_sShutdown(AD5142A_HandleTypeDef *hAD5142A,uint8_t RADC_Channel,uint8_t ENABLE)
{
	uint8_t Control_Address = 0xC8|RADC_Channel;//����λ�͵�ַλ
	AD5142A_TransmitData(hAD5142A,(((uint16_t)Control_Address)<<8)+(ENABLE&0x01));
}

/*
	CmdNo 12
	��������
	[15:8]1 0 0 1 1 0 0 A0,[7:0]1 X X X X X X D0
	MODE:0-exit;1-enter,�ڶ�������ģʽ�£�RawΪ�������裬Լ��ʮŷķ��RwbԼΪRab���޸�RADC�Ĵ��������˳�
*/
void AD5142A_SetTopScale(AD5142A_HandleTypeDef *hAD5142A,uint8_t RADC_Channel,uint8_t MODE)
{
	uint8_t Control_Address = 0x98|RADC_Channel;//����λ�͵�ַλ
	AD5142A_TransmitData(hAD5142A,(((uint16_t)Control_Address)<<8)+((MODE|0x80)&0x81));
}

/*
	CmdNo 13
	�ײ�����
	[15:8]1 0 0 1 1 0 0 A0,[7:0]0 X X X X X X D0
	MODE:0-exit;1-enter,�ڵײ�����ģʽ�£�RbwΪ�ײ����裬Լ��ʮŷķ��RawԼΪRab���޸�RADC�Ĵ��������˳�
*/
void AD5142A_SetBottomScale(AD5142A_HandleTypeDef *hAD5142A,uint8_t RADC_Channel,uint8_t MODE)
{
	uint8_t Control_Address = 0x98|RADC_Channel;//����λ�͵�ַλ
	AD5142A_TransmitData(hAD5142A,(((uint16_t)Control_Address)<<8)+(MODE&0x01));
}

/******���³���Ϊ��չӦ�ó���******/

#ifdef __AD5142A_EXAPP_

/*
	��ֵ���ú���
	���ܣ�����Rwb�˵ĵ���
	����Resistance����Ϊ�����õ�Rwb��ֵ����λΪŷķ
*/
void AD5142A_SetResistance(AD5142A_HandleTypeDef *hAD5142A,uint8_t RADC_Channel,float Resistance)
{
	if(Resistance>(hAD5142A->Max_Resistance))
		Resistance=(hAD5142A->Max_Resistance);
	else if(Resistance<0)Resistance=0;
	AD5142A_WriteChannel(hAD5142A,RADC_Channel,(uint8_t)(255*Resistance/(hAD5142A->Max_Resistance)));
}

/*
	��ѹ�����ú���
	���ܣ�����Rwb�˵ĵ����������ʵ��W�˵����ѹ���
	����kDivide����Ϊ�����õķ�ѹϵ����k=Rwb/(Raw+Rwb)
*/
void AD5142A_SetResDivider(AD5142A_HandleTypeDef *hAD5142A,uint8_t RADC_Channel,float kDivide)
{
	if(kDivide>1)kDivide=1;
	else if(kDivide<0)kDivide=0;
	
	AD5142A_SetResistance(hAD5142A,RADC_Channel,kDivide*(hAD5142A->Max_Resistance));
}
#endif
