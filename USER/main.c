#include "led.h"
#include "delay.h"
#include "sys.h"
#include "remote.h"  
#include "pwm.h" 
#include "timer.h" 
#include "usart.h"

 int main(void)
 { 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// ÉèÖÃÖĞ¶ÏÓÅÏÈ¼¶·Ö×é2	  
	uart_init(115200);	 	//´®¿Ú³õÊ¼»¯Îª115200
	LED_Init();		  		//³õÊ¼»¯ÓëLEDÁ¬½ÓµÄÓ²¼ş½Ó¿Ú
	//PID_Init();
  TIM1_PWM_Init(1439,999);
	 TIM_SetCompare1(TIM1,118);
	 Remote_Init();			//ºìÍâ½ÓÊÕ³õÊ¼»
	TIM4_Int_Init(1439,999);	
	while(1)
	{
	}
}




















