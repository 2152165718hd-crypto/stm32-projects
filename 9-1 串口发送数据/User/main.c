#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "OLED.h"
#include "Serial.h"

uint8_t Dat[4]={0x41,0x42,0x43,0x44};
int main(void)
{
	OLED_Init();
	Serial_Init();
//	Serial_SendByte('A');
//	Serial_SendString("I Love CS\r\n");
//	Serial_SendArray(Dat,4);
//	Serial_SendNum(12345,5);
	printf("我喜欢CS");
	while(1)
	{
		
	}
}

