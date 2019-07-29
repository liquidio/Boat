#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

extern u32 Dval_in8,cc9_more,hw_cc9,Dval8;
extern u8 hw_cc1,hw_cc2,hw_cc3,hw_cc4,hw_cc5,hw_cc6,hw_cc7;
void TIM4_Int_Init(u16 arr,u16 psc); 
void control(void);

#endif
