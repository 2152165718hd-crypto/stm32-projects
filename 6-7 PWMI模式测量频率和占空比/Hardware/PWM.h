#ifndef __PWM_H__
#define __PWM_H__
void PWM_Init(void);
void Set_Compare(uint16_t Compare);
void SetFreq(uint16_t PSC,uint16_t ARR);
#endif
