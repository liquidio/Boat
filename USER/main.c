#include "led.h"
#include "delay.h"
#include "sys.h"
#include "remote.h"  
#include "pwm.h" 
#include "timer.h" 
#include "usart.h"

 int main(void)
 { 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2	  
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	//PID_Init();
  TIM1_PWM_Init(1439,999);
	 TIM_SetCompare1(TIM1,36);
	 Remote_Init();			//������ճ�ʼ�
	TIM4_Int_Init(2000,720-1);	
	while(1)
	{
	}
}




















