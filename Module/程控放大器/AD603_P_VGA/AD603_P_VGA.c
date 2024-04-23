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

/*
AD603�������
Gain(dB)=40*V_gain+gain_K
*/

#include "AD603_P_VGA.h"
#include "TLC5615.h"

#include "stdint.h"
#include "math.h"

void AD603_Module_Init(AD603_M_HandleTypeDef *hAD603,TLC5615_HandleTypeDef *hTLC5615,float gain_K)
{
	/*ָ��AD603ģ����ʹ�õ�TLC5615*/
	hAD603->hTLC5615=hTLC5615;
	/*ָ��AD603ģ��������泣����*/
	hAD603->gain_K=gain_K;
}

void AD603_Module_SetDigVoltage(AD603_M_HandleTypeDef *hAD603,float Vg)
{
	uint16_t Din;
	Din=(uint16_t)(1024*(Vg+0.75f)/1.5f);
	if(Din>1023)Din=1023;
	TLC5615_SetDigtalValue(hAD603->hTLC5615,Din);
}

/*����ģ��ֱ����溯��*/
void AD603_Module_SetDecibelGain(AD603_M_HandleTypeDef *hAD603,float DecGain)
{
	/*�ж������Ƿ���������Է�Χ�ڲ�����Լ��*/
	if(DecGain<((hAD603->gain_K)-20))DecGain=(hAD603->gain_K)-20;
	else if(DecGain>((hAD603->gain_K)+20))DecGain=(hAD603->gain_K)+20;
	
	/*���������ѹ*/
	float Vgain=(DecGain-(hAD603->gain_K))/40;
	/*���������ѹ*/
	AD603_Module_SetDigVoltage(hAD603,Vgain);
}

/*����ģ���������溯��*/
void AD603_Module_SetLinearGain(AD603_M_HandleTypeDef *hAD603,float Au)
{
	/*��ֱ�����*/
	float DecGain=20*log10(Au);
	/*���������ѹ*/
	float Vgain=(DecGain-(hAD603->gain_K))/40;
	/*���������ѹ*/
	AD603_Module_SetDigVoltage(hAD603,Vgain);
}

/*����ģ��ֱ��������Է�Χ����
*ͨ����������ֱ�Ϊ���޺����ޣ����泣��gain_K��СΪ10
*/
void AD603_Module_CalcDecibelGainLinearRange(float *Range,float gain_K)
{
	if(gain_K<10)
	{
		*Range=-999;*(Range+1)=-999;
		return;
	}

	if(gain_K+20>60)
	{
		*(Range+1)=60;
	}
	else{*(Range+1)=gain_K+20;}

	if(gain_K-20<-10) *Range=-10;
	else *Range=gain_K-20;
	return;
}

/*����ģ������������溯��
*����ֵΪ������棬���泣��gain_K��СΪ10
*/
float AD603_Module_CalcMaxLineaGain(float gain_K)
{
  float range[2];
  AD603_Module_CalcDecibelGainLinearRange(range,gain_K);
	if(range[0]==range[1])return 0;
  else return pow(10,range[1]/20);
}
