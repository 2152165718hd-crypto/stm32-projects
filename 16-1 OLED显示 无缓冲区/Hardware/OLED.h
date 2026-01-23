#ifndef __OLED_H
#define __OLED_H

void OLED_Init(void);
void OLED_Clear(void);
void OLED_SetCursor(uint8_t Page, uint8_t Column);
void OLED_ShowChar(uint8_t Page, uint8_t Column, char Char,uint8_t FontSize);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String, uint8_t FontSize);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Num, uint8_t Length, uint8_t FontSize);
void OLED_ShowImage(uint8_t Page, uint8_t Column, uint8_t Width, uint8_t Height, const uint8_t *Image);
void OLED_ShowChinese(uint8_t Page, uint8_t Column, char *Chinese);
#endif
