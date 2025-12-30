#ifndef __MYFLASH_H__
#define __MYFLASH_H__
uint16_t MyFlash_ReadHalfWord(uint32_t Address);
uint32_t MyFlash_ReadWord(uint32_t Address);
void MyFlash_ProgramHalfWord(uint32_t Address, uint16_t Data);
void MyFlash_ProgramWord(uint32_t Address, uint16_t Data);
void MyFlash_ErasePage(uint32_t Address);
void MyFlash_EraseAllPages(uint32_t Address, uint16_t Data);


#endif
