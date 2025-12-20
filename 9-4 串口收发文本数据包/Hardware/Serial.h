#ifndef __SERIAL_H__
#define __SERIAL_H__
extern char Serial_RXdataPackt[100];
extern uint16_t Serial_Flag;
void Serial_Init(void);
void Serial_SendByte(uint16_t Data);
void Serial_SendString(char *String);
void Serial_SendArray(uint8_t * Array,uint16_t Length);
void Serial_SendNum(uint32_t Num,uint8_t Length);
void Serial_SendPacket(void);

#endif
