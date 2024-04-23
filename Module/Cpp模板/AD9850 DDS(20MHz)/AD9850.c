#include "AD9850.h"
#include "sys.h"
#include "delay.h"
/*8080����out*/
void Data_Out(u8 data)
{
	B8080_Init(GPIO_Mode_OUT);  			//���ģʽ
	GPIOE->ODR&=~(0XFF<<8);		//���8~15
	GPIOE->ODR|=data<<8;		//���ݴ�����8λ

	GPIO_Write(GPIOE,data<<8);
} 

/*8080����In*/
u8 Data_In(void)
{ 
	B8080_Init(GPIO_Mode_IN);  //����ģʽ
	u8 dat;
 
	dat=(GPIOE->IDR)>>8;
	return dat;
}

// GPIO_Mode_IN   = 0x00, 
// GPIO_Mode_OUT  = 0x01,
void B8080_Init(GPIOMode_TypeDef mode)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15 ;
	GPIO_InitStructure.GPIO_Mode = mode;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);				//��ʼ��

}

void AD9850_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_WCLK,ENABLE);
	GPIO_InitStructure.GPIO_Pin = PIN_WCLK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(GPIO_WCLK, &GPIO_InitStructure);			//��ʼ��

	RCC_AHB1PeriphClockCmd(RCC_FQUP,ENABLE);
	GPIO_InitStructure.GPIO_Pin = PIN_FQUP ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(GPIO_FQUP, &GPIO_InitStructure);			//��ʼ��

	RCC_AHB1PeriphClockCmd(RCC_REST,ENABLE);
	GPIO_InitStructure.GPIO_Pin =PIN_REST ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(GPIO_REST, &GPIO_InitStructure);			//��ʼ��
	WCLK_L;
	FQUP_L;
	REST_L;
}
//��λ
void AD9850_Rest()
{
  WCLK_L;
  FQUP_L;
  REST_L;
  REST_H;
  delay_us(5);
	REST_L;
}

//������λ��Ƶ��(w0�а���5λ��λ��1λ���磬2λ����)
void AD9850_SetOut(double frequence,u8 w0)
{
  u8 w;
  u32 y,x;
//����Ƶ�ʵ�HEXֵ
  x=4294967295/125;//�ʺ�125M����
//���ʱ��Ƶ�ʲ�Ϊ125MHZ���޸ĸô���Ƶ��ֵ����λMHz
  frequence=frequence/1000000;
  frequence=frequence*x;
  y=frequence;
	//дw0����
  w=w0; 
  Data_Out(w0);
	WCLK_H;
	WCLK_L;
	//дw1����
  w=(y>>24);
	Data_Out(w);
	WCLK_H;
	WCLK_L;
	//дw2����
  w=(y>>16);
	Data_Out(w);
	WCLK_H;
	WCLK_L;
	//дw3����
  w=(y>>8);
	Data_Out(w);
	WCLK_H;
	WCLK_L;
	//дw4����
  w=y;
	Data_Out(w);
	WCLK_H;
	WCLK_L;
	//����ʼ��
	FQUP_L;
	FQUP_H;
	delay_us(5);
	FQUP_L;
	//Data_Out(y<<8);
}
