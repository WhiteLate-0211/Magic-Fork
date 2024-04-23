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
extern "C" {
	#include "stdlib.h"
	#include "oledfont.h"  	 
	#include "delay.h"
}
#include "oled.h"

//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
u8 OLED_GRAM[128][8];	 

OLED12864::OLED12864()
{
	OLED_Init();
	OLED_Clear();
	OLED_ShowString(1,20,"12354",12);
}
//�����Դ浽LCD
void OLED12864::OLED_Refresh_Gram(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
	}
}
#if OLED_MODE==1	//8080����
//ͨ��ƴ�յķ�����OLED���һ��8λ����
//data:Ҫ���������
void OLED12864::OLED_Data_Out(u8 data)
{
	u16 dat=data&0X0F;
	GPIOC->ODR&=~(0XF<<6);		//���6~9
	GPIOC->ODR|=dat<<6;			//D[3:0]-->PC[9:6]
	GPIO_Write(GPIOC,dat<<6);
	PCout(11)=(data>>4)&0X01;	//D4
	PBout(6)=(data>>5)&0X01;	//D5
	PEout(5)=(data>>6)&0X01;	//D6
	PEout(6)=(data>>7)&0X01;	//D7 
} 
//��SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED12864::OLED_WR_Byte(u8 dat,u8 cmd)
{
	OLED_Data_Out(dat);	    
 	OLED_RS=cmd;
	OLED_CS=0;	   
	OLED_WR=0;	 
	OLED_WR=1;
	OLED_CS=1;	  
	OLED_RS=1;	 
}
#elif  OLED_MODE==2

void OLED12864::Write_IIC_Command(unsigned char IIC_Command)
{
IIC_Start();
IIC_Send_Byte(0x78);            //Slave address,SA0=0
IIC_Wait_Ack();	
IIC_Send_Byte(0x00);			//write command
IIC_Wait_Ack();	
IIC_Send_Byte(IIC_Command);
IIC_Wait_Ack();	
IIC_Stop();
}
// IIC Write Data
void OLED12864::Write_IIC_Data(unsigned char IIC_Data)
{
	IIC_Start();
	IIC_Send_Byte(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
	IIC_Send_Byte(0x40);			//write data
	IIC_Wait_Ack();	
	IIC_Send_Byte(IIC_Data);
	IIC_Wait_Ack();	
	IIC_Stop();
}

void OLED12864::OLED_WR_Byte(u8 dat,u8 cmd)
{
 if(cmd)
 {
   Write_IIC_Data(dat);
 }
  else 
{
  Write_IIC_Command(dat);		
 }
}


#else
//��SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED12864::OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	OLED_RS=cmd; //д���� 
	OLED_CS=0;		  
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK=0;
		if(dat&0x80)OLED_SDIN=1;
		else OLED_SDIN=0;
		OLED_SCLK=1;
		dat<<=1;   
	}				 
	OLED_CS=1;		  
	OLED_RS=1;   	  
} 
#endif
	  	  
//����OLED��ʾ    
void OLED12864::OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED12864::OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED12864::OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//������ʾ
}
//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���				   
void OLED12864::OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}
//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,���;1,���	  
void OLED12864::OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//������ʾ
}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 12/16/24
void OLED12864::OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[chr][t];	//����1608����
		else if(size==24)temp=asc2_2412[chr][t];	//����2412����
		else return;								//û�е��ֿ�
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}
    }
	OLED_Refresh_Gram();
}
//m^n����
u32 OLED12864::mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED12864::OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
	OLED_Refresh_Gram();
} 
//��ʾ�ַ���
//x,y:�������  
//size:�����С 
//*p:�ַ�����ʼ��ַ 
void OLED12864::OLED_ShowString(u8 x,u8 y,const char *p,u8 size)
{	
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }
	OLED_Refresh_Gram();
	
}	
//��ʼ��SSD1306					    
void OLED12864::OLED_Init(void)
{ 	 		 
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOG, ENABLE);
	 //ʹ��PORTA~E,PORTGʱ��,�����ĸ��Ų�һ����ͷ�ļ�
  
#if OLED_MODE==1		//ʹ��8080����ģʽ		
	
	//GPIO��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7 ;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_11;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��	
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_5;	
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��	
 
	
 	OLED_WR=1;
	OLED_RD=1; 
#elif  OLED_MODE==0					//ʹ��4��SPI ����ģʽ

  GPIO_InitStructure.GPIO_Pin = OLED_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(OLED_CS_GPIO, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = OLED_SCLK_PIN|OLED_SDIN_PIN;	
	GPIO_Init(OLED_SCLK_GPIO, &GPIO_InitStructure);//��ʼ��		

	GPIO_InitStructure.GPIO_Pin = OLED_RS_PIN;	
	GPIO_Init(OLED_RS_GPIO, &GPIO_InitStructure);//��ʼ��		
	
	GPIO_InitStructure.GPIO_Pin = OLED_RST_PIN;	
	GPIO_Init(OLED_RST_GPIO, &GPIO_InitStructure);//��ʼ��
	
	OLED_SDIN=1;
	OLED_SCLK=1;
#else   //iic
  GPIO_InitStructure.GPIO_Pin=OLED_SCL_PIN|OLED_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(OLED_SCL_GPIO, &GPIO_InitStructure);//��ʼ��
		OLED_SCL=1;
    OLED_SDA=1;

#endif
	OLED_CS=1;
	OLED_RS=1;	 
	
	OLED_RST=0;
	delay_ms(100);
	OLED_RST=1; 
					  
	OLED_WR_Byte(0xAE,OLED_CMD); //�ر���ʾ
	OLED_WR_Byte(0xD5,OLED_CMD); //����ʱ�ӷ�Ƶ����,��Ƶ��
	OLED_WR_Byte(80,OLED_CMD);   //[3:0],��Ƶ����;[7:4],��Ƶ��
	OLED_WR_Byte(0xA8,OLED_CMD); //��������·��
	OLED_WR_Byte(0X3F,OLED_CMD); //Ĭ��0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //������ʾƫ��
	OLED_WR_Byte(0X00,OLED_CMD); //Ĭ��Ϊ0

	OLED_WR_Byte(0x40,OLED_CMD); //������ʾ��ʼ�� [5:0],����.
													    
	OLED_WR_Byte(0x8D,OLED_CMD); //��ɱ�����
	OLED_WR_Byte(0x14,OLED_CMD); //bit2������/�ر�
	OLED_WR_Byte(0x20,OLED_CMD); //�����ڴ��ַģʽ
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
	OLED_WR_Byte(0xA1,OLED_CMD); //���ض�������,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0,OLED_CMD); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
	OLED_WR_Byte(0xDA,OLED_CMD); //����COMӲ����������
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]����
		 
	OLED_WR_Byte(0x81,OLED_CMD); //�Աȶ�����
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	OLED_WR_Byte(0xD9,OLED_CMD); //����Ԥ�������
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //����VCOMH ��ѹ����
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	OLED_WR_Byte(0xA6,OLED_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ	    						   
	OLED_WR_Byte(0xAF,OLED_CMD); //������ʾ	 
	OLED_Clear();
}

////////iic������
void OLED12864::IIC_Start(void)
{
	SDA_OUT();     //sda�����
	OLED_SDA=1;	  	  
	OLED_SCL=1;
	delay_us(5);
 	OLED_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(5);
	OLED_SCL=0;//ǯסI2C���ߣ�׼�����ͻ����������Ϊֻ��ʱ�������͵�ʱ����������ݱ仯 .
}	  
//����IICֹͣ�ź�
//��SCLΪ�ߵ�ƽ��ʱ��SDA�����ɵ͵��ߵ����䱻����Ϊֹͣ����
void OLED12864::IIC_Stop(void)
{
	SDA_OUT();//sda�����
	OLED_SCL=0;
	OLED_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	OLED_SCL=1; 
	OLED_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 OLED12864::IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	OLED_SDA=1;delay_us(1);	   
	OLED_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	OLED_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
//IIC_SDA���һ�����ź�
void OLED12864::IIC_Ack(void)
{
	OLED_SCL=0;
	SDA_OUT();
	OLED_SDA=0;
	delay_us(2);
	OLED_SCL=1;
	delay_us(2);
	OLED_SCL=0;
}
//������ACKӦ��		    
void OLED12864::IIC_NAck(void)
{
	OLED_SCL=0;
	SDA_OUT();
	OLED_SDA=1;
	delay_us(2);
	OLED_SCL=1;
	delay_us(2);
	OLED_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void OLED12864::IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    OLED_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        OLED_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		OLED_SCL=1;
		delay_us(2); 
		OLED_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 OLED12864::IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        OLED_SCL=0; 
        delay_us(2);
		OLED_SCL=1;
        receive<<=1;//����һλ
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}
