#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"
// MPU6050 灵敏度系数 (请根据你的 MPU6050_Init 初始化函数的设置修改此处)
// 加速度计配置 (AFS_SEL):
// 0: ±2g  -> 16384 LSB/g
// 1: ±4g  -> 8192 LSB/g
// 2: ±8g  -> 4096 LSB/g
// 3: ±16g -> 2048 LSB/g
#define MPU6050_ACCEL_SENSITIVITY  16384.0f  

// 陀螺仪配置 (FS_SEL):
// 0: ±250 °/s  -> 131.0 LSB/(°/s)
// 1: ±500 °/s  -> 65.5 LSB/(°/s)
// 2: ±1000 °/s -> 32.8 LSB/(°/s)
// 3: ±2000 °/s -> 16.4 LSB/(°/s)
#define MPU6050_GYRO_SENSITIVITY  	131.0f

void MPU6050_WaitEvent(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT)
{
    uint32_t timeout = 50000;
    while (I2C_CheckEvent(I2Cx, I2C_EVENT)!=SUCCESS)
    {
        if ((timeout--) == 0)
            break;
    }     
}

/*
*********************************************************************************************************
*	函 数 名: MPU6050_SendData
*	功能说明: 向MPU6050指定寄存器写入一个字节数据
*	形    参: Address：寄存器地址
*			  Data：要写入的数据
*	返 回 值: 无
*********************************************************************************************************
*/
void MPU6050_SendData(uint8_t Address, uint8_t Data){
    I2C_GenerateSTART(I2C2, ENABLE);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);

    I2C_Send7bitAddress(I2C2, MPU6050_ADDR, I2C_Direction_Transmitter);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

    I2C_SendData(I2C2, Address);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);

    I2C_SendData(I2C2, Data);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);

    I2C_GenerateSTOP(I2C2, ENABLE);
}
/*
*********************************************************************************************************
*	函 数 名: MPU6050_ReceiveData
*	功能说明: 从MPU6050指定寄存器读取一个字节数据
*	形    参: Address：寄存器地址
*	返 回 值: 读取到的数据
*********************************************************************************************************
*/
uint8_t MPU6050_ReceiveData(uint8_t Address)
{
    uint8_t Data = 0;

    I2C_GenerateSTART(I2C2, ENABLE);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);

    I2C_Send7bitAddress(I2C2, MPU6050_ADDR, I2C_Direction_Transmitter);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

    I2C_SendData(I2C2, Address);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);

    /* Repeated start for read */
    I2C_GenerateSTART(I2C2, ENABLE);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);

    I2C_Send7bitAddress(I2C2, MPU6050_ADDR, I2C_Direction_Receiver);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);

    /* For single byte read: disable ACK, generate STOP, then wait and read */
    I2C_AcknowledgeConfig(I2C2, DISABLE);
    I2C_GenerateSTOP(I2C2, ENABLE);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);

    Data = I2C_ReceiveData(I2C2);
    I2C_AcknowledgeConfig(I2C2, ENABLE); // 恢复 ACK 设置

    return Data;
}


/*
*********************************************************************************************************
*	函 数 名: MPU6050_ReadBuffer
*	功能说明: 从MPU6050指定寄存器读取多个字节数据
*	形    参: RegAddress：寄存器地址
*             pBuffer：数据缓冲区
*             Length：要读取的数据长度
*	返 回 值: 无
*********************************************************************************************************
*/
void MPU6050_ReadBuffer(uint8_t RegAddress, uint8_t *pBuffer, uint16_t Length)
{
    I2C_GenerateSTART(I2C2, ENABLE);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);

    I2C_SendData(I2C2, 0xD0);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

    I2C_SendData(I2C2, RegAddress);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
  
    I2C_GenerateSTART(I2C2, ENABLE);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
    I2C_SendData(I2C2, 0xD1);
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);

    for(uint16_t i=0; i<Length; i++)
    {
        if(i == (Length - 1))
        {
            I2C_AcknowledgeConfig(I2C2, DISABLE); // 最后一个字节，发送 NACK
            I2C_GenerateSTOP(I2C2, ENABLE);       // 产生停止条件
        }
        MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);
        pBuffer[i] = I2C_ReceiveData(I2C2);
    }
    I2C_AcknowledgeConfig(I2C2, ENABLE); // 恢复 ACK 设置     
}

/*
*********************************************************************************************************
*	函 数 名: MPU6050_Init
*	功能说明: 初始化MPU6050
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/

void MPU6050_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    I2C_InitTypeDef I2C_InitStruct;
    I2C_InitStruct.I2C_ClockSpeed = 100000;
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C2, &I2C_InitStruct);
    I2C_Cmd(I2C2, ENABLE);

    MPU6050_SendData(MPU6050_PWR_MGMT_1, 0x01);
    MPU6050_SendData(MPU6050_PWR_MGMT_2, 0x00);
    MPU6050_SendData(MPU6050_SMPLRT_DIV, 0x09);
    MPU6050_SendData(MPU6050_CONFIG, 0x06);
    MPU6050_SendData(MPU6050_GYRO_CONFIG, 0x00);
    MPU6050_SendData(MPU6050_ACCEL_CONFIG, 0x00);
}

typedef struct {
    float Ax; // 加速度 X轴 (单位: g)
    float Ay; // 加速度 Y轴 (单位: g)
    float Az; // 加速度 Z轴 (单位: g)
    float Gx; // 角速度 X轴 (单位: deg/s)
    float Gy; // 角速度 Y轴 (单位: deg/s)
    float Gz; // 角速度 Z轴 (单位: deg/s)
} MPU6050_Data_t;

void MPU6050_GetPhysicalData(MPU6050_Data_t *data)
{
    uint8_t buffer[14]; // 存储 ACCEL_X(2) + ACCEL_Y(2) + ACCEL_Z(2) + TEMP(2) + GYRO_X(2) + GYRO_Y(2) + GYRO_Z(2)
    int16_t raw_ax, raw_ay, raw_az, raw_gx, raw_gy, raw_gz;

    MPU6050_ReadBuffer(MPU6050_ACCEL_XOUT_H, buffer, 14);
    
    // 合成 16 位原始数据
    raw_ax = (int16_t)((buffer[0] << 8) | buffer[1]);
    raw_ay = (int16_t)((buffer[2] << 8) | buffer[3]);
    raw_az = (int16_t)((buffer[4] << 8) | buffer[5]);
    // buffer[6] 和 buffer[7] 是温度数据，这里跳过
    raw_gx = (int16_t)((buffer[8] << 8) | buffer[9]);
    raw_gy = (int16_t)((buffer[10] << 8) | buffer[11]);
    raw_gz = (int16_t)((buffer[12] << 8) | buffer[13]);

    // 转换为实际物理值
    data->Ax = (float)raw_ax / MPU6050_ACCEL_SENSITIVITY;
    data->Ay = (float)raw_ay / MPU6050_ACCEL_SENSITIVITY;
    data->Az = (float)raw_az / MPU6050_ACCEL_SENSITIVITY;

    data->Gx = (float)raw_gx / MPU6050_GYRO_SENSITIVITY;
    data->Gy = (float)raw_gy / MPU6050_GYRO_SENSITIVITY;
    data->Gz = (float)raw_gz / MPU6050_GYRO_SENSITIVITY;
}


