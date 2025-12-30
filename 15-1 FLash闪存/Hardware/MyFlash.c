#include "stm32f10x.h"   // Device header



uint16_t MyFlash_ReadHalfWord(uint32_t Address)
{
	return *((__IO uint16_t*) Address);
}

uint32_t MyFlash_ReadWord(uint32_t Address)
{
	return *((__IO uint32_t*) Address);
}

void MyFlash_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
	FLASH_Unlock();
	FLASH_ProgramHalfWord(Address,Data);
	FLASH_Lock();
}

void MyFlash_ProgramWord(uint32_t Address, uint16_t Data)
{
	FLASH_Unlock();
	FLASH_ProgramWord(Address,Data);
	FLASH_Lock();
}


void MyFlash_ErasePage(uint32_t Address)
{
	FLASH_Unlock();
	FLASH_ErasePage(Address);
	FLASH_Lock();
}

void MyFlash_EraseAllPages(uint32_t Address, uint16_t Data)
{
	FLASH_Unlock();
	FLASH_EraseAllPages();
	FLASH_Lock();
}

