#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "W25Q64.h"

int main(void)
{
	OLED_Init();
	W25Q64_Init();
	uint8_t Data[4]={0x00,0x11,0x22,0x33};
	uint8_t ReadData[4]={0};
	uint16_t DID = 0;
	uint8_t MID = 0;
	OLED_ShowString(1,1,"W25Q64 MID:");
	OLED_ShowString(2,1,"Device DID:");
	W25Q64_ReadID(&MID,&DID);
	OLED_ShowHexNum(1,12,MID,2);
	OLED_ShowHexNum(2,12,DID,4);
	W25Q64_SectorErase(0x000000);
	W25Q64_PageProgram(0x000000,Data,4);
	OLED_ShowHexNum(3,1,Data[0],2);
	OLED_ShowHexNum(3,4,Data[1],2);
	OLED_ShowHexNum(3,7,Data[2],2);
	OLED_ShowHexNum(3,10,Data[3],2);
	W25Q64_ReadData(0x000000,ReadData,4);
	OLED_ShowHexNum(4,1,ReadData[0],2);
	OLED_ShowHexNum(4,4,ReadData[1],2);
	OLED_ShowHexNum(4,7,ReadData[2],2);
	OLED_ShowHexNum(4,10,ReadData[3],2);
	while(1)
	{
		
	}
}

