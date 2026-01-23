#include "stm32f10x.h"
#include "OLED_Data.h"
#include <string.h>

#define OLED_SCL_PIN GPIO_Pin_8
#define OLED_SDA_PIN GPIO_Pin_9
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
uint8_t OLED_Buffer[8][128] = {0};

void OLED_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, OLED_SCL_PIN, (BitAction)BitValue);

}

void OLED_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, OLED_SDA_PIN, (BitAction)BitValue);
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
	GPIO_InitStructure.GPIO_Pin = OLED_SCL_PIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = OLED_SDA_PIN;
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


void OLED_SetCursor(uint8_t Page, uint8_t x)
{

	OLED_WriteCommand(0xB0 | Page);				 // 设置页位置
	OLED_WriteCommand(0x10 | ((x & 0xF0) >> 4)); // 设置X位置高4位
	OLED_WriteCommand(0x00 | (x & 0x0F));		 // 设置X位置低4位
}

void OLED_Clear(void)
{
	uint8_t i, page;
	for (page = 0; page < 8; page++) // 遍历8页
	{
		OLED_SetCursor(page, 0); // 设置光标位置到第j页，第0列
		for (i = 0; i < 128; i++) // 遍历128列
		{
			OLED_Buffer[page][i] = 0x00;
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

void OLED_UpdateScreen(void)
{
	uint8_t i, j;
	for (i = 0; i < 8; i++) // 遍历8页
	{
		OLED_SetCursor(i, 0); // 设置光标位置到第i页，第0列
		for (j = 0; j < 128; j++) // 遍历128列
		{
			OLED_WriteData(OLED_Buffer[i][j]);
		}
	}
}

void OLED_ClearArea(uint8_t y, uint8_t x, uint8_t w, uint8_t h)
{
	if (w == 0 || h == 0)
		return;

	// 防止越界（非常重要）
	if (x >= 128)
		return;
	if (y >= 64)
		return;
	if (x + w > 128)
		w = 128 - x;
	if (y + h > 64)
		h = 64 - y;

	uint8_t page_start = y / 8; // 从第几页开始
	uint8_t offset = y % 8;		// 在起始页里，从第几行开始（0~7）

	// 需要处理多少个页面（向上取整）
	uint8_t pages = (offset + h + 7) / 8;

	for (uint8_t p = 0; p < pages; p++) // 遍历受影响的每一页
	{
		uint8_t page = page_start + p;
		if (page >= 8)
			break; // 超出8页就停（64行OLED通常只有8页）

		// 这一页要清哪些位？默认全清
		uint8_t mask = 0xFF; // 1111 1111（意思：所有位都要处理）

		// 第一页特殊处理：只清 offset 及以上的位
		if (p == 0)
		{
			mask = 0xFF << offset; // 例如 offset=3 → 11111000
		}

		// 最后一页特殊处理：如果没用满8行，只清用到的低几位
		if (p == pages - 1 && (offset + h) % 8 != 0)
		{
			uint8_t bits_to_clear = (offset + h) % 8;
			if (bits_to_clear == 0)
				bits_to_clear = 8;
			uint8_t keep_mask = 0xFF >> (8 - bits_to_clear); // 例如只清低3位 → 00000111
			mask &= keep_mask;
		}

		// 真正清零：对这一页的指定列，保留不想清的位，清掉要清的位
		for (uint8_t col = x; col < x + w; col++)
		{
			OLED_Buffer[page][col] &= ~mask; // ~mask 才是真正要清零的那些位
		}
	}
}

void OLED_ShowImage(uint8_t y, uint8_t x, uint8_t Width, uint8_t Height, const uint8_t *Image)
{
	for(uint8_t j=0; j<(Height-1)/8+1; j++)
	{
		for (uint8_t i = 0; i < Width; i++)
		{
			OLED_Buffer[y / 8+j][x + i] |= Image[j*Width+i] << y % 8;
			OLED_Buffer[y / 8 + 1+j][x + i] |= Image[j*Width+i] >> 8 - y % 8;
		}
	}
}

void OLED_ShowChar(uint8_t y, uint8_t x, char Char,uint8_t FontSize)
{
	uint8_t c = Char - ' '; // 得到偏移后的字符位置

	if (FontSize == 16)
	{
		OLED_ShowImage(y, x, 8, 16, &OLED_F8x16[c][0]);
	}
	else if (FontSize == 8)
	{
		OLED_ShowImage(y, x, 6, 8, &OLED_F6x8[c][0]);
	}
}

void OLED_ShowString(uint8_t y, uint8_t x, char *String, uint8_t FontSize)
{
	while (*String != '\0')
	{
		OLED_ShowChar(y, x, *String, FontSize);
		if(FontSize ==16)
		{
			x +=8;
		}
		else if(FontSize ==8)
		{
			x +=6;
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

void OLED_ShowNum(uint8_t y, uint8_t x, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
	uint8_t i;
	for (i = 0; i < Length; i++)
	{
		if(FontSize ==16)
		{
			OLED_ShowChar(y, x + (Length - i - 1) * 8, (Number / power(10, i)) % 10 + '0',16);
		}
		else if(FontSize ==8)
		{
			OLED_ShowChar(y, x + (Length - i - 1) * 6, (Number / power(10, i)) % 10 + '0',8);
		}
	}
}

void OLED_ShowChinese(uint8_t y, uint8_t x, char *ChineseString)
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
					OLED_ShowImage(y, x, 16, 16, OLED_CF16x16[pIndex].Data);
					x += 16; //每个汉字占16列
					break;
				}
			}
		}	
	}
}

void OLED_DrawPoint(uint8_t y, uint8_t x)
{
	OLED_Buffer[y / 8][x] |= 0x01 << (y % 8);
}
