#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "OLED.h"

int main(void)
{
	OLED_Init();
	OLED_ShowString(1,1,"Clock:xxxxxxxxHz");
	while(1)
	{
		OLED_ShowNum(1,7,SystemCoreClock,8);
		OLED_ShowString(2,1,"Runing");
		Delay_ms(500);
		OLED_ShowString(2,1,"      ");
		Delay_ms(500);
	}
}

