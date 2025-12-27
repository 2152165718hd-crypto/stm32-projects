#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"

uint8_t Rxdata;
int main(void)
{
	OLED_Init();
	Serial_Init();

	OLED_ShowString(1, 1, "Data:");

	while (1)
	{
		if (Serial_GetFlag() == 1)
		{
			Rxdata = Serial_GetData();
			Serial_SendByte(Rxdata);
			OLED_ShowHexNum(1, 6, Rxdata, 2);
		}
		OLED_ShowString(2,1,"Runing");
		Delay_ms(200);
		OLED_ShowString(2,1,"      ");
		Delay_ms(200);
		__WFI();
	}
}
