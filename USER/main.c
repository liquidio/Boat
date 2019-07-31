#include "led.h"
#include "delay.h"
#include "sys.h"
#include "remote.h"   
#include "timer.h" 
#include "usart.h"
#include "pwm.h"

#define HUANG

 int main(void)
 {
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 2	  
	//uart_init(9600);	 	
	LED_Init();		  		//led初始化
	TIM1_PWM_Init(14399,99);
	 Remote_Init();			//红外接收初始化
	TIM4_Int_Init(1000,720-1);	
//	delay_ms(600);
	#ifdef HUANG
	 TIM_SetCompare1(TIM1,800);
	#endif
	#ifdef BAI
	 TIM_SetCompare1(TIM1,1000);
	#endif
	#ifdef HONG
	 TIM_SetCompare1(TIM1,1030);
	#endif
	#ifdef HAI
	 TIM_SetCompare1(TIM1,1020);
	#endif
	#ifdef PO
	 TIM_SetCompare1(TIM1,1000);
	#endif
	#ifdef WU
	 TIM_SetCompare1(TIM1,1050);
	#endif
	while(1)
	{	
	}
	}
