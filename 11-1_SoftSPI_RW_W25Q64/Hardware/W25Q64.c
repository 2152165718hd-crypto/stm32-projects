#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_Ins.h"

void W25Q64_Init(void)
{
    MySPI_Init();
}

void W25Q64_WaitBusy(void)
{
    uint32_t timeout = 100000;
    MySPI_Start();
    MySPI_TransferByte(W25Q64_READ_STATUS_REGISTER_1);
    while (MySPI_TransferByte(W25Q64_DUMMY_BYTE) & 0x01)
    {
        if (--timeout == 0)
        {
            break; // Timeout to prevent infinite loop
        }
    }
    MySPI_Stop();
}

void W25Q64_ReadID(uint8_t *MID,uint16_t *DID)
{
    MySPI_Start();
    MySPI_TransferByte(W25Q64_JEDEC_ID);
    *MID = MySPI_TransferByte(W25Q64_DUMMY_BYTE);
    uint8_t did_high = MySPI_TransferByte(W25Q64_DUMMY_BYTE);
    uint8_t did_low = MySPI_TransferByte(W25Q64_DUMMY_BYTE);
    *DID = (did_high << 8) | did_low;
    MySPI_Stop();
    W25Q64_WaitBusy();
}

void W25Q64_WriteEnable(void)
{
    MySPI_Start();
    MySPI_TransferByte(W25Q64_WRITE_ENABLE);
    MySPI_Stop();
    W25Q64_WaitBusy();
}


void W25Q64_SectorErase(uint32_t Address)
{
    W25Q64_WriteEnable();
    MySPI_Start();
    MySPI_TransferByte(W25Q64_SECTOR_ERASE_4KB);
    MySPI_TransferByte((Address >> 16) & 0xFF);
    MySPI_TransferByte((Address >> 8) & 0xFF);
    MySPI_TransferByte(Address & 0xFF);
    MySPI_Stop();
    W25Q64_WaitBusy();
}

void W25Q64_PageProgram(uint32_t Address, uint8_t *Data, uint16_t Length)
{
    W25Q64_WriteEnable();
    MySPI_Start();
    MySPI_TransferByte(W25Q64_PAGE_PROGRAM);
    MySPI_TransferByte((Address >> 16) & 0xFF);
    MySPI_TransferByte((Address >> 8) & 0xFF);
    MySPI_TransferByte(Address & 0xFF);
    for (uint16_t i = 0; i < Length; i++)
    {
        MySPI_TransferByte(Data[i]);
    }
    MySPI_Stop();
    W25Q64_WaitBusy();
}

void W25Q64_ReadData(uint32_t Address, uint8_t *Data, uint16_t Length)
{
    MySPI_Start();
    MySPI_TransferByte(W25Q64_READ_DATA);
    MySPI_TransferByte((Address >> 16) & 0xFF);
    MySPI_TransferByte((Address >> 8) & 0xFF);
    MySPI_TransferByte(Address & 0xFF);
    for (uint16_t i = 0; i < Length; i++)
    {
        Data[i] = MySPI_TransferByte(W25Q64_DUMMY_BYTE);
    }
    MySPI_Stop();
}
