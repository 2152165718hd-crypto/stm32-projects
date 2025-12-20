#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "OLED.h"
#include "Serial.h"
uint16_t Rxdata;
int main(void)
{
	OLED_Init();
	Serial_Init();
	OLED_ShowString(1,1,"Data:");
	while(1)
	{
		if(Serial_GetFlag()==1)
		{
			Rxdata=Serial_GetData();
			Serial_SendByte(Rxdata);
			OLED_ShowHexNum(1,6,Serial_GetData(),2);
		}
	}
}

