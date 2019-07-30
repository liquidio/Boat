#include "led.h"
#include "delay.h"
#include "sys.h"
#include "remote.h"   
#include "timer.h" 
#include "usart.h"
#include "pwm.h"

extern u8 k1;
 int main(void)
 {
	 u8 len,t;
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2	  
	uart_init(9600);	 	//串口初始化为115200
	LED_Init();		  		//初始化与LED连接的硬件接口
	TIM1_PWM_Init(1439,999);
	 TIM_SetCompare1(TIM1,36);
	 Remote_Init();			//红外接收初始�
	TIM4_Int_Init(2000,720-1);	
	
	while(1)
	{	
		if(USART_RX_STA&0x8000)
		{	
			len=USART_RX_STA&0x3fff;
			for (t=0;t<len;t++)
			if(USART_RX_BUF[0]=='0'){
				USART1->DR=USART_RX_BUF[t];
				while((USART1->SR&0X40)==0);
			}
			USART_RX_STA=0;
		}
}
	}




















