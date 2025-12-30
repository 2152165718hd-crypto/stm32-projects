#include "stm32f10x.h"                  // Device header

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}

void LED1_TurnOn(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
}
void LED1_TurnOFF(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
}
void LED2_TurnOn(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_2);
}
void LED2_TurnOFF(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
}

void LED1_Turn()
{
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_0)==1)
		GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	else
		GPIO_SetBits(GPIOA,GPIO_Pin_0);
}
void LED2_Turn()
{
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_2)==1)
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	else
		GPIO_SetBits(GPIOA,GPIO_Pin_2);
}

void LED_Turn(uint8_t Lednum)
{
	if(Lednum==1)
		LED1_Turn();

	if(Lednum==2)
		LED2_Turn();
}

