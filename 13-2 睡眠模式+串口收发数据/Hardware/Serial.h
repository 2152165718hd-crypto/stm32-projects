#ifndef __SERIAL_H__
#define __SERIAL_H__
#include <stdio.h>
void Serial_Init(void);
void Serial_SendByte(uint8_t Data);
void Serial_SendString(char *String);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendNum(uint32_t Num, uint8_t Length);
uint8_t Serial_GetData(void);
uint8_t Serial_GetFlag(void);
void Serial_PollHandler(void);
#endif
