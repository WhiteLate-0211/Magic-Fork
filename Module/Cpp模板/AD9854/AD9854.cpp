#include "AD9854.h"

u8 ad9854_freq[6];
u8 FreqWord[6];

AD9854::AD9854()
{
	delay_init(168);
	AD9854_IOinit();
	Func_Init();
}
//����Լ��ĳ������й��������
//��������ŵ�JTAG/SW���ù��ܵĲ���
//����ͨ���������������ܸ�ԭ
AD9854::~AD9854()
{
	
}
//��ʼ��AD9854��IO��
void AD9854::AD9854_IOinit()
{
	//��ʼ��AD9854�ĵ�ַ��
	GPIO_InitTypeDef  Addr_InitStructure;
	RCC_AHB1PeriphClockCmd(AD9854_Addr_CLK, ENABLE);
	Addr_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	Addr_InitStructure.GPIO_OType = GPIO_OType_PP;
	Addr_InitStructure.GPIO_Pin = AD9854_Addr_Pin;
	Addr_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	Addr_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(AD9854_Addr_Port,&Addr_InitStructure);
	//��ʼ��AD9854�Ŀ�������
	GPIO_InitTypeDef  Ctr_InitStructure;
	RCC_AHB1PeriphClockCmd(AD9854_Ctr_CLK, ENABLE);
	Ctr_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	Ctr_InitStructure.GPIO_OType = GPIO_OType_PP;
	Ctr_InitStructure.GPIO_Pin = AD9854_Ctr_Pin;
	Ctr_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	Ctr_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(AD9854_Ctr_Port,&Ctr_InitStructure);
}
//Master Reset����DDS��������λ�����мĴ�����ԭ
void AD9854::reset()
{
	DDS_RESET = 0;
	DDS_RESET = 1;
	delay_ms(30);
	DDS_RESET = 0;
	
	DDS_WR = 1;
	DDS_RD = 1;
	UCLK = 1;
}
//��ָ���ļĴ�����д������
void AD9854::write_data(u8 addr,u8 data)
{
	//��д��ֹ
	DDS_WR = 1;
	DDS_RD = 1;
	//д���ַ
	GPIO_Write(GPIOD,addr);
	//д������
	Data_Out(data);
	//дʹ��
	DDS_WR = 0;
	//дʧ��
	DDS_WR = 1;	
}
//��ȡָ���Ĵ���������
u8 AD9854::read_data(u8 addr)
{
	u8 data;
	//�ٴζ�ȡ����дʧ��
	DDS_WR = 1;
	DDS_RD = 1;
	//д���ַ
	GPIO_Write(GPIOD,addr);
	//��ʹ��
	DDS_RD = 0;
	//��ȡ����
	data = Data_In();
	//��ʧ��
	DDS_RD = 1;	
	printf("\r\n address[0x%02x] = 0x%02x; \r\n",addr,data);
	return data;
}
//���ƼĴ�����ʼ��[0x1d : 0x20]
void AD9854::Ctr_Reg_Init(u8 addr_1d, u8 addr_1e, u8 addr_1f, u8 addr_20)
{
	write_data(0x1d,addr_1d);
	write_data(0x1e,addr_1e);
	write_data(0x1f,addr_1f);
	write_data(0x20,addr_20);	
	Reg_Update();
}
//Ƶ�ʿ�����1��ʼ��
void AD9854::FTW1_Init(long Freq)
{
	Freq_convert(Freq);
	write_data(0x04,FreqWord[5]);
	write_data(0x05,FreqWord[4]);
	write_data(0x06,FreqWord[3]);
	write_data(0x07,FreqWord[2]);
	write_data(0x08,FreqWord[1]);
	write_data(0x09,FreqWord[0]);	
	Reg_Update();
}
//���ȿ�����I/Q��ʼ��
void AD9854::AM_Init(u8 iam_h, u8 iam_l, u8 qam_h, u8 qam_l)
{
	write_data(0x21,iam_h);
	write_data(0x22,iam_l);
	write_data(0x23,qam_h);
	write_data(0x24,qam_l);	
	Reg_Update();
}
//ˢ�¼Ĵ������ݣ���ʹ����Int Update Clockλ����Ҫ��
void AD9854::Reg_Update()
{
	UCLK = 0;
	delay_ms(1);
	UCLK = 1;
}
//оƬ�����ʼ��
void AD9854::Func_Init()
{
	reset();
	Ctr_Reg_Init();
	FTW1_Init();
	AM_Init();	
	Reg_Update();
}
//Ƶ��ת����������48λFTW1Ƶ�ʿ����ֲ��6��
void AD9854::Freq_convert(long Freq)   
{
    ulong FreqBuf;
    ulong Temp=Freq_mult_ulong;   	       
	//������Ƶ�����ӷ�Ϊ�ĸ��ֽ�
	uchar Array_Freq[4];			     
	Array_Freq[0]=(uchar)Freq;
	Array_Freq[1]=(uchar)(Freq>>8);
	Array_Freq[2]=(uchar)(Freq>>16);
	Array_Freq[3]=(uchar)(Freq>>24);

	FreqBuf=Temp*Array_Freq[0];                  
    FreqWord[0]=FreqBuf;    
    FreqBuf>>=8;

    FreqBuf+=(Temp*Array_Freq[1]);
    FreqWord[1]=FreqBuf;
    FreqBuf>>=8;

    FreqBuf+=(Temp*Array_Freq[2]);
    FreqWord[2]=FreqBuf;
    FreqBuf>>=8;

	FreqBuf+=(Temp*Array_Freq[3]);
    FreqWord[3]=FreqBuf;
    FreqBuf>>=8;

    FreqWord[4]=FreqBuf;
    FreqWord[5]=FreqBuf>>8;			
}  

void AD9854::Freq_Reset(long Freq)
{
	Freq_convert(Freq);
	write_data(0x04,FreqWord[5]);
	write_data(0x05,FreqWord[4]);
	write_data(0x06,FreqWord[3]);
	write_data(0x07,FreqWord[2]);
	write_data(0x08,FreqWord[1]);
	write_data(0x09,FreqWord[0]);

	Reg_Update();
}
//ɨƵ����,Ĭ�ϴ�1KHz��ʼ����4KHz����������1Hz�����ʱ��5000us,ɨ5������
void AD9854::Freq_Scan(long freq_low, long freq_high, long  steps, u32 intervals, u32 circles)
{
//	u8 flag = 0;
	u32 count = 0;
	long frequence = freq_low;
	Freq_Reset(frequence);
	
	while(1)
	{		
		Freq_Reset(frequence);		
//		if(flag == 0)
//		{
		frequence += steps;
//		}
//		if(flag == 1)
//		{
//			frequence -= steps;
//		}
		if(frequence == freq_high | frequence == freq_low)
		{
//			flag = !flag;
			count += 1;
//			if(count == circles)
//			{
//				Freq_Reset(frequence);	
//				break;
//			}
			frequence = freq_low;
		}
		delay_ms(intervals);	
	}	
}
/*************************************************************˽�г�Ա����*************************************************************/
void AD9854::Data_Out(u8 data)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIOE->ODR &= ~(0XFF<<8);		//ֻ���PE[8:15]
	GPIOE->ODR |= data<<8;			//PE[8:15]��������

} 

u8 AD9854::Data_In(void)
{ 
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	u8 dat;
	dat=(GPIOE->IDR)>>8;
	return dat;
}
