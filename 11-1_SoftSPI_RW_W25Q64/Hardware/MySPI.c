#include "stm32f10x.h"                  // Device header


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
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7; // SCK MOSI CS
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;  // MISO
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);
}

void MySPI_WriteCLK(uint8_t Bit)
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_5,(BitAction)Bit); // SCK

}
void MySPI_WriteMOSI(uint8_t Bit)
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_7,(BitAction)Bit); // MOSI
}   

void MySPI_WriteCS(uint8_t Bit)
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)Bit); // CS
}

uint8_t MySPI_ReadMISO(void)
{
    return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6); // MISO
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
    uint8_t i;
    uint8_t ReceivedData = 0;
    for (i = 0; i < 8;i++)
    {
        MySPI_WriteMOSI(Data & (0x80 >> i)); // Send bit
        MySPI_WriteCLK(1); // CLK high
        if(MySPI_ReadMISO())
            ReceivedData |= (0x80 >> i); // Read bit
        MySPI_WriteCLK(0); // CLK low
    }
    return ReceivedData;
}
