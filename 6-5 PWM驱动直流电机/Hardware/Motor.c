#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	PWM_Init();
}

void Motor_SetSpeed(int16_t Speed)
{
    if(Speed>0)
    {
        GPIO_SetBits(GPIOA,GPIO_Pin_4);
        GPIO_ResetBits(GPIOA,GPIO_Pin_5);
        Set_Compare2(Speed);
    }
    else if(Speed<0)
    {
        GPIO_SetBits(GPIOA,GPIO_Pin_5);
        GPIO_ResetBits(GPIOA,GPIO_Pin_4);
        Set_Compare2(-Speed);
    }
    else  // 处理Speed=0的情况
    {
        GPIO_ResetBits(GPIOA,GPIO_Pin_4 | GPIO_Pin_5);
        Set_Compare2(0);
    }
}
