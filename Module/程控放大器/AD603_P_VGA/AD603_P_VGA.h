/*
AD603�̿�-ѹ�طŴ���_��������
Version 1.00

��������wrx��Ʋ�������AD603ѹ�طŴ���ģ�����������
ͨ������DACоƬTLC5615,ʵ��0-4Vֱ����ѹ�������ͨ������Ŵ��·ʵ�֡�750mV������Ƶ�ѹ�������ʵ�ֶ�����ĳ�����ơ�

������������TLC5615��������

���������л�Ϊ�ֶ����ơ�

***���������2021��ȫ����ѧ��������ƾ�������ʹ�ã������⴫***

(C)LWNF 2015-2021
*/
#ifndef __AD603_P_VGA_H
#define __AD603_P_VGA_H

#include "TLC5615.h"
#include "stdint.h"

typedef struct
{
	TLC5615_HandleTypeDef *hTLC5615;
	float gain_K;
}AD603_M_HandleTypeDef;

void AD603_Module_Init(AD603_M_HandleTypeDef *hAD603,TLC5615_HandleTypeDef *hTLC5615,float gain_K);
void AD603_Module_SetDigVoltage(AD603_M_HandleTypeDef *hAD603,float Vg);
void AD603_Module_SetDecibelGain(AD603_M_HandleTypeDef *hAD603,float DecGain);
void AD603_Module_SetLinearGain(AD603_M_HandleTypeDef *hAD603,float Au);
void AD603_Module_CalcDecibelGainLinearRange(float *Range,float gain_K);
float AD603_Module_CalcMaxLineaGain(float gain_K);

#endif
