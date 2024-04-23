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
 *@Attention:
 **********************************************************************
 *��Ҫ���õĿ⣺stm32f4xx_spi.h
 **********************************************************************
**/
#ifndef	_SPI_H
#define _SPI_H

extern "C"
{
	#include "stm32f4xx_gpio.h"
	#include "stm32f4xx_spi.h"
}

namespace spi{
	class SPI;
}

//ֻ��public����������ҪSPI������İְ�
class SPI
{
public:
	SPI();
	~SPI();
	void SPI1_Init();
	void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
	u8 SPI1_ReadWriteByte(u8 TxData);
};

#endif
