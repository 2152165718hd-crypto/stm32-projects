#include "stm32f10x.h"                  // Device header
#include "Key.h"
#include "OLED.h"
#include "Store.h"
uint8_t Keynum;
int main(void)
{
	OLED_Init();
	Store_Init();
	Key_Init();
	OLED_ShowString(1,1,"Flag:");
	OLED_ShowString(2,1,"Data:");
	while(1)
	{
		Keynum=Get_KeyNum();
		if(Keynum==1)
		{
			Data[1]++;
			Data[2]++;
			Data[3]++;
			Data[4]++;
			Store_Save();
		}
		
		if(Keynum==2)
		{
			Store_Clear();
		}
		OLED_ShowHexNum(1,6,Data[0],4);
		OLED_ShowHexNum(3,1,Data[1],4);
		OLED_ShowHexNum(3,6,Data[2],4);
		OLED_ShowHexNum(4,1,Data[3],4);
		OLED_ShowHexNum(4,6,Data[4],4);
	}
}

