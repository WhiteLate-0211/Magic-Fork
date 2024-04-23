/*
 **********************************************************************
 *Copyright(c)		Lance Zhang
 *Identification	CHIC
 *Describtion		Template
 *
 *Present Version	1.0
 *Writer			Lance Zhang
 *Date				8/16/2018
 **********************************************************************
 *W25Q16ϵ�й���32��block��ÿ��block 64KB��ÿ��block����16��sector��
 *ÿ��sector 4KB.ÿ��block����256��page��
 **********************************************************************
 *@Attention:
 *��ִ�в�����������ʱ����Ҫ��������ֽ�������4096
 *ԭ��
 *д�뺯����W25Q16_FLASH::W25QXX_Write()������ĵ�ַ�����ֽ�Ϊ��λ�ģ�
 *����4096(��4KB)�õ������ı�š�
 *��BLOCK 0Ϊ�������һ�������ı��Ϊ15��
 *����д��ʱ����ĵ�ַΪ��15*4096 = F000(h)������15����ʼ��ַ
 *����������W25QXX_Erase_Sector()��������������ı�ţ����*4096 = �ֽ���
 **********************************************************************
**/
#ifndef	_W25Q16_H
#define _W25Q16_H

extern "C"
{
	#include "stm32f4xx_gpio.h"
	#include "delay.h"
}
#include "SPI.h"

/*W25Xϵ��/Qϵ��оƬ�б�	   
W25Q80  ID  0XEF13
W25Q16  ID  0XEF14
W25Q32  ID  0XEF15
W25Q64  ID  0XEF16	
W25Q128 ID  0XEF17*/	
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
//����W25QXXоƬ�ͺ�
extern u16 W25QXX_TYPE;							   
//W25QXX��Ƭѡ�ź�
#define	W25QXX_CS 		PBout(0)  		

////////////////////////////////////////////////////////////////////////////////// 
//ָ�����ֲ��19ҳ
//Byte 0��
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

#define W25Q16_SIZE	2*1024*1024

class	W25Q16_FLASH : public SPI
{
public:
	W25Q16_FLASH();
	~W25Q16_FLASH();

private:
	u16 	W25QXX_TYPE;	//Ĭ����W25Q16
public:
	u32 	FLASH_SIZE;		//FLASH��С
	//sector_id * 4096 = FLASH_SIZE
	u32		SECTOR_ID(u8 sector_id);

public:
	void W25QXX_Init(void);
	u16  W25QXX_ReadID(void);  	    											//��ȡFLASH ID
	u8	 W25QXX_ReadSR(void);        											//��ȡ״̬�Ĵ��� 
	void W25QXX_Write_SR(u8 sr);  												//д״̬�Ĵ���
	void W25QXX_Write_Enable(void);  											//дʹ�� 
	void W25QXX_Write_Disable(void);											//д����
	void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
	void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   			//��ȡflash
	void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);			//д��flash
	void W25QXX_Erase_Chip(void);    	  										//��Ƭ����
	void W25QXX_Erase_Sector(u32 Dst_Addr);										//��������
	void W25QXX_Wait_Busy(void);           										//�ȴ�����
	void W25QXX_PowerDown(void);        										//�������ģʽ
	void W25QXX_WAKEUP(void);													//����
	void W25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
};

#endif
