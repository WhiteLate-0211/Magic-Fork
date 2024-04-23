#ifndef	_DAC_INTERNAL_H
#define _DAC_INTERNAL_H
extern "C"{
	#include "stm32f4xx_dac.h"
}
#define MIN_GAIN 			(-2)	// �����õ���С����
#define MAX_GAIN 			(40)	// �����õ��������
#define AUTORELOADVALUE  	2000	// �趨
#define VREF  				3287

class	DAC_Internal
{
public:
	DAC_Internal();
	~DAC_Internal();
public:
	void AM_Change();
	void DAC_Reset(int AGAIN); 
private:
	void func_init();
};

#endif
