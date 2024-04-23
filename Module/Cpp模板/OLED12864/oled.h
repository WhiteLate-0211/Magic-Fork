/*
 **********************************************************************
 *Copyright(c)		Lance Zhang
 *Identification	CHIC
 *Describtion		Template
 *
 *Present Version	1.0
 *Writer			Lance Zhang
 *Date				8/1/2018
 **********************************************************************/
/**
 **********************************************************************
 *@Attention
 *		delay.h
 *		stm32f4xx_gpio.h
 *@PinPort:
 *		DO --------->	PB8
 *		DI --------->	PB9
 *		RES -------->	PB6
 *		DC --------->	PB3
 *		CS --------->	PB7
 **********************************************************************
 *���ܲ����ĳ�ͻ��
 *				����
 **********************************************************************
**/
#ifndef __OLED_H
#define __OLED_H
extern "C"{
#include "sys.h"
#include "stdlib.h"		
}
    
//////////////////////////////////////////////////////////////////////////////////	 
 
////////////////////////////////////////////////////////////////////////////////// 	  
 

//OLEDģʽ����
//0: SPIģʽ  ��ģ���BS1��BS2����GND��
//1: ����8080ģʽ ��ģ���BS1��BS2����VCC��
//2: I2Cģʽ ��ģ���BS1��BS2����VCC��
#define SPI		0
#define B8080	1
#define I2C		2

#define OLED_MODE  SPI 

#define FONT_LARGE		24
#define FONT_MID		16
#define FONT_SMALL		12
//-----------------OLED�˿ڶ���---------------- 
#define OLED_CS_CLOCK     RCC_AHB1Periph_GPIOB
#define OLED_RST_CLOCK    RCC_AHB1Periph_GPIOB
#define OLED_RS_CLOCK     RCC_AHB1Periph_GPIOB
#define OLED_WR_CLOCK     RCC_AHB1Periph_GPIOA
#define OLED_RD_CLOCK     RCC_AHB1Periph_GPIOD
#define OLED_CS 	PBout(7)				  //CS
#define OLED_RST  PBout(6)					  //RESET
#define OLED_RS 	PBout(3)				  //DC
#define OLED_WR 	PAout(4)		  		  
#define OLED_RD 	PDout(7)
#define OLED_CS_GPIO     GPIOB
#define OLED_CS_PIN      GPIO_Pin_7
#define OLED_RST_GPIO    GPIOB
#define OLED_RST_PIN     GPIO_Pin_6
#define OLED_RS_GPIO     GPIOB
#define OLED_RS_PIN      GPIO_Pin_3
#define OLED_WR_GPIO     GPIOA
#define OLED_WR_PIN      GPIO_Pin_4
#define OLED_RD_GPIO     GPIOD
#define OLED_RD_PIN      GPIO_Pin_7
/******************************SPI ģʽ**************************************/
#ifdef	SPI
	#define OLED_SCLK_CLOCK    RCC_AHB1Periph_GPIOB
	#define OLED_SDIN_CLOCK    RCC_AHB1Periph_GPIOB
	#define OLED_SCLK 	       PBout(8)			  //CLK.D0
	#define OLED_SDIN 	       PBout(9)			  //D1
	#define OLED_SCLK_GPIO     GPIOB
	#define OLED_SCLK_PIN      GPIO_Pin_8
	#define OLED_SDIN_GPIO     GPIOB
	#define OLED_SDIN_PIN      GPIO_Pin_9
#endif
/******************************I2C ģʽ**************************************/
#ifdef	I2C
	#define SDA_IN()  {GPIOC->MODER&=~(3<<(7*2));GPIOC->MODER|=0<<7*2;}	//PB9����ģʽ
	#define SDA_OUT() {GPIOC->MODER&=~(3<<(7*2));GPIOC->MODER|=1<<7*2;} //PB9���ģʽ
	#define OLED_SCL_CLOCK    RCC_AHB1Periph_GPIOC
	#define OLED_SDA_CLOCK    RCC_AHB1Periph_GPIOC
	#define OLED_SCL 	        PCout(6)
	#define OLED_SDA 	        PCout(7)
	#define READ_SDA   PCin(7)  //����SDA 
	#define OLED_SCL_GPIO     GPIOC
	#define OLED_SCL_PIN      GPIO_Pin_6
	#define OLED_SDA_GPIO     GPIOC
	#define OLED_SDA_PIN      GPIO_Pin_7

	#define OLED_CMD  	0		//д����
	#define OLED_DATA 	1		//д����
#endif

class OLED12864{

public:
	OLED12864();
	~OLED12864();
	//OLED�����ú���
	void OLED_WR_Byte(u8 dat,u8 cmd);	    
	void OLED_Display_On(void);
	void OLED_Display_Off(void);
	void OLED_Refresh_Gram(void);		   
											
	void OLED_Init(void);
	void OLED_Clear(void);
	void OLED_DrawPoint(u8 x,u8 y,u8 t);
	void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
	void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
	void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
	void OLED_ShowString(u8 x,u8 y,const char *p,u8 size);	 

private:
	//OLED��ʼ��Э��
	void IIC_Init(void);                					//��ʼ��IIC��IO��				 
	void IIC_Start(void);									//����IIC��ʼ�ź�
	void IIC_Stop(void);	  								//����IICֹͣ�ź�
	void IIC_Send_Byte(u8 txd);								//IIC����һ���ֽ�
	u8 	 IIC_Read_Byte(unsigned char ack);					//IIC��ȡһ���ֽ�
	u8   IIC_Wait_Ack(void); 								//IIC�ȴ�ACK�ź�
	void IIC_Ack(void);										//IIC����ACK�ź�
	void IIC_NAck(void);									//IIC������ACK�ź�
	void Write_IIC_Command(unsigned char IIC_Command);
	void Write_IIC_Data(unsigned char IIC_Data);

protected:
	void OLED_Data_Out(u8 data);
	u32  mypow(u8 m,u8 n);
};

#endif

