#include "stm32f10x.h"                  // Device header
#include "Delay.h"  

void MyI2C_WriteSDA(uint8_t Bit)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)Bit);
	Delay_us(10);
}

void MyI2C_WriteSCL(uint8_t Bit)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)Bit);
	Delay_us(10);
}

uint8_t MyI2C_ReadSDA(void)
{
	uint8_t Data=0;
	Data=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
	Delay_us(10);
	return Data;
}

void MyI2C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void MyI2C_Start(void)
{
	MyI2C_WriteSDA(1);
	MyI2C_WriteSCL(1);
	MyI2C_WriteSDA(0);
	MyI2C_WriteSCL(0);
}

void MyI2C_Stop(void)
{
	MyI2C_WriteSDA(0);
	MyI2C_WriteSCL(1);
	MyI2C_WriteSDA(1);
}

void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i=0;
	for(i=0;i<8;i++)
	{
		MyI2C_WriteSCL(0);
		MyI2C_WriteSDA((Byte & (0x80 >> i)) ? Bit_SET : Bit_RESET);
		MyI2C_WriteSCL(1);
	}
	MyI2C_WriteSCL(0);
}


uint8_t MyI2C_ReadByte(void)
{
	uint8_t i=0,Data=0;
	MyI2C_WriteSDA(1);
	MyI2C_WriteSCL(0);
	for(i=0;i<8;i++)
	{
		MyI2C_WriteSCL(1);
		if(MyI2C_ReadSDA()==1)
			Data |=(0x80>>i);
		MyI2C_WriteSCL(0);
	}
	return Data;
}

uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t Ack=0;
	MyI2C_WriteSDA(1);
	MyI2C_WriteSCL(0);
	MyI2C_WriteSCL(1);
	Ack=MyI2C_ReadSDA();
	MyI2C_WriteSCL(0);
	return Ack;
}

void MyI2C_SendAck(BitAction Bit)
{
	MyI2C_WriteSCL(0);
	MyI2C_WriteSDA(Bit);
	MyI2C_WriteSCL(1);
	MyI2C_WriteSCL(0);
}

