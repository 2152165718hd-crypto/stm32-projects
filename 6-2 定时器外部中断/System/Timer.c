#include "stm32f10x.h"                  // Device header

void Timer_Init(void)
{
	//开启定时器时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//初始化GPIO
	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//选择内部时钟源
	TIM_ETRClockMode2Config(TIM2,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,0x00);
	
	//初始化定时器时基单元
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=10-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=1-1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=1;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	TIM_ClearFlag(TIM2,TIM_IT_Update);
	
	//使能定时器中断
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	//使能定时器
	TIM_Cmd(TIM2,ENABLE);
	
	//配置中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef  NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_InitStruct);
}

uint16_t Get_Count(void)
{
	return TIM_GetCounter(TIM2);
}
//中断函数模板
//void TIM2_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
//	{
//		Num++;
//		TIM_ClearFlag(TIM2,TIM_IT_Update);
//	}
//}
