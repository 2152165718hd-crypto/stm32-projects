#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "OLED.h"
#include "MyDMA.h"

uint8_t DadaA[3]={0,1,2};
uint8_t DadaB[3]={0};
int main(void)
{
	OLED_Init();
	MyDMA_Init((uint32_t)DadaB,(uint32_t)DadaA,3);
	OLED_ShowString(1,1,"DadaA:");
	OLED_ShowString(2,1,"AAdr:");
	OLED_ShowHexNum(2,6,(uint32_t)DadaA,8);
	OLED_ShowString(3,1,"DadaB:");
	OLED_ShowString(4,1,"BAdr:");
	OLED_ShowHexNum(4,6,(uint32_t)DadaB,8);
	
	while(1)
	{
		DadaA[0]++;
		DadaA[1]++;
		DadaA[2]++;
		OLED_ShowNum(1,7,DadaA[0],2);
		OLED_ShowNum(1,10,DadaA[1],2);
		OLED_ShowNum(1,13,DadaA[2],2);
		OLED_ShowNum(3,7,DadaB[0],2);
		OLED_ShowNum(3,10,DadaB[1],2);
		OLED_ShowNum(3,13,DadaB[2],2);
		MyDMA_Transfer();
		Delay_ms(1000);
		OLED_ShowNum(1,7,DadaA[0],2);
		OLED_ShowNum(1,10,DadaA[1],2);
		OLED_ShowNum(1,13,DadaA[2],2);
		OLED_ShowNum(3,7,DadaB[0],2);
		OLED_ShowNum(3,10,DadaB[1],2);
		OLED_ShowNum(3,13,DadaB[2],2);
		Delay_ms(1000);
	}
}

