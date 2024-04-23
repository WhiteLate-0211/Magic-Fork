#include "CFFT.h"

CFFT::CFFT()
{
	factor_ave = 0;
	max_val = 0;
	min_val = 0;
	freq_val = 0;	
	AM_val = 0;
	DC_val = 0;
	
	debug_max_loc = 0;
	debug_max_val = 0;
	debug_str_val = 0;
	debug_mid_val = 0;
	debug_ave_val = 0;

	debug_freq = 0;
}

CFFT::~CFFT()
{
	
}
//����fft_inputbuf[]���飬���������ĵ�ѹֵ���븵��Ҷ��������
void CFFT::Gen_Input(u16* vol_input, float* fft_input,u16 size)
{
	for(int i=0;i<size;i++)									//�����ź�����
	{
		fft_input[2*i] = (vol_input[i]/4096.0)*3.3;			//���������ź�ʵ��
		fft_input[2*i+1]=0;									//�鲿ȫ��Ϊ0
	}
}
//����Ҷ�任
void CFFT::FFT(float* fft_inputbuf,float* fft_outputbuf,u16 input_size)
{
	arm_cfft_radix4_instance_f32 	scfft;
	arm_cfft_radix4_init_f32(&scfft,input_size,0,1);
	arm_cfft_radix4_f32(&scfft,fft_inputbuf);					//FFT���㣨��4��
	arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,input_size);	//��������������ģ�÷�ֵ 		
}
//����Ҷ��任
void CFFT::IFFT(float* fft_inputbuf,float* fft_outputbuf,u16 input_size)
{
	arm_cfft_radix4_instance_f32 	scfft;
	arm_cfft_radix4_init_f32(&scfft,input_size,1,1);
	arm_cfft_radix4_f32(&scfft,fft_inputbuf);					//FFT���㣨��4��
	arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,input_size);	//��������������ģ�÷�ֵ 
}
//����FFT��������飬�������Ƶ��
void CFFT::output_scan(float* fft_output, float Fs, u16 size)
{
	//��ȡƵ��ϵ��freq_temp = (n-1)/N
	float freq_temp = 0;	
	float am_temp = 0;
	//�Ȼ�ȡֱ������
	float dc_value = fft_output[0];
	//�õ����ֵ���ڵ�λ��
	u32 max_location;
	
	//���ֱ������
	DC_val = dc_value/size;
	
	//��ֱ������ȥ��,����ǵüӻ�ȥ
	fft_output[0] = 0;
	//��ǰһ�������е����ֵ
	arm_max_f32(fft_output,512,&am_temp,&max_location);
	//��ֱ��������ԭ
	fft_output[0] = dc_value;
	//��Ƶ��,��ֵ
	freq_temp = (float)max_location/size;
	//�����жϣ���������źŵķ�ֵС����max_val/(size/2),����Ϊֱ������
	//size = 1024�������Ϊ90mV
	if(fft_output[max_location] <= 500)
	{
		freq_val = 0;
		debug_freq = 0;
	}
	else
	{
		freq_val = freq_temp*Fs;
		debug_freq = 1;
	}
	AM_val = 2*(am_temp/size);
	//�����Сֵ
	max_val = DC_val + AM_val;
	min_val = DC_val - AM_val;
}

void CFFT::debug_output_scan(float* fft_output, float Fs, u16 size)
{
	//��ȡƵ��ϵ��freq_temp = (n-1)/N
	float freq_temp = 0;	
	float am_temp = 0;
	//�Ȼ�ȡֱ������
	float dc_value = fft_output[0];
	debug_str_val = dc_value;
	//�õ����ֵ���ڵ�λ��
	u32 max_location;
	
	//���ֱ������
	//DC_val = dc_value/size;
	
	//��ֱ������ȥ��,����ǵüӻ�ȥ
	fft_output[0] = 0;
	//��ǰһ�������е����ֵ
	arm_max_f32(fft_output,512,&am_temp,&max_location);
	//��ֱ��������ԭ
	fft_output[0] = dc_value;
	//�õ����ֵ������
	debug_max_val = fft_output[max_location];
	debug_max_loc = max_location;
	
	//��Ƶ��,��ֵ�������Сֵ
	freq_temp = (float)max_location/size;
	freq_val = freq_temp*Fs;
	AM_val = 2*(am_temp/size);
	max_val = DC_val + AM_val;
	min_val = DC_val - AM_val;
}

//��ֵ�˲�
void CFFT::blur(float* fft_output, u16 size, float bias)
{
	//��ƽ��ֵ,��þ�ֵ����
	arm_mean_f32(fft_output,512,&factor_ave);
	
	for(int i=0;i<size;i++)
	{
		if(factor_ave + bias > fft_output[i])
		{
			fft_output[i] = 0;
		}			
	}
}
