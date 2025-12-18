#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "OLED.h"
#include "ADC.h"

int main(void)
{
	OLED_Init();
	AD_Init();
	OLED_ShowString(1,1,"ADvalue:");
	while(1)
	{
		OLED_ShowNum(1,9,AD_GetValue(),4);
		Delay_ms(100);   
	}
}

