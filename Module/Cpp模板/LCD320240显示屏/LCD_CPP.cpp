#include "LCD_CPP.h"

//Ĭ�ϵ�����ģʽ
LCD_Config::LCD_Config()
{
	font_color = BLACK;
	background_color = WHITE;
	screen = HOR;
	LCD_Initlize(font_color,background_color,screen);
	LCD_ShowString(20,70,210,24,16,"Default mode selected.");
	LCD_Clear(background_color);
	status_bar(GRAY,screen);
}

LCD_Config::LCD_Config(int font_color,int background_color,char screen)
{
	LCD_Initlize(font_color,background_color,screen);
	LCD_Clear(background_color);
	status_bar(GRAY,screen);
}

LCD_Config::~LCD_Config()
{
	
}

void LCD_Config::LCD_Initlize(int font_color,int background_color,char screen)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);      		//��ʼ����ʱ����
	uart_init(115200);			//��ʼ�����ڲ�����Ϊ115200
	LCD_Init(screen);           		//��ʼ��LCD FSMC�ӿ�
	POINT_COLOR=font_color;
	LCD_Clear(background_color);	
	LCD_ShowString(20,10,210,24,16,"Initlizing");
	delay_ms(100);
	LCD_ShowString(20,10,210,24,16,"Initlizing.");
	delay_ms(100);
	LCD_ShowString(20,10,210,24,16,"Initlizing..");
	delay_ms(100);
	LCD_ShowString(20,10,210,24,16,"Initlizing...");
	delay_ms(100);
	LCD_ShowString(20,40,210,24,16,"Initlize complete!");
}

void LCD_Config::status_bar(int color,char screen)
{
	if(!screen)
	{
		LCD_Fill(0,300,240,320,color);
	}
	else
	{
		LCD_Fill(0,220,480,240,color);
	}
}
