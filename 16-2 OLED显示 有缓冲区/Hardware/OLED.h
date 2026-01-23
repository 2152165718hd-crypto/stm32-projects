#ifndef __OLED_H
#define __OLED_H
extern uint8_t OLED_Buffer[1024];
void OLED_Init(void);
void OLED_Clear(void);
void OLED_SetCursor(uint8_t Page, uint8_t x);
void OLED_ShowChar(uint8_t y, uint8_t x, char Char,uint8_t FontSize);
void OLED_ShowString(uint8_t y, uint8_t x, char *String, uint8_t FontSize);
void OLED_ShowNum(uint8_t y, uint8_t x, uint32_t Num, uint8_t Length, uint8_t FontSize);
void OLED_ShowImage(uint8_t y, uint8_t x, uint8_t Width, uint8_t Height, const uint8_t *Image);
void OLED_ShowChinese(uint8_t y, uint8_t x, char *Chinese);
void OLED_UpdateScreen(void);
void OLED_ClearArea(uint8_t y, uint8_t x, uint8_t Width, uint8_t Height);
void OLED_DrawPoint(uint8_t y, uint8_t x);
#endif
