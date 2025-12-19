#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "OLED.h"
#include "Serial.h"
uint8_t Data;
uint8_t Dat[4]={0x41,0x42,0x43,0x44};
int main(void)
{
	OLED_Init();
	Serial_Init();
	OLED_ShowString(1,1,"Data:");
	while(1)
	{
		OLED_ShowHexNum(1,6,Serial_GetData(),2);
	}
}

