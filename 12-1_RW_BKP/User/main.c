#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Key.h"
#include "OLED.h"
uint8_t Keynum;
uint16_t WArry[4]={11,22,33,44};
uint16_t RArry[4]={0};
int main(void)
{
	OLED_Init();
	Key_Init();

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	
	
	while(1)
	{
		Keynum=Get_KeyNum();
		if(Keynum==1)
		{
			OLED_ShowNum(1,1,WArry[0],2);
			OLED_ShowNum(1,4,WArry[1],2);
			OLED_ShowNum(1,7,WArry[2],2);
			OLED_ShowNum(1,10,WArry[3],2);
			
			BKP_WriteBackupRegister(BKP_DR1,WArry[0]++);
			BKP_WriteBackupRegister(BKP_DR2,WArry[1]++);
			BKP_WriteBackupRegister(BKP_DR3,WArry[2]++);
			BKP_WriteBackupRegister(BKP_DR4,WArry[3]++);
			
		}
		
		RArry[0]=BKP_ReadBackupRegister(BKP_DR1);
		RArry[1]=BKP_ReadBackupRegister(BKP_DR2);
		RArry[2]=BKP_ReadBackupRegister(BKP_DR3);
		RArry[3]=BKP_ReadBackupRegister(BKP_DR4);
		
		OLED_ShowNum(2,1,RArry[0],2);
		OLED_ShowNum(2,4,RArry[1],2);
		OLED_ShowNum(2,7,RArry[2],2);
		OLED_ShowNum(2,10,RArry[3],2);
	}
}

