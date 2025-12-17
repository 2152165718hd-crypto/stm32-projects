#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "PWM.h"
#include "OLED.h"
int16_t i;
int main(void)
{
	OLED_Init();
	PWM_Init();
	OLED_ShowString(1,3,"HelloWorld");
	
	while(1)
	{
		for(i=0;i<100;i++)
		{
			Set_Compare(i);
			Delay_ms(10);
		}
		for(i=100;i>0;i--)
		{
			Set_Compare(i);
			Delay_ms(10);
		}
	}
}

