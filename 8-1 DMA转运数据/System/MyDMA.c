#include "stm32f10x.h"                  // Device header

uint16_t My_BufferSize;
void MyDMA_Init(uint32_t MemoryBaseAddr,uint32_t PeripheralBaseAddr,uint16_t BufferSize)
{
	//存储BufferSize
	My_BufferSize=BufferSize;
	
	//开启DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	//初始化DMA，配置DMA参数 
	DMA_InitTypeDef   DMA_InitStruct;
	DMA_InitStruct.DMA_BufferSize=BufferSize;					 
	DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralSRC;				
	DMA_InitStruct.DMA_M2M=DMA_M2M_Enable;						
	DMA_InitStruct.DMA_MemoryBaseAddr=MemoryBaseAddr;
	DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_Mode=DMA_Mode_Normal;
	DMA_InitStruct.DMA_PeripheralBaseAddr=PeripheralBaseAddr;
	DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Enable;
	DMA_InitStruct.DMA_Priority=DMA_Priority_Medium;
	DMA_Init(DMA1_Channel1,&DMA_InitStruct);
	DMA_Cmd(DMA1_Channel1,DISABLE);
}

void MyDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1,My_BufferSize);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==RESET);
	DMA_ClearFlag(DMA1_FLAG_TC1);
}

