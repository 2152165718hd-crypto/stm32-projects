#include "stm32f10x.h"                  // Device header


void Encoder_Init(void)
{
	//开启外设时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//初始化GIPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//初始化定时器时基单元
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=65535-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=0;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	TIM_ClearFlag(TIM3,TIM_IT_Update);
	
	//初始化输入捕获通道
	TIM_ICInitTypeDef  TIM_ICInitStruct;
	//初始化通道1
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter=0xF;
	TIM_ICInit(TIM3,&TIM_ICInitStruct);
	//初始化通道2
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_2;
	TIM_ICInitStruct.TIM_ICFilter=0xF;
	TIM_ICInit(TIM3,&TIM_ICInitStruct);
	
	//初始化编码器接口
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	
	//开启定时器
	TIM_Cmd(TIM3,ENABLE);
}

int16_t Encoder_GetSpeed(void)
{
	uint16_t Temp;
	Temp=TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3,0);
	return Temp;
}
