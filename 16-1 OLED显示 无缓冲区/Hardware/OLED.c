#include "stm32f10x.h"
#include "OLED_Data.h"
#include <string.h>
const uint8_t init_seq[] = {
	0xAE, // Display OFF

	0xD5, 0x80, // Set Display Clock Divide Ratio/Osc Freq: 0x80 (default推荐)
	0xA8, 0x3F, // Set Multiplex Ratio: 63 (128x64必须0x3F)

	0xD3, 0x00, // Set Display Offset: 0

	0x40 | 0x00, // Set Display Start Line: 0

	0xA0 | 0x01, // Segment Re-map: 0xA1 (翻转左右，使column 0在左边)
	0xC8,		 // COM Output Scan Direction: reverse (C8，使page0在顶部)

	0xDA, 0x12, // COM Pins Hardware Config: 0x12 (128x64最常见，Alternative + no L/R remap)

	0x81, 0xCF, // Set Contrast: 0xCF (较高亮度，可调0x00~0xFF)

	0xD9, 0xF1, // Set Pre-charge Period: 0xF1 (内部电荷泵推荐)

	0xDB, 0x30, // Set VCOMH Deselect Level: 0x30 (~0.83 x VCC，常见值)

	0xA4, // Entire Display ON: resume to RAM content (A4正常，A5强制全亮)
	0xA6, // Normal Display (A6正显，A7反显)

	0x8D,0x14,
	0xAF // Display ON
};
void OLED_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)BitValue);

}

void OLED_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)BitValue);

}

void OLED_GPIO_Init(void)
{
	uint32_t i, j;

	for (i = 0; i < 1000; i++)
	{
		for (j = 0; j < 1000; j++)
			;
	}

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	OLED_W_SCL(1);
	OLED_W_SDA(1);
}



void OLED_I2C_Start(void)
{
	OLED_W_SDA(1);
	OLED_W_SCL(1);
	OLED_W_SDA(0);
	OLED_W_SCL(0);
}

void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA(!!(Byte & (0x80 >> i)));
		OLED_W_SCL(1);
		OLED_W_SCL(0);
	}

	OLED_W_SCL(1);
	OLED_W_SCL(0);
}

void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);
	OLED_I2C_SendByte(0x00);
	OLED_I2C_SendByte(Command);
	OLED_I2C_Stop();
}

void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);
	OLED_I2C_SendByte(0x40);
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}


void OLED_SetCursor(uint8_t Page, uint8_t Column)
{

	OLED_WriteCommand(0xB0 | Page);				 // 设置页位置
	OLED_WriteCommand(0x10 | ((Column & 0xF0) >> 4)); // 设置X位置高4位
	OLED_WriteCommand(0x00 | (Column & 0x0F));		 // 设置X位置低4位
}

void OLED_Clear(void)
{
	uint8_t i, page;
	for (page = 0; page < 8; page++) // 遍历8页
	{
		OLED_SetCursor(page, 0); // 设置光标位置到第j页，第0列
		for (i = 0; i < 128; i++) // 遍历128列
		{
			OLED_WriteData(0x00); // 写入0x00，清除该像素点
		}
	}
}

void OLED_Init(void)
{
	uint8_t i;

	OLED_GPIO_Init();

	for (i = 0; i < sizeof(init_seq); i++)
	{
		OLED_WriteCommand(init_seq[i]);
	}
	OLED_Clear();
}

void OLED_ShowImage(uint8_t Line, uint8_t Column, uint8_t Width, uint8_t Height, const uint8_t *Image)
{
	uint8_t i, j;
	uint8_t pages = Height / 8;

	for (i = 0; i < pages; i++)
	{
		OLED_SetCursor(Line + i, Column);
		for (j = 0; j < Width; j++)
		{
			OLED_WriteData(Image[i * Width + j]);
		}
	}
}
void OLED_ShowChar(uint8_t Page, uint8_t Column, char Char,uint8_t FontSize)
{
	uint8_t c = Char - ' '; // 得到偏移后的字符位置

	if (FontSize == 16)
	{
		OLED_ShowImage(Page, Column, 8, 16, &OLED_F8x16[c][0]);
	}
	else if (FontSize == 8)
	{
		OLED_ShowImage(Page, Column, 6, 8, &OLED_F6x8[c][0]);
	}
}

void OLED_ShowString(uint8_t Line, uint8_t Column, char *String, uint8_t FontSize)
{
	while (*String != '\0')
	{
		OLED_ShowChar(Line, Column, *String, FontSize);
		if(FontSize ==16)
		{
			Column +=8;
		}
		else if(FontSize ==8)
		{
			Column +=6;
		}
		String++;
	}
}

uint16_t power(uint8_t base, uint8_t exp)
{
	uint16_t result = 1;
	while (exp--)
	{
		result *= base;
	}
	return result;
}

void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
	uint8_t i;
	for (i = 0; i < Length; i++)
	{
		if(FontSize ==16)
		{
			OLED_ShowChar(Line, Column + (Length - i - 1) * 8, (Number / power(10, i)) % 10 + '0',16);
		}
		else if(FontSize ==8)
		{
			OLED_ShowChar(Line, Column + (Length - i - 1) * 6, (Number / power(10, i)) % 10 + '0',8);
		}
	}
}

void OLED_ShowChinese(uint8_t Page, uint8_t Column, char *ChineseString)
{
	char SigleChinese[4] = {0};
	uint8_t pChinese = 0;
	uint8_t pIndex = 0;

	for(uint8_t i=0; ChineseString[i] != '\0'; i++)
	{
		SigleChinese[pChinese++] = ChineseString[i];
		if(pChinese == 3)
		{
			pChinese =0;
			//搜索字模数据
			for (pIndex = 0; strcmp(OLED_CF16x16[pIndex].Index, "0") != 0; pIndex++)
			{
				if(strcmp(OLED_CF16x16[pIndex].Index, SigleChinese) == 0)
				{
					//找到对应字模数据
					OLED_ShowImage(Page, Column, 16, 16, OLED_CF16x16[pIndex].Data);
					Column += 16; //每个汉字占16列
					break;
				}
			}
		}	
	}
}
