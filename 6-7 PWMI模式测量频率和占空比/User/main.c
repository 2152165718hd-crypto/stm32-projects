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
	SetFreq(72-1,100-1);      						//Freq=72M/(PSC-1)/ARR       ARR=100
	Set_Compare(80);                        
	OLED_ShowString(1,1,"Freq:     Hz");
	OLED_ShowString(2,1,"Duty:00%");
	while(1)
	{
		OLED_ShowNum(1,6,IC_GetFreq(),5);
		OLED_ShowNum(2,6,IC_GetDuty(),2);
	}
}

