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
	OLED_ShowImage(0, 0, 15, 15, Smile_Img);
	for (uint8_t x = 0; x < 128; x += 4)
	{
		OLED_DrawPoint(sin(x / 128.0 * 2 * 3.14159) * 32 + 32, x);
	}
	OLED_ShowNum(32,80,666,3,16);
	OLED_UpdateScreen();
	while (1)
	{
	}
}
