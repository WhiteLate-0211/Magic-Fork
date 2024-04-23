/**
 **********************************************************************
 *@Copyright(c)		Lance Zhang
 *@Identification	CHIC
 *@Describtion		Template
 *
 *@PresentVersion	1.2
 *@Writer			Lance Zhang
 *@Date				7/10/2019
 **********************************************************************/
/**********************************************************************
 *@Instruction
 *����ʹ�÷�����
 *
 *1������Gen_Input()���������ɼ����ĵ�ѹֵ(int)
 *	 ת��Ϊ��FFT����ʹ�õ���������
 *
 *2������FFT()������������������и�������Ҷ�任�����
 *
 *3������output_scan()���������ԶԸ���Ҷ�任�Ժ���������ɨ��
 *	 ������������Ļ�������
 **********************************************************************/
/**********************************************************************
 *@Attention		��������Ĵ�СӦ����������С��2��
 *@Attention		���ڲ��û�4���㣬������������ӦΪ4��������
 *@Attention		��ν����̬ѧ�˲���ͬ��������Ҫ�������ã������Լ�д
 **********************************************************************
 *@Compatibility	������arm_math.h��û�����
 *
 *��������ԣ���
 *
 *@Characteristic	output_scan()������Ҫ�����������
 *					ƽ��ֵ�����ֵ����Сֵ��Ƶ��					
 *
 *@Warning	#include "stm32f4xx.h"�������arm_math.h֮ǰ���г�ʼ��
 *			�����������¾��棺
 **********************************************************************
 *		#warning "Compiler generates FPU instructions for a device 
 *					without an FPU (check __FPU_PRESENT)"
 **********************************************************************/
        
#ifndef	_CFFT_H
#define _CFFT_H
extern "C"
{
	#include "stm32f4xx.h"
	#include "arm_math.h"
	#include "sys.h"
}

class	CFFT
{
public:
	CFFT();
	~CFFT();
public:
	//����Ҷ��任
	void IFFT(float* fft_inputbuf,float* fft_outputbuf,u16 input_size = 1024);
	//����Ҷ�任
	void FFT(float* fft_inputbuf,float* fft_outputbuf,u16 input_size = 1024);
public:
	void Gen_Input(u16* vol_input, float* fft_input,u16 size = 1024);
	//ɨ�踵��Ҷ�任����������õ�ƽ��ֵ�����ֵ����Сֵ��Ƶ�ʵ���Ϣ
	void output_scan(float* fft_output, float Fs = 50000, u16 size = 1024);
//ԭ�ź�״̬��Ŀǰ��F��΢׼һ��
public:
	float max_val;
	float min_val;
	float freq_val;
	float AM_val;
	float DC_val;
public:
	void debug_output_scan(float* fft_output, float Fs = 50000, u16 size = 1024);
	u16 debug_max_loc;
	u16 debug_max_val;
	u16 debug_str_val;
	float debug_mid_val;
	u16 debug_ave_val;
	u16 debug_freq;
//��̬ѧ�˲�
public:
	//��ֵ�˲�,Ĭ��ƫ��Ϊ0
	void blur(float* fft_output,u16 size = 1024, float bias = 0);
//��̬ѧ����
private:
	//��ֵ�˲�����
	float factor_ave;
};

/**
 **********************************************************************
 *@Copyright(c)		Lance Zhang
 *@Identification	CHIC
 *@Describtion		Template
 *
 *@PresentVersion	1.2
 *@Writer			Lance Zhang
 *@Date				7/10/2019
 **********************************************************************/
/************************************************* *********************
 *@Instruction
 *Basic use:
 *
 *1, call the Gen_Input() function, and the collected voltage value (int)
 * Converted to an input array for FFT calculations
 *
 *2, call the FFT () function, perform complex Fourier transform on the input array and output
 *
 *3, call the output_scan () function, you can scan the array after the Fourier transform
 * and get the basic characteristics of the output array
 ************************************************** ********************/
/************************************************* *********************
 *@Attention The size of the input array should be twice the size of the output array
 *@Attention The input and output arrays should be integer multiples of 4 due to base 4 operations.
 *@Attention The so-called morphological filtering requires specific 
 *			 configuration in different environments, try to write by yourself
 ************************************************** ********************
 *@Compatibility called arm_math.h, nothing else
 *
 *@Overall compatibility: high
 *
 *@Characteristic output_scan() function main request output array
 * Average, maximum, minimum, frequency
 *
 *@Warning #include "stm32f4xx.h" must be initialized before arm_math.h
 * Otherwise the following warning will appear:
 **********************************************************************
 *		#warning "Compiler generates FPU instructions for a device 
 *					without an FPU (check __FPU_PRESENT)"
 **********************************************************************/
 
 /**
  **********************************************************************
  * @Copyright(c) ���ѧߧ� ���اѧ�
  * @���է֧ߧ�ڧ�ڧܧѧ�ڧ� CHIC
  * @Describtion ��ѧҧݧ�ߧ�
  *
  * @���ѧ�����ѧ� �ӧ֧��ڧ� 1.2
  * @���ڧ�ѧ�֧ݧ� ���ѧߧ� ���اѧ�
  * @���ѧ�� 7/10/2019
  **********************************************************************/
/**********************************************************************
 * @���ߧ����ܧ�ڧ�
 * ����ߧ�ӧߧ�� �ڧ���ݧ�٧�ӧѧߧڧ�:
 *
 * 1, �ӧ�٧�ӧڧ�� ���ߧܧ�ڧ� Gen_Input () �� ���ݧ��֧ߧߧ�� �٧ߧѧ�֧ߧڧ� �ߧѧ���ا֧ߧڧ� (int)
 * ����֧�ҧ�ѧ٧�ӧѧ� �ӧ� �ӧ��էߧ�� �ާѧ��ڧ� �էݧ� ��ѧ��֧��� ������
 *
 * 2, �ӧ�٧ӧѧ�� ���ߧܧ�ڧ� FFT (), �ӧ���ݧߧڧ�� �ܧ�ާ�ݧ֧ܧ�ߧ�� ���֧�ҧ�ѧ٧�ӧѧߧڧ� ������� �էݧ� �ӧ��էߧ�ԧ� �ާѧ��ڧӧ� �� �ӧ�ӧ֧���
 *
 * 3, �ӧ�٧�ӧڧ�� ���ߧܧ�ڧ� output_scan (), �ӧ� �ާ�ا֧�� ��ܧѧߧڧ��ӧѧ�� �ާѧ��ڧ� ����ݧ� ���֧�ҧ�ѧ٧�ӧѧߧڧ� �������
 * �� ���ݧ��ڧ�� ���ߧ�ӧߧ�� ��ѧ�ѧܧ�֧�ڧ��ڧܧ� �ӧ���էߧ�ԧ� �ާѧ��ڧӧ�
 **********************************************************************/
/************************************************* *********************
 * @���ߧڧާѧߧڧ� ���ѧ٧ާ֧� �ӧ��էߧ�ԧ� �ާѧ��ڧӧ� �է�ݧا֧� �ҧ��� �� �էӧ� ��ѧ٧� �ҧ�ݧ��� �ӧ���էߧ�ԧ� �ާѧ��ڧӧ�
 * @���ߧڧާѧߧڧ� ���ѧ��ڧӧ� �ӧӧ�է� �� �ӧ�ӧ�է� �է�ݧاߧ� �ҧ��� ��֧ݧ�ާ� ��ڧ�ݧѧާ�, �ܧ�ѧ�ߧ�ާ� 4, �ڧ�-�٧� ���֧�ѧ�ڧ� �� �ҧѧ٧�� 4.
 * @���ߧڧާѧߧڧ� ���ѧ� �ߧѧ٧�ӧѧ֧ާѧ� �ާ����ݧ�ԧڧ�֧�ܧѧ� ��ڧݧ���ѧ�ڧ� ���֧ҧ�֧� ����֧է֧ݧ֧ߧߧ�� �ܧ�ߧ�ڧԧ��ѧ�ڧ� �� ��ѧ٧ߧ�� ���֧էѧ�, ������ҧ�ۧ�� �ߧѧ�ڧ�ѧ�� ��ѧާ������֧ݧ�ߧ�
 ************************************************** ********************
 * @����ӧާ֧��ڧާ���� �ߧѧ٧�ӧѧ֧��� arm_math.h, �ҧ�ݧ��� �ߧڧ�֧ԧ�
 *
 * ���ҧ�ѧ� ���ӧާ֧��ڧާ����: �ӧ���ܧѧ�
 *
 * @���ѧ�ѧܧ�֧�ڧ��ڧܧ� output_scan () ���ߧܧ�ڧ� �ӧ���էߧ�ԧ� �ާѧ��ڧӧ� ���ߧ�ӧߧ�ԧ� �٧ѧ�����
 * ����֧էߧ��, �ާѧܧ�ڧާѧݧ�ߧѧ�, �ާڧߧڧާѧݧ�ߧѧ�, ��ѧ�����
 *
 * @����֧է���֧اէ֧ߧڧ� #include "stm32f4xx.h" �է�ݧاߧ� �ҧ��� �ڧߧڧ�ڧѧݧڧ٧ڧ��ӧѧߧ� �է� arm_math.h
 * �� �����ڧӧߧ�� ��ݧ��ѧ� ����ӧڧ��� ��ݧ֧է���֧� ���֧է���֧اէ֧ߧڧ�:
 *
 **********************************************************************
 *		#warning "Compiler generates FPU instructions for a device 
 *					without an FPU (check __FPU_PRESENT)"
 **********************************************************************/
#endif
