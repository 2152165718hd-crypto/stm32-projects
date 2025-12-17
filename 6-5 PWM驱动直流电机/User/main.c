#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Motor.h"
#include "OLED.h"
#include "Key.h"
uint8_t Keynum;
int16_t Speed;
int main(void)
{
	OLED_Init();
	Motor_Init();
	Key_Init();
	OLED_ShowString(1,1,"Speed:");
	
	while(1)
	{
		Keynum= Get_KeyNum();
		if(Keynum==1)
		{
			Speed+=25;
			if(Speed>100)
				Speed=100;  // 达到最大速度后不再增加
		}
		if(Keynum==2)
		{
			Speed-=25;
			if(Speed<-100)
				Speed=-100;  // 减到-100为止
		}
		Motor_SetSpeed(Speed);
		OLED_ShowSignedNum(1,7,Speed,3);
	}
}

