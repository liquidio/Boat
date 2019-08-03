#include "led.h"
#include "delay.h"
#include "sys.h"
#include "remote.h"   
#include "timer.h" 
#include "usart.h"
#include "pwm.h"
#include "ano_y.h"
#define HUANG
#define DEBUG

extern double k1,k2,k3,k4;
extern double par,pwm;
extern u16 s1;
#ifdef DEBUG
extern float KP_ANO_Y,KI_ANO_Y,KD_ANO_Y;
extern float KP_ANO_Y1,KI_ANO_Y1,KD_ANO_Y1;
extern float KP_ANO_Y2,KI_ANO_Y2,KD_ANO_Y2;
#endif
 int main(void)
 {
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 2	  
	uart_init(115200);	 	
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
#ifdef DEBUG
		mpu6050_send_data(0XF1,pwm,par,0,0,0,0);
		mpu6050_send_data(0XF2,k1,k2,k3,k4,0,0);
		mpu6050_send_data(0XF3,s1,0,0,0,0,0);
		//ANO_DT_send_data(0XF2,(int)k1,0,0,0,0,0);
//		if(KP_ANO_Y){k1 = KP_ANO_Y;}
//		if(KI_ANO_Y){k2 = KI_ANO_Y;}
//		if(KD_ANO_Y){k3 = KD_ANO_Y;}
//		if(KP_ANO_Y1){k4 = KP_ANO_Y1;}
//		if(KI_ANO_Y1){s3 = KI_ANO_Y1;}
//		if(KD_ANO_Y1){s2 = KI_ANO_Y1;}
#endif
	}
	}
