#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"
#include "MPU6050_Reg.h"

uint8_t ID;

int main(void)
{
	OLED_Init();
	MPU6050_Init();
	OLED_ShowString(1, 1, "ID:");
	ID = MPU6050_ReceiveData(MPU6050_WHO_AM_I);
	OLED_ShowHexNum(1, 4, ID, 2);
	MPU6050_Data_t sensorData;
	while (1)
	{
		MPU6050_GetPhysicalData(&sensorData);
		OLED_ShowFloat(2, 1, sensorData.Ax, 2, 2);
		OLED_ShowFloat(3, 1, sensorData.Ay, 2, 2);
		OLED_ShowFloat(4, 1, sensorData.Az, 2, 2);
		OLED_ShowFloat(2, 8, sensorData.Gx, 3, 2);
		OLED_ShowFloat(3, 8, sensorData.Gy, 3, 2);
		OLED_ShowFloat(4, 8, sensorData.Gz, 3, 2);
	}
}
