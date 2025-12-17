#include "stm32f10x.h"                  // Device header

void IC_Init(void)
{
	 // 1. 集中开启时钟（定时器+GPIO）
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	
	//选择内部时钟
	TIM_InternalClockConfig(TIM3);
	
    // 2. 先初始化GPIO（确保引脚准备就绪）
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;    // 复用推挽输出（定时器控制）
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;          // TIM3_CH1对应PA6
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  // 输出速度
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // 3. 配置定时器时基参数
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;  // 不分频
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数
    TIM_TimeBaseInitStruct.TIM_Period = 65535 - 1;  // ARR=65534（周期65534个计数单位）
    TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1;  // PSC=71（分频后1MHz）
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	//4.配置输入捕获通道
	TIM_ICInitTypeDef   TIM_ICInitStruct;
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter=0xF;
	TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICPrescaler=TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICSelection=TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3,&TIM_ICInitStruct);
	
	//5.配置触发源选择和从模式
	TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);
	TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_Reset);
	
	//6.开启定时器
	TIM_Cmd(TIM3,ENABLE);
}

uint16_t GetFreq(void)
{
	return 1000000/(TIM_GetCapture1(TIM3)+1);
}
