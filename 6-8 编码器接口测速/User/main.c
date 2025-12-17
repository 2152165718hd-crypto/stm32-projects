#include "stm32f10x.h"                  // Device header
#include "Encoder.h"
#include "Timer.h"
#include "OLED.h"
int16_t Num;
int16_t Speed;
int main(void)
{
	OLED_Init();
	Encoder_Init();
	Timer_Init();
	OLED_ShowString(1,1,"Speed:");
	OLED_ShowString(2,1,"Num:");
	while(1)
	{
		OLED_ShowSignedNum(1,7,Speed,3);
		OLED_ShowNum(2,5,Num,3);
	}
}


void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
	{
		Num++;
		Speed=Encoder_GetSpeed();
		TIM_ClearFlag(TIM2,TIM_IT_Update);
	}
}
