#include "stm32f10x.h"                  // Device header
#include "MyFlash.h"

#define  Save_Address         0x0800Fc00
uint16_t Data[512];


void Store_Init(void)
{
	uint16_t i=0;
	if(MyFlash_ReadHalfWord(Save_Address)!=0xa5a5)
	{
		MyFlash_ErasePage(Save_Address);
		MyFlash_ProgramHalfWord(Save_Address,0xa5a5);
		for(i=1;i<512;i++)
			MyFlash_ProgramHalfWord(Save_Address+i*2,0);
	}
	for(i=0;i<512;i++)
		Data[i]=MyFlash_ReadHalfWord(Save_Address+i*2);	
}


void Store_Save(void)
{
	uint16_t i=0;
	MyFlash_ErasePage(Save_Address);
	for(i=0;i<512;i++)
	{
		MyFlash_ProgramHalfWord(Save_Address+i*2,Data[i]);
	}
}

void Store_Clear(void)
{
	uint16_t i=0;
	for(i=1;i<512;i++)
	{
		Data[i]=0x0000;
	}
	Store_Save();
	
}
