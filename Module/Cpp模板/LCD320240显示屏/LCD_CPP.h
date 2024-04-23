#ifndef	__LCD_CPP
#define __LCD_CPP

extern "C" { 
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "lcd.h"
#include "stm32f4xx_fsmc.h"
}
//��������LCD�ĺ���������Ĭ��Ϊ1������ģʽ
#define HOR 1
#define VIR 0

class LCD_Config
{
	
private:
	int font_color;			//������ɫ
	int background_color;	//����ɫ
	char screen;			//��Ļ��������������

public:
	//Ĭ�ϳ�ʼ���ӿ�
	LCD_Config();
	//�Զ����ʼ���ӿ�
	LCD_Config(int font_color,int background_color,char screen);
	~LCD_Config();

	//״̬��
	void status_bar(int color,char screen = 0);

private:
	//LCD��ʼ������
	void LCD_Initlize(int font_color,int background_color,char screen);
};

#endif /*__LCD_CPP*/
