#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Key.h"
#include "OLED.h"

int main(void)
{
	uint8_t Keynum;
	
	OLED_Init();
	Key_Init();
	
	OLED_ShowString(1,1,"IWDG TEST");
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST)==SET)
	{
		OLED_ShowString(2,1,"IWDGRST");
		Delay_ms(200);
		OLED_ShowString(2,1,"       ");
		RCC_ClearFlag();
	}
	else
	{
		OLED_ShowString(3,1,"RST");
		Delay_ms(200);
		OLED_ShowString(3,1,"   ");
		RCC_ClearFlag();
	}
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_8);
	IWDG_SetReload(2499);
	IWDG_Enable();
	IWDG_ReloadCounter();
	
	
	while(1)
	{
		IWDG_ReloadCounter();
		Keynum=Get_KeyNum();
		Delay_ms(200);
		OLED_ShowString(4,1,"FEED");
		Delay_ms(200);
		OLED_ShowString(4,1,"    ");
	}
}

