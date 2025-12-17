#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Timer.h"
#include "OLED.h"
uint16_t Num;

int main(void)
{
	OLED_Init();
	Timer_Init();
	OLED_ShowString(1,1,"Num");
	while(1)
	{
		OLED_ShowNum(2,1,Num,5);
	}
}
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
	{
		Num++;
		TIM_ClearFlag(TIM2,TIM_IT_Update);
	}
}
