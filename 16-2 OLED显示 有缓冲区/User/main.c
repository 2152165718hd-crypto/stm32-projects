#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "LED.h"
#include "OLED.h"
#include "OLED_Data.h"
#include <math.h>
int main(void)
{
	OLED_Init();
	// 测试：将缓冲区的第一个字节设置为0xFF
	OLED_ShowImage(0,0,127,63,JJY_Img);
	OLED_UpdateScreen();
	while (1)
	{
	}
}
