#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Encode.h"
int16_t Count=0;
int main(void)
{
	OLED_Init();
	Encode_Init();
	OLED_ShowString(1,1,"Count:");
	while(1)
	{
		Count+=Get_EncodeCount();
		OLED_ShowSignedNum(1,7,Count,5);
	}
}

