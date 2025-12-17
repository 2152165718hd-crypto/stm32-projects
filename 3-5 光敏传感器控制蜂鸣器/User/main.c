#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LightSensor.h"
#include "Buzzer.h"
uint8_t State;
int main(void)
{
	Buzzer_Init();
	LightSensor_Init();
	while(1)
	{
		State=Get_LightState();
		if(State==1)
			Buzzer_On();
		else
			Buzzer_OFF();
	}
}

