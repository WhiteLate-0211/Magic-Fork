#include "SPI.h"
#include "LCD_CPP.h"
SPI::SPI()
{
	SPI1_Init();
}

SPI::~SPI()
{
	
}
/****
 *������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
 *SPI�ڳ�ʼ��
 *�������Ƕ�SPI1�ĳ�ʼ��
***/
void SPI::SPI1_Init()
{	 
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//ʹ��SPI1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	//GPIOFB3,4,5��ʼ�����ã�PB3~5���ù������	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	//�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//100MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	//����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//PB3����Ϊ SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); 
	//PB4����Ϊ SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); 
	//PB5����Ϊ SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); 

	//����ֻ���SPI�ڳ�ʼ������λSPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);
	//ֹͣ��λSPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);
	//����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  
	//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		
	//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		
	//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		
	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	
	//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		
	//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		
	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	
	//CRCֵ����Ķ���ʽ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	
	//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	SPI_Init(SPI1, &SPI_InitStructure);  
	//ʹ��SPI����
	SPI_Cmd(SPI1, ENABLE); 
	//����״̬��ƽ���Ǹߣ��൱�ڴ�������
	SPI1_ReadWriteByte(0xff);	 
}   
/****SPI1�ٶ����ú���
 *SPI�ٶ�=fAPB2/��Ƶϵ��
 *@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
 *fAPB2ʱ��һ��Ϊ84Mhz��
***/
void SPI::SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
	SPI1->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI1,ENABLE); //ʹ��SPI1
} 
/****
 *SPI1 ��дһ���ֽ�
 *TxData:Ҫд����ֽ�
 *����ֵ:��ȡ�����ֽ�
***/
u8 SPI::SPI1_ReadWriteByte(u8 TxData)
{		 			 
 //�ȴ���������  
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}
	//ͨ������SPIx����һ��byte  ����
	SPI_I2S_SendData(SPI1, TxData); 
  //�ȴ�������һ��byte
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){}   
	//����ͨ��SPIx������յ�����	
	return SPI_I2S_ReceiveData(SPI1); 
}
