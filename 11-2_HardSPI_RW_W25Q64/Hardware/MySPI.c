#include "stm32f10x.h"                  // Device header

void MySPI_WriteCS(uint8_t Bit)
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)Bit); // CS
}

/*
*********************************************************
*               MySPI_Init
* Description : Initialize SPI GPIO pins
* Argument    : None
* Return      : None
*********************************************************
*/
void MySPI_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1,ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7; // SCK MOSI 
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;  // MISO
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;  // CS
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	SPI_InitTypeDef  SPI_InitStruct;
	SPI_InitStruct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_128 ;
	SPI_InitStruct.SPI_CPHA=SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_CPOL=SPI_CPOL_Low;
	SPI_InitStruct.SPI_CRCPolynomial=0x00;
	SPI_InitStruct.SPI_DataSize=SPI_DataSize_8b;
	SPI_InitStruct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit=SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_Mode=SPI_Mode_Master;
	SPI_InitStruct.SPI_NSS=SPI_NSS_Soft;
	SPI_Init(SPI1,&SPI_InitStruct);
	SPI_Cmd(SPI1,ENABLE);
	MySPI_WriteCS(1);
}
  



void MySPI_Start(void)
{
    MySPI_WriteCS(0); // CS low
}
void MySPI_Stop(void)
{
    MySPI_WriteCS(1); // CS high
}

uint8_t MySPI_TransferByte(uint8_t Data)
{
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI1,Data);
	
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);
	Data=SPI_I2S_ReceiveData(SPI1);
	
	return Data;
}
