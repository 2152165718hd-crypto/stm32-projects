#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include "OLED.h"
uint16_t Serial_Flag;
char Serial_RXdataPackt[100];

void Serial_Init(void)
{
	//开启外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//GPIO初始化
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//配置USART参数
	USART_InitTypeDef   USART_InitStruct;
	USART_InitStruct.USART_BaudRate=9600;
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode=USART_Mode_Tx |USART_Mode_Rx;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);
	
	//配置中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef   NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStruct);
	
	//使能USART
	USART_Cmd(USART1,ENABLE);
}

//发送一个字节
void Serial_SendByte(uint8_t Data)
{
	USART_SendData(USART1,Data);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}

//发送字符串
void Serial_SendString(char *String)
{
	uint16_t i=0;
	for(i=0;String[i]!='\0';i++)
	{
		USART_SendData(USART1,String[i]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);	
	}
}

//发送数组
void Serial_SendArray(uint8_t * Array,uint16_t Length)
{
	uint16_t i=0;
	for(i=0;i<Length;i++)
	{
		USART_SendData(USART1,Array[i]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);	
	}
}

//发送数字
uint16_t Power(uint32_t x,uint32_t y)
{
	uint32_t result=1;
	if(y>0)
	{
		while(y--)
			result *=x;
	}
	return result;
}
void Serial_SendNum(uint32_t Num,uint8_t Length)
{
	uint8_t i=0;
	for(i=0;i<Length;i++)
	{
		USART_SendData(USART1,Num/Power(10,Length-i-1)%10+'0');
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	}
}


//printf函数
int fputc(int ch,FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}



void USART1_IRQHandler(void)
{
	static uint8_t RXState=0;
	static uint8_t pPacket=0;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
	{
		uint8_t RXData=USART_ReceiveData(USART1);
		if(RXState==0)
		{
			if(RXData=='@' && Serial_Flag==0)
			{
				RXState=1;
				pPacket=0;
			}
		}
		else if(RXState==1)
		{
			if(RXData =='\r')
				RXState=2;
			else
			{
				Serial_RXdataPackt[pPacket]=RXData;
				pPacket++;
			}					
		}
		else if(RXState==2)
		{
			if(RXData=='\n')
			{
				RXState=0;
				Serial_Flag=1;
				Serial_RXdataPackt[pPacket]='\0';
			}
		}
	}
}
