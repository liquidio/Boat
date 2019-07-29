#include "led.h"
#include "delay.h"
#include "sys.h"
#include "remote.h"  
#include "pwm.h" 
#include "timer.h" 
#include "usart.h"

 int main(void)
 { 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2	  
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();		  		//初始化与LED连接的硬件接口
	//PID_Init();
  TIM1_PWM_Init(1439,999);
	 TIM_SetCompare1(TIM1,118);
	 Remote_Init();			//红外接收初始�
	TIM4_Int_Init(1439,999);	
	while(1)
	{
	}
}




















