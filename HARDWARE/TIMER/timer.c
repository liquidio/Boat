#include "timer.h"
#include "led.h"
#include "pid.h"
#include "remote.h"
#include "delay.h"
#include <math.h>
#define HUANG
#ifdef HUANG
#define MID 118

#endif

void TIM4_Int_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;  
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //使能PORTB时钟 
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//上拉输入 
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
		TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;  // 选择输入端 IC2映射到TI5上
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 配置输入滤波器 8个定时器时钟周期滤波
  TIM_ICInit(TIM4, &TIM_ICInitStructure);//初始化定时器输入捕获通道
	
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM4, //TIM4
		TIM_IT_Update|TIM_IT_CC2 ,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设
							 
}
u8 control_ms=0;//电机控制时间间隔1个10ms
u16 control_val=0,time;
void TIM4_IRQHandler(void)   //TIM3中断
{
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			cc9_more++;
			control_ms ++;
			time ++;
			if(control_ms==2)
			{
				control();
				control_ms =0;}
	
	TIM_ClearFlag(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源
	}

	if(TIM_GetITStatus(TIM4,TIM_IT_CC2)!=RESET)
	{	  
		if(RDATB8)//上升沿捕获
		{
			TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获				
	    cc9_more=0;
			Dval_in8=TIM_GetCapture1(TIM4);	

		}else //下降沿捕获
		{			
  		 Dval8=cc9_more*10000+TIM_GetCapture1(TIM4)-Dval_in8;//读取CCR1也可以清CC1IF标志位
			 TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Rising); //CC4P=0	设置为上升沿捕获

				if(Dval8>300&&Dval8<800)		//560为标准值
				{
					hw_cc9=1;
				}			else			 
				if(Dval8>1400&&Dval8<1800)		//为标准值
				{
					hw_cc9=1;
				}				else		 
 				if(Dval8>4200&&Dval8<4700)		//4500为标准值4.5ms
				{
					hw_cc9=1;
				}		else
				if(Dval8>2000&&Dval8<2600)		//4500为标准值4.5ms
				{
					hw_cc9=1;
				}		else	{hw_cc9=0;}			 
		}	
		TIM_ClearFlag(TIM4,TIM_IT_CC2);
	}
}

void control(void){

			if(hw_cc1&&hw_cc2&&hw_cc3&&hw_cc4&&hw_cc5&&hw_cc6&&hw_cc7){TIM_SetCompare1(TIM1,118);}
			else if(hw_cc2&&hw_cc3&&hw_cc4&&hw_cc5&&hw_cc6){TIM_SetCompare1(TIM1,118);}
			else if(hw_cc2&&hw_cc3&&hw_cc5&&hw_cc6&&!hw_cc4){
				//选择门
			}else
			{
		if(hw_cc3&&hw_cc4&&hw_cc5){TIM_SetCompare1(TIM1,118);}
		if(hw_cc1&&hw_cc2&&hw_cc3)TIM_SetCompare1(TIM1,125);
		
		if(hw_cc1)TIM_SetCompare1(TIM1,140);

		if(hw_cc1&&hw_cc2)TIM_SetCompare1(TIM1,135);

		if(hw_cc2)TIM_SetCompare1(TIM1,130);

		if(hw_cc2&&hw_cc3)TIM_SetCompare1(TIM1,124);

		if(hw_cc3)TIM_SetCompare1(TIM1,126);
		if(hw_cc8)TIM_SetCompare1(TIM1,122);
		if(hw_cc3&&hw_cc4)TIM_SetCompare1(TIM1,120);

		if(hw_cc4) TIM_SetCompare1(TIM1,118);//中间值
		if(hw_cc4&&hw_cc8)TIM_SetCompare1(TIM1,122);
		if(hw_cc4&&hw_cc9)TIM_SetCompare1(TIM1,110);
		if(hw_cc9&&hw_cc5)TIM_SetCompare1(TIM1,135);

		if(hw_cc5)TIM_SetCompare1(TIM1,110);

		if(hw_cc5&&hw_cc6)TIM_SetCompare1(TIM1,95);

		if(hw_cc6)TIM_SetCompare1(TIM1,90);

		if(hw_cc6&&hw_cc7)TIM_SetCompare1(TIM1,85);

		if(hw_cc7)TIM_SetCompare1(TIM1,80);
		if(hw_cc5&&hw_cc6&&hw_cc7)TIM_SetCompare1(TIM1,98);}
			
	hw_cc1=0;hw_cc2=0;hw_cc3=0;hw_cc4=0;hw_cc5=0;hw_cc6=0;hw_cc7=0;hw_cc8=0;hw_cc9=0;
}

