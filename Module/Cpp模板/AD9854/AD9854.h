/*
 **********************************************************************
 *Copyright(c)		Lance Zhang
 *Identification	CHIC
 *Describtion		Template
 *
 *Present Version	1.0
 *Writer			Lance Zhang
 *Date				7/15/2019
 **********************************************************************
**/
//**********************����Ϊϵͳʱ���Լ�����ر�������**************************
/* 
      �˴������Լ�����Ҫ����ϵͳʱ���Լ�������ص����ӣ�һ������ֻ�迪��һ��   
      CLK_SetΪʱ�ӱ�Ƶ���ã�������4~20����Ƶ��������ܳ���300MHZ
      Freq_mult_ulong��Freq_mult_doulle��Ϊ2��48�η�����ϵͳʱ�ӣ�һ��Ϊ�����Σ�һ��Ϊ˫������
	  
							@���³������ھ���Ϊ20MHzʱ�����@
*/
#define ulong 	unsigned long 

/*
#define      CLK_Set            4
const ulong  Freq_mult_ulong  = 3518437;
const double Freq_mult_doulle = 3518437.2088832;
*/

/*
#define      CLK_Set            5
const ulong  Freq_mult_ulong  = 2814750;
const double Freq_mult_doulle = 2814749.76710656;
*/

 /*
#define      CLK_Set            6
const ulong  Freq_mult_ulong  = 2345625;
const double Freq_mult_doulle = 2345624.80592213;
  */

/*
#define      CLK_Set            7
const ulong  Freq_mult_ulong  = 2010536;
const double Freq_mult_doulle = 2010535.54793326;
*/

/*
#define      CLK_Set            8
const ulong  Freq_mult_ulong  = 1759219;
const double Freq_mult_doulle = 1759218.6044416;
*/

/*
#define      CLK_Set            9
const ulong  Freq_mult_ulong  = 1563750;
const double Freq_mult_doulle = 1563749.87061476;
*/

/*
#define      CLK_Set            10
const ulong  Freq_mult_ulong  = 1407375;
const double Freq_mult_doulle = 1407374.88355328;
*/
/*
#define      CLK_Set            11
const ulong  Freq_mult_ulong  = 1279432;
const double Freq_mult_doulle = 1279431.712321164;
*/

/*
#define      CLK_Set            12
const ulong  Freq_mult_ulong  = 1172812;
const double Freq_mult_doulle = 1172812.402961067;
*/

/*
#define      CLK_Set            13
const ulong  Freq_mult_ulong  = 1082596;
const double Freq_mult_doulle = 1082596.064271754;
*/

/*
#define      CLK_Set            14
const ulong  Freq_mult_ulong  = 1005268;
const double Freq_mult_doulle = 1005267.773966629;
*/


#define      CLK_Set            15
const ulong  Freq_mult_ulong  = 938250;
const double Freq_mult_doulle = 938249.9223688533;


#ifndef __AD9854_H
#define __AD9854_H
extern "C"
{
	#include "stm32f4xx_gpio.h"
	#include "sys.h"
	#include "delay.h"
	#include "usart.h"
}

//����PBΪ���
#define uint unsigned int
#define ulong unsigned long	
#define uchar unsigned char
//������������
#define DDS_RESET 	PCout(6)	//��λ
#define DDS_WR 		PCout(7)	//д��
#define DDS_RD 		PCout(8)	//��ȡ
#define UCLK 		PCout(9)	//Ƭѡ
//��ʼ���˿�����
#define AD9854_Data_Port	GPIOE
#define AD9854_Addr_Port	GPIOD
#define AD9854_Ctr_Port		GPIOC
//ʱ�Ӷ˿�
#define AD9854_Ctr_CLK		RCC_AHB1Periph_GPIOC
#define AD9854_Addr_CLK		RCC_AHB1Periph_GPIOD
#define AD9854_Data_CLK		RCC_AHB1Periph_GPIOE
//��������
#define AD9854_Data_Pin		GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | \
							GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15
//��ַ����
#define AD9854_Addr_Pin		GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |	\
							GPIO_Pin_4 | GPIO_Pin_5
//��������
#define AD9854_Ctr_Pin		GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9
//ģʽ����
#define Stone 	0x00
#define RFsk 	0x24

class	AD9854
{
//�������������
public:
	AD9854();
	~AD9854();

//�û����õĺ���
public:
	//��ȡָ���Ĵ����е�����
	u8 read_data(u8 addr);
	//��ָ���Ĵ�����д������
	void write_data(u8 addr,u8 data);
	//MASTER RESET����λ���мĴ���
	void reset();
	//�ı��������Ƶ�����趨��Freq
	void Freq_Reset(long Freq);
	//���ڸ��¼Ĵ�����Ϣ
	void Reg_Update();
	//ɨƵ����,Ĭ�ϴ�1KHz��ʼ����4KHz����������1Hz�����ʱ��5000us,ɨ5������
	void Freq_Scan(long freq_low = 1000, long freq_high = 4000, long  steps = 1, u32 intervals = 5000, u32 circles = 5);
//�����ú��������û�������
public:	
	//���ƼĴ�����ʼ��[0x1d : 0x20]
	//[0x1d]�رձȽ���
	//[0x1e]��Ĭ�ϳ�ʼ״̬10��Ƶ
	//[0x20]����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����
	void Ctr_Reg_Init(u8 addr_1d = 0x00, u8 addr_1e = CLK_Set, 
						u8 addr_1f = 0x00, u8 addr_20 = 0x60);
	//Ƶ�ʿ�����1��ʼ��,��Ĭ��ֵ��1000Hz
	void FTW1_Init(long Freq = 1000);
	//���ȿ�����I/Q��ʼ��
	void AM_Init(u8 iam_h = 0xef, u8 iam_l = 0xff,
					u8 qam_h = 0xef, u8 qam_l = 0xff);
	//оƬ�����ʼ��
	void Func_Init();

private:
	//��ʼ��AD9854��IO��
	void AD9854_IOinit();
	//���ݰ�λ����
	u8 Data_In(void);
	//���ݰ�λ���
	void Data_Out(u8 data);
	//Ƶ��ת����������48λFTW1Ƶ�ʿ����ֲ��6��
	void Freq_convert(long Freq);
};

#endif
