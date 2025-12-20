#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "Serial.h"
#include <string.h>
uint8_t Keynum;

int main(void)
{
	OLED_Init();
	Serial_Init();
	Key_Init();
	LED_Init();
	OLED_ShowString(1,1,"TXPacket:");
	OLED_ShowString(3,1,"RXPacket:");
	
	while(1)
	{
		if(Serial_Flag==1)
		{
			OLED_ShowString(4,1,Serial_RXdataPackt);
			if(strcmp(Serial_RXdataPackt,"LED_ON")==0)
			{
				LED1_TurnOn();
				Serial_SendString("LED_ON_OK");
			}
			
			if(strcmp(Serial_RXdataPackt,"LED_OFF")==0)
			{
				LED1_TurnOFF();
				Serial_SendString("LED_OFF_OK");
			}
			Serial_Flag==0;
		}
	}
}

