//-----------------------------------------------------------------
// LED����ͷ�ļ�
// ͷ�ļ���: LED.h
// ��    ��: ���ǵ���
// ��ʼ����: 2014-01-28
// �������: 2014-01-28
// �޸�����: 2014-02-16
// ��ǰ�汾: V1.0.1
// ��ʷ�汾:
//   - V1.0: (2014-02-07)LED IO ����
// - V1.0.1:(2014-02-16)ͷ�ļ��в���������ͷ�ļ�
//-----------------------------------------------------------------

#ifndef _MB1504_H
#define _MB1504_H
#define uchar unsigned char
#define uint unsigned int
//-----------------------------------------------------------------
// λ����
//-----------------------------------------------------------------
#define CLK_0 		GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define CLK_1 		GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define DATA_0	  GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define DATA_1    GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define LE_0		  GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define LE_1      GPIO_SetBits(GPIOA,GPIO_Pin_7)

//-----------------------------------------------------------------
// �ⲿ��������
//-----------------------------------------------------------------
extern void GPIO_LED_Configuration(void);
extern void Send16Bit(uint SendData);
extern void Send11Bit( uint SendData ) ;  
extern void Send8Bit( uchar SendData ) ; 
extern void SendReferF( void )  ;
extern void FrequenceToSend( uint FrequenceD ); 

#endif

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
