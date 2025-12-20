#ifndef __MCU6050_H__
#define __MCU6050_H__
void MPU6050_SendData(uint8_t Address,uint8_t Data);
uint16_t MPU6050_ReceiveData(uint8_t Address);
void MPU6050_Init(void);
typedef struct {
    float Ax; // 加速度 X轴 (单位: g)
    float Ay; // 加速度 Y轴 (单位: g)
    float Az; // 加速度 Z轴 (单位: g)
    float Gx; // 角速度 X轴 (单位: deg/s)
    float Gy; // 角速度 Y轴 (单位: deg/s)
    float Gz; // 角速度 Z轴 (单位: deg/s)
} MPU6050_Data_t;
void MPU6050_ReadBuffer(uint8_t RegAddress, uint8_t *pBuffer, uint16_t Length);
void MPU6050_GetPhysicalData(MPU6050_Data_t *data);
#endif
