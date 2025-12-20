#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "Serial.h"
uint8_t Keynum;

int main(void)
{
	OLED_Init();
	Serial_Init();
	Key_Init();
	OLED_ShowString(1,1,"TXPacket:");
	OLED_ShowString(3,1,"RXPacket:");
	
	Serial_TXdataPackt[0]=1;
	Serial_TXdataPackt[1]=2;
	Serial_TXdataPackt[2]=3;
	Serial_TXdataPackt[3]=4;
	Serial_SendPacket();
	while(1)
	{
		Keynum=Get_KeyNum();
		if(Keynum==1)
		{
			Serial_TXdataPackt[0]++;
			Serial_TXdataPackt[1]++;
			Serial_TXdataPackt[2]++;
			Serial_TXdataPackt[3]++;
			Serial_SendPacket();
			OLED_ShowHexNum(2,1,Serial_TXdataPackt[0],2);
			OLED_ShowHexNum(2,4,Serial_TXdataPackt[1],2);
			OLED_ShowHexNum(2,7,Serial_TXdataPackt[2],2);
			OLED_ShowHexNum(2,10,Serial_TXdataPackt[3],2);
		}
		if(Serial_GetFlag()==1)
		{
			OLED_ShowHexNum(4,1,Serial_RXdataPackt[0],2);
			OLED_ShowHexNum(4,4,Serial_RXdataPackt[1],2);
			OLED_ShowHexNum(4,7,Serial_RXdataPackt[2],2);
			OLED_ShowHexNum(4,10,Serial_RXdataPackt[3],2);
		}
	}
}

