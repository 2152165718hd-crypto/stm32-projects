#ifndef __MY_I2C_H__
#define __MY_I2C_H__
void MyI2C_WriteSDA(uint8_t Bit);
void MyI2C_WriteSCL(uint8_t Bit);
uint8_t MyI2C_ReadSDA(void);
void MyI2C_Init(void);
void MyI2C_Start(void);
void MyI2C_Stop(void);
void MyI2C_SendByte(uint8_t Byte);
uint8_t MyI2C_ReadByte(void);
uint8_t MyI2C_ReceiveAck(void);
void MyI2C_SendAck(BitAction Bit);
#endif
