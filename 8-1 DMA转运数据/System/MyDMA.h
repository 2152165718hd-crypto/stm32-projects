#ifndef __MYDMA_H__
#define __MYDMA_H__
void MyDMA_Init(uint32_t MemoryBaseAddr,uint32_t PeripheralBaseAddr,uint16_t BufferSize);
void MyDMA_Transfer(void);
#endif
