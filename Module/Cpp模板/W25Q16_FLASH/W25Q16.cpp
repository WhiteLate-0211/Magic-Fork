#include "W25Q16.h"

W25Q16_FLASH::W25Q16_FLASH()
{
	//��ʼ��FLASH��С
	FLASH_SIZE = W25Q16_SIZE;
	//��ʼ��оƬ����
	W25QXX_TYPE = W25Q16;
	W25QXX_Init();
}

W25Q16_FLASH::~W25Q16_FLASH()
{
	
}
/*FLASH��ʼ��*/
void W25Q16_FLASH::W25QXX_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	//ʹ��GPIOBʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	  //GPIOB pin0 Ƭѡ��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		
	//�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	//����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
	
	//SPI FLASH��ѡ��
	W25QXX_CS=1;	
	//��ʼ��SPI,���ڸ�����SPI������Ҫ�ٴγ�ʼ��
	//����Ϊ42Mʱ��,����ģʽ 
	SPI1_SetSpeed(SPI_BaudRatePrescaler_2);		
	//��ȡFLASH ID.
	W25QXX_TYPE=W25QXX_ReadID();				
}
/*��ȡ״̬�Ĵ���*/
u8 W25Q16_FLASH::W25QXX_ReadSR(void)   
{  
	u8 byte=0;   
	//ʹ������ 
	W25QXX_CS=0;                              
	//д��BYTE1ָ��
	SPI1_ReadWriteByte(W25X_ReadStatusReg);    
	//��ʾ���У���ȡһ���ֽ�
	byte=SPI1_ReadWriteByte(0Xff);             
	//ȡ��Ƭѡ   
	W25QXX_CS=1;                              
	return byte;   
}
/*д��״̬�Ĵ�����S0 - S7��*/
void W25Q16_FLASH::W25QXX_Write_SR(u8 sr)   
{   
	W25QXX_CS=0;                         
	//����дȡ״̬�Ĵ�������  
	SPI1_ReadWriteByte(W25X_WriteStatusReg);     
	//д��һ���ֽ�  
	SPI1_ReadWriteByte(sr);               
	W25QXX_CS=1;                               	      
}
/****
 *��ȡ״̬�Ĵ���
 *W25Q16дʹ��	
 *��WEL��λ
***/
void W25Q16_FLASH::W25QXX_Write_Enable(void)   
{
	W25QXX_CS=0;                            
	//����дʹ��  
    SPI1_ReadWriteByte(W25X_WriteEnable);      
	W25QXX_CS=1;                                 	      
}  
/****
 *W25QXXд��ֹ
 *��WEL����	
***/
void W25Q16_FLASH::W25QXX_Write_Disable(void)   
{  
	W25QXX_CS=0;                        
	//����д��ָֹ��        
    SPI1_ReadWriteByte(W25X_WriteDisable);    
	W25QXX_CS=1;                             	      
} 		
/****
 *��ȡоƬID
 *����ֵ����:				   
 *0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
 *0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
 *0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
 *0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
 *0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	  
***/
u16 W25Q16_FLASH::W25QXX_ReadID(void)
{
	u16 Temp = 0;	  
	W25QXX_CS=0;			
	//���Ͷ�ȡID����	
	SPI1_ReadWriteByte(0x90);
	//24λ��ַΪ00h
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	 	
	//16λ�����ݣ��ȴ�MSB��ʼ�������ư�λ������LSB��
	//����0xff��ʾ���У��ɽ�������FLASH������
	Temp|=SPI1_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI1_ReadWriteByte(0xFF);	 
	W25QXX_CS=1;				    
	return Temp;
} 
/****
 *��ȡSPI FLASH  
 *��ָ����ַ��ʼ��ȡָ�����ȵ�����
 *pBuffer:���ݴ洢��
 *ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
 *NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
***/
void W25Q16_FLASH::W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;   										   
	//ʹ������  	
	W25QXX_CS=0;                     
	//���Ͷ�ȡ����   
    SPI1_ReadWriteByte(W25X_ReadData);
	//����24bit��ַ����MSB��LSB����    	
    SPI1_ReadWriteByte((u8)((ReadAddr)>>16));  
    SPI1_ReadWriteByte((u8)((ReadAddr)>>8));   
    SPI1_ReadWriteByte((u8)ReadAddr);
	//����Ϊֹ
    for(i=0;i<NumByteToRead;i++)
	{ 
		//ѭ������
        pBuffer[i]=SPI1_ReadWriteByte(0XFF);   
    }
	W25QXX_CS=1;  				    	      
}  
/****
 *SPI��һҳ(0~65535)��д������256���ֽڵ�����
 *��ָ����ַ��ʼд�����256�ֽڵ�����
 *pBuffer:���ݴ洢��
 *WriteAddr:��ʼд��ĵ�ַ(24bit)
 *NumByteToWrite:Ҫд����ֽ���(���256)
 *������Ӧ�ó�����ҳ��ʣ���ֽ���
 *ÿһ���������ֽ�����00h -> ffh
***/
void W25Q16_FLASH::W25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
	//SET WEL
    W25QXX_Write_Enable();                  
	W25QXX_CS=0;       
	//����дҳ����   
    SPI1_ReadWriteByte(W25X_PageProgram);
	//����24bit��ַ   
    SPI1_ReadWriteByte((u8)((WriteAddr)>>16));  
    SPI1_ReadWriteByte((u8)((WriteAddr)>>8));   
    SPI1_ReadWriteByte((u8)WriteAddr);   
	//ѭ��д��  
    for(i=0;i<NumByteToWrite;i++)
		SPI1_ReadWriteByte(pBuffer[i]);
	W25QXX_CS=1;
	//�ȴ�д�����
	W25QXX_Wait_Busy();					   
} 
/****
 *�޼���дSPI FLASH 
 *����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
 *�����Զ���ҳ���� 
 *��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
 *pBuffer:���ݴ洢��
 *WriteAddr:��ʼд��ĵ�ַ(24bit)
 *NumByteToWrite:Ҫд����ֽ���(���65535)
 *CHECK OK
***/
void W25Q16_FLASH::W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	//��ҳʣ����ֽ���
	pageremain=256-WriteAddr%256; 		 	    
	if(NumByteToWrite<=pageremain)
		//������256���ֽ�
		pageremain=NumByteToWrite;
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		//д�����
		if(NumByteToWrite==pageremain)
			break;
		//NumByteToWrite>pageremain
	 	else 
		{
			pBuffer += pageremain;
			WriteAddr += pageremain;	

			//��ȥ�Ѿ�д���˵��ֽ���
			NumByteToWrite -= pageremain;			  
			//һ�ο���д��256���ֽ�
			if(NumByteToWrite>256)
				pageremain = 256; 
			//����256���ֽ���
			else 
				pageremain = NumByteToWrite; 	  
		}
	};	    
} 
/****
 *дSPI FLASH  
 *��ָ����ַ��ʼд��ָ�����ȵ�����
 *�ú�������������!
 *pBuffer:���ݴ洢��
 *WriteAddr:��ʼд��ĵ�ַ(24bit)						
 *NumByteToWrite:Ҫд����ֽ���(���65535) 
***/
u8 		W25QXX_BUFFER[4096];		 
void W25Q16_FLASH::W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * W25QXX_BUF;
	
   	W25QXX_BUF 	= 	W25QXX_BUFFER;	     
	//��ȡ������ַ  
 	secpos		=	WriteAddr/4096;
	//�������ڵ�ƫ��
	secoff		=	WriteAddr%4096;
	//����ʣ��ռ��С
	secremain	=	4096 - secoff;
	//������4096���ֽ�
 	if(NumByteToWrite<=secremain)
		secremain=NumByteToWrite;
	while(1) 
	{	
		//������������������
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);
		for(i=0;i<secremain;i++)//У������
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)
				break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			W25QXX_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//д����������  

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	};	 
}
/****
 *��������оƬ		  
 *�ȴ�ʱ�䳬��...
***/
void W25Q16_FLASH::W25QXX_Erase_Chip(void)   
{                                   
    W25QXX_Write_Enable();                  //SET WEL 
    W25QXX_Wait_Busy();   
  	W25QXX_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(W25X_ChipErase);        //����Ƭ��������  
	W25QXX_CS=1;                            //ȡ��Ƭѡ     	      
	W25QXX_Wait_Busy();   				   //�ȴ�оƬ��������
}   
/****
 *����һ������
 *Dst_Addr:������ַ ����ʵ����������
 *����һ������������ʱ��:150ms
***/
void W25Q16_FLASH::W25QXX_Erase_Sector(u32 Dst_Addr)   
{  
	//����falsh�������,������   
 	//printf("fe:%x\r\n",Dst_Addr);	  
 	Dst_Addr *= 4096;
    W25QXX_Write_Enable();                  //SET WEL 	 
    W25QXX_Wait_Busy();   
  	W25QXX_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(W25X_SectorErase);      //������������ָ�� 
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>16));  //����24bit��ַ    
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>8));   
    SPI1_ReadWriteByte((u8)Dst_Addr);  
	W25QXX_CS=1;                            //ȡ��Ƭѡ     	      
    W25QXX_Wait_Busy();   				   //�ȴ��������
}  
/*�ȴ�����*/
void W25Q16_FLASH::W25QXX_Wait_Busy(void)   
{   // �ȴ�BUSYλ(S0)����
	while((W25QXX_ReadSR()&0x01)==0x01);   
}  
//�������ģʽ
void W25Q16_FLASH::W25QXX_PowerDown(void)   
{ 
  	W25QXX_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(W25X_PowerDown);        //���͵�������  
	W25QXX_CS=1;                            //ȡ��Ƭѡ     	      
    delay_us(3);                               //�ȴ�TPD  
}   
/*����*/
void W25Q16_FLASH::W25QXX_WAKEUP(void)   
{  
  	W25QXX_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	W25QXX_CS=1;                            //ȡ��Ƭѡ     	      
    delay_us(3);                               //�ȴ�TRES1
}   
/*ͨ���������������д����*/
u32 W25Q16_FLASH::SECTOR_ID(u8 sector_id)
{
	sector_id *= 4096;
	return sector_id;
}
