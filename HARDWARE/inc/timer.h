#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

extern u32 Dval_in8,cc9_more,hw_cc9,Dval8;
void TIM4_Int_Init(u16 arr,u16 psc); 
void control(void);

#endif
