#ifndef __SERIAL_H__
#define __SERIAL_H__
extern uint8_t Serial_RXdataPackt[4];
extern uint8_t Serial_TXdataPackt[4];
void Serial_Init(void);
void Serial_SendByte(uint16_t Data);
void Serial_SendString(char *String);
void Serial_SendArray(uint8_t * Array,uint16_t Length);
void Serial_SendNum(uint32_t Num,uint8_t Length);
void Serial_SendPacket(void);
uint16_t Serial_GetFlag(void);
#endif
