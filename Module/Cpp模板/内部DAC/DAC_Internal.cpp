#include "DAC_Internal.h"

DAC_Internal::DAC_Internal()
{
	func_init();
}

DAC_Internal::~DAC_Internal()
{
	
}

void DAC_Internal::func_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	//ʹ��GPIOAʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//ʹ��DACʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	//Analog Mode ģ��ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	//����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	//��ʼ��
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//��ʹ�ô������� TEN1=0
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	
	//��ʹ�ò��η���
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;
	//���Ρ���ֵ����
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;
	//DAC1�������ر� BOFF1=1
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;
	//��ʼ��DACͨ��1	
	DAC_Init(DAC_Channel_1,&DAC_InitType);	 
	//ʹ��DACͨ��1
	DAC_Cmd(DAC_Channel_1, ENABLE);  
	//12λ�Ҷ������ݸ�ʽ����DACֵ
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);  
}

void DAC_Internal::DAC_Reset(int AGAIN)
{
	if(AGAIN<14)
		DAC_SetChannel1Data(DAC_Align_12b_R, (unsigned long int)((22.5*AGAIN+133)*4095/VREF));
	else if(AGAIN>=15 && AGAIN<=23)	
		DAC_SetChannel1Data(DAC_Align_12b_R, (unsigned long int)((22*AGAIN+133)*4095/VREF));
	else if(AGAIN>=24 && AGAIN<=29)
		DAC_SetChannel1Data(DAC_Align_12b_R, (unsigned long int)((21.8*AGAIN+133)*4095/VREF));
	else if(AGAIN>=30&& AGAIN<=34)
		DAC_SetChannel1Data(DAC_Align_12b_R, (unsigned long int)((21.5*AGAIN+133)*4095/VREF));
	else if(AGAIN>=35&& AGAIN<=MAX_GAIN)
		DAC_SetChannel1Data(DAC_Align_12b_R, (unsigned long int)((21.4*AGAIN+133)*4095/VREF));
}

