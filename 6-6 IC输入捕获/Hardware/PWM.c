#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
    // 1. 集中开启时钟（定时器+GPIO）
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//选择内部时钟
	TIM_InternalClockConfig(TIM2);
	
    // 2. 先初始化GPIO（确保引脚准备就绪）
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;    // 复用推挽输出（定时器控制）
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;          // TIM2_CH1对应PA0
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  // 输出速度
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // 3. 配置定时器时基参数
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;  // 不分频
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数
    TIM_TimeBaseInitStruct.TIM_Period = 100 - 1;  // ARR=99（周期100个计数单位）
    TIM_TimeBaseInitStruct.TIM_Prescaler = 720 - 1;  // PSC=719（分频后100kHz）
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
    
    // 4. 配置PWM输出参数（显式使能输出）
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCStructInit(&TIM_OCInitStruct);  // 初始化默认值
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;  // PWM模式1
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;  // 高电平有效
    TIM_OCInitStruct.TIM_Pulse = 50;  // CCR=50（占空比50%）
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;  // 使能通道输出
    TIM_OC1Init(TIM2, &TIM_OCInitStruct);
    
    // 5. 最后启动定时器（确保所有配置完成）
    TIM_Cmd(TIM2, ENABLE);
}
void Set_Compare(uint16_t Compare)
{
	TIM_SetCompare1(TIM2,Compare);
}

void SetFreq(uint16_t PSC)
{
	TIM_PrescalerConfig(TIM2,PSC,TIM_PSCReloadMode_Immediate);
}


