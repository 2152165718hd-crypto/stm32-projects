#include "stm32f10x.h"                  // Device header
#include "IC.h"
#include "PWM.h"
#include "OLED.h"
int16_t i;
int main(void)
{
	OLED_Init();
	IC_Init();
	PWM_Init();
	SetFreq(72-1);      						//Freq=72M/(PSC-1)/ARR       ARR=100
	OLED_ShowString(1,1,"Freq:       Hz");
	while(1)
	{
		OLED_ShowNum(1,6,GetFreq(),5);
	}
}

