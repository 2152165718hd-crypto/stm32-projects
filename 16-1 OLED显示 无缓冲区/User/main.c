#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "LED.h"
#include "OLED.h"
#include "OLED_Data.h"

int main(void)
{
	OLED_Init();
	OLED_Clear();
	OLED_ShowChinese(0, 0, "贺栋爱蒋静怡");
	while (1)
	{
	}
}
