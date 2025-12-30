#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Key.h"
#include "OLED.h"

int main(void)
{
	OLED_Init();
	Key_Init();
	
	OLED_ShowString(1,1,"WWDG TEST");
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_WWDG,ENABLE);
	if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST)==SET)
	{
		OLED_ShowString(2,1,"WWDGRST");
		Delay_ms(500);
		OLED_ShowString(2,1,"       ");
		Delay_ms(100);
		RCC_ClearFlag();
	}
	else
	{
		OLED_ShowString(3,1,"RST");
		Delay_ms(500);
		OLED_ShowString(3,1,"   ");
		Delay_ms(100);
		RCC_ClearFlag();
	}
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	WWDG_SetWindowValue(0x40|21);
	WWDG_Enable(0x40|54);
	while(1)
	{
		Get_KeyNum();
		OLED_ShowString(4,1,"FEED");
		Delay_ms(20);
		OLED_ShowString(4,1,"    ");
		Delay_ms(20);
		WWDG_SetCounter(0x40|54);
	}
}

