#include "MAX262.h"

MAX262::MAX262()
{
	IO_Init();
	//���������ݶ�д����ʱ���������ȥ��
	delay_init(168);
	//��ʼ��������ֻ�ǳ�ʼ��������һ��Ϊֹû����������
	HeartBeat_Init();
}

MAX262::~MAX262()
{
	
}

void MAX262::HeartBeat_Init()
{
	//�Զ���װ��ֵ��84
	arr = 84 - 1;
	//ʱ��Ԥ��Ƶϵ����0������Ƶ��
	psc = 1 - 1;
	//������PWMƵ�ʣ�����ʱ����Ƶ/Ԥ��Ƶϵ����/�Զ���װ��ֵ
	//84/1 = 84MHz�� 84MHz/84 = 1MHz��
	TIMx			= 	TIM14;
	TIM_CLKBUS 		= 	RCC_APB1Periph_TIM14;
	
	GPIO_CLKBUS 	= 	RCC_AHB1Periph_GPIOA;
	GPIOx 			= 	GPIOA;
	GPIO_PinSource 	= 	GPIO_PinSource7;
	GPIO_AF_TIMx 	= 	GPIO_AF_TIM14;
	GPIO_Pin_x 		= 	GPIO_Pin_7;
/************************************************************************/
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	//TIM14ʱ��ʹ��    
	RCC_APB1PeriphClockCmd(TIM_CLKBUS,ENABLE);  		
	//ʹ��PORTFʱ��		
	RCC_AHB1PeriphClockCmd(GPIO_CLKBUS, ENABLE); 			
	//GPIOF9����Ϊ��ʱ��14
	GPIO_PinAFConfig(GPIOx,GPIO_PinSource,GPIO_AF_TIMx); 			
	//GPIO A7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
	//���ù���	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        	
	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		
	//���츴�����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    
	//����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	//��ʼ��PF9
	GPIO_Init(GPIOx,&GPIO_InitStructure);              				
	//��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_Prescaler = psc;  		
	//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	
	//�Զ���װ��ֵ	
	TIM_TimeBaseStructure.TIM_Period = arr;   						
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	//��ʼ����ʱ��14
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);					
	//��ʼ��TIM14 Channel1 PWMģʽ	
	//�Ƚ����ʹ��	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//�������:TIM����Ƚϼ��Ե�
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	//����Tָ���Ĳ�����ʼ������TIM1 4OC1	
	TIM_OC1Init(TIMx, &TIM_OCInitStructure); 
	//ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���	
	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
	//ARPEʹ�� 
	TIM_ARRPreloadConfig(TIMx,ENABLE);								
	//ʹ��TIM14
	TIM_Cmd(TIMx, ENABLE);  	
}

void MAX262::IO_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin = EN_Pin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void MAX262::Data_Out(u8 data)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin = ADDR_Pin_0 | ADDR_Pin_1 | ADDR_Pin_2 | 
									ADDR_Pin_3 | DATA_Pin_0 | DATA_Pin_1 | EN_Pin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIOE->ODR &= ~(0x3f<<8);
	GPIOE->ODR |= data << 8;		
} 

u8 MAX262::Data_In(void)
{ 
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin = ADDR_Pin_0 | ADDR_Pin_1 | ADDR_Pin_2 | 
									ADDR_Pin_3 | DATA_Pin_0 | DATA_Pin_1 | EN_Pin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	u8 dat;
	dat=(GPIOE->IDR) >> 8;
	return dat;
}

void MAX262::write_data(u8 addr, u8 data)
{	
	//data&0x03 �� 1111 -> 0011,��Ӱ��PE14��PE15
	Data_Out(addr|((data&0x03)<<4)); 
	EN = 0;
	delay_us(100);
	EN = 1;
	delay_us(50);
}

void MAX262::Set_Af(u8 data_f)
{
	write_data(1,data_f);  
	data_f = data_f>>2;         
	write_data(2,data_f);
	data_f = data_f>>2;         
	write_data(3,data_f);
}

//����Aͨ��Qֵ
//7λƷ������Q����λQ0-Q6 ��Ӧʮ����0-127
void MAX262::Set_Aq(u8 data_q)    
{    
	write_data(4,data_q);         
	data_q = data_q>>2;         
	write_data(5,data_q);         
	data_q = data_q>>2;         
	write_data(6,data_q);    
	//�Ե�ַ7ֻȡһλ 
	data_q = (data_q>>2)&1;       
	write_data(7,data_q);    
} 

//����Bͨ��Fֵ     
void MAX262::Set_Bf(u8 data_f)    
{    
    write_data(9,data_f);        
	data_f = data_f>>2;       
	write_data(10,data_f);      
	data_f = data_f>>2;       
	write_data(11,data_f);    
} 
    
//����Bͨ��Qֵ     
void MAX262::Set_Bq(u8 data_q)    
{    
    write_data(12,data_q);         
	data_q = data_q>>2;        
	write_data(13,data_q);         
	data_q = data_q>>2;         
	write_data(14,data_q);    
    data_q = (data_q>>2)&1;
	write_data(15,data_q);    
}

//����f0��Q��workMode��ͨ��channel����MAX262���ݲ��������� 
//����ʱ��Ƶ��fclk 
float MAX262::bs_WorkFclk(float f0,float Q,u8 workMode,u8 channel) 
{ 
	u8 Qn; 
    if (workMode==modeTwo)              
	{
		//ǿ�Ƶ����ȼ��㣬���Ч��
		Qn = (u8)(128-90.51f/Q);          
	}
	else
	{		
    Qn = (u8)(128-64/Q); 
	}		
	if(channel==channelA)		
	{ 
        write_data(0,workMode);       
		Set_Af(63);
		//Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.     		
		Set_Aq(Qn);       
	}     
	else      
	{ 
		write_data(8,workMode);         
		Set_Bf(63);     
		//Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.         
		Set_Bq(Qn);       
	}   
	if(workMode==modeTwo)        
	{
		return  31.46625*PI*f0;     
	}
	else
	{		
       return  44.5*PI*f0; 
	}
}
//��������
void MAX262::CLK_set(float freq)
{
	u32 arr_t;
	
	if(freq<100000)
	{
		TIM14->PSC = 840-1;
		arr_t = 100000/freq;
		arr_t--;
		TIM14->ARR = arr_t;
		TIM14->CCR1 = arr_t/2;
	}
	else
	{
		TIM14->PSC = 0;
		arr_t = 84000000/freq;
		arr_t--;
		TIM14->ARR = arr_t;
		TIM14->CCR1 = arr_t/2;
	}
}

void MAX262::Filter_On(float f0,float Q,u8 workMode,u8 channel)
{
	float heartbeat;
	heartbeat = bs_WorkFclk(f0,Q,workMode,channel);
	CLK_set(heartbeat);
}
