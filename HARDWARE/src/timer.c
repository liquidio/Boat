#include "timer.h"
#include "led.h"
#include "remote.h"
#include "delay.h"
#include <math.h>
#define HUANG//定义使用哪艘船的参数
//#define DEBUG
#ifdef DEBUG
u8 blue;
u8 pwm=118;//中间值pwm
u8 l1=100,
	l2=100,
	l3=100,
	r1=100,
	r2=100=100,
	r3=100;
u8 k1=1,k2=1,k3=1,k4=1;
#endif

#ifdef HUANG //黄船
u8 pwm=118;//中间值pwm
u8 l1,
	l2,
	l3,
	r1,
	r2,
	r3;
u8 k1,k2,k3,k4;
#endif

#ifdef BAI //白船
u8 mid;//中间值pwm
u8 l1,
	l2,
	l3,
	r1,
	r2,
	r3;
u8 k1,k2,k3,k4;//分段的比例系数
#endif
#ifdef HONG //红船
u8 mid=118;//中间值pwm
u8 l1,
	l2,
	l3,
	r1,
	r2,
	r3;
u8 k1,k2,k3,k4;//分段的比例系数
#endif

#ifdef PO //破军
u8 mid;//中间值pwm
u8 l1,
	l2,
	l3,
	r1,
	r2,
	r3
u8 k1,k2,k3,k4;//分段的比例系数
#endif
#ifdef HAI //海鹰
u8 mid;//中间值pwm
u8 l1,
	l2,
	l3,
	r1,
	r2,
	r3;
u8 k1,k2,k3,k4;//分段的比例系数
#endif
#ifdef WU //无名
u8 mid;//中间值pwm
u8 l1,
	l2,
	l3,
	r1,
	r2,
	r3;
u8 k1,k2,k3,k4;//分段的比例系数
#endif
int par,k;//偏差和比例系�
u8 blue;

void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	
	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM4, //TIM4
		TIM_IT_Update,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设
							 
}

void TIM4_IRQHandler(void)   //TIM3中断
{
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			control();
	TIM_ClearFlag(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源
	}
}

void control(void){
	#ifndef DEBUG
			if(hw_cc1&&hw_cc2&&hw_cc3&&hw_cc4&&hw_cc5&&hw_cc6&&hw_cc7){par=pwm;}
			else if((hw_cc2||hw_cc3)&&(hw_cc5||hw_cc6)&&!hw_cc4){
				par=l2;//选择门
			}else if(hw_cc4&&(hw_cc5||hw_cc6||hw_cc7)){
				par =pwm;
			}else
			{
		if(hw_cc3&&hw_cc4&&hw_cc5){par=pwm;}
		if(hw_cc1&&hw_cc2&&hw_cc3)par=l2;
		
		if(hw_cc1)par=l3;

		if(hw_cc1&&hw_cc2)par=round((l2+l3)/2);

		if(hw_cc2)par=l2;

		if(hw_cc2&&hw_cc3)par=round((l2+l1)/2);

		if(hw_cc3)par=l1;
		//if(hw_cc8)TIM_SetCompare1(TIM1,122);
		if(hw_cc3&&hw_cc4)par =round((l1+l2)/2);

		if(hw_cc4) par=pwm;//中间值
		//if(hw_cc4&&hw_cc8)p
		//if(hw_cc4&&hw_cc9)TIM_SetCompare1(TIM1,110);
		//if(hw_cc9&&hw_cc5)TIM_SetCompare1(TIM1,135);

		if(hw_cc5)par=r1;

		if(hw_cc5&&hw_cc6)par =round((r1+r2)/2);

		if(hw_cc6)par =r2;

		if(hw_cc6&&hw_cc7)par =round((r2+r3)/2);

		if(hw_cc7)par=r3;
		if(hw_cc5&&hw_cc6&&hw_cc7)par=r2;
			}
		hw_cc1=0;
		hw_cc2=0;
		hw_cc3=0;
			hw_cc4=0;
			hw_cc5=0;
			hw_cc6=0;
			hw_cc7=0;
#endif
/*确定哪一段的比例系数*/
#ifdef DEBUG
			par = blue;
			//k1,k2,k3
			//l1,l2,l3
#endif
		u8 e;
		e=fabs(par-pwm);
		if(e<l1){
			k=k1;
		}else if((e>=l1)&&(e<l2)){
			k=k2;
		}else if ((e>=l2)&&(e<=l3)){
			k=k3;
		}else {
		k=k4;
		}
/*计算输出pwm*/
		if(par<(pwm-1)){
			par = par +k;
		}else if(par>(pwm+1)){
			par = par -k;
		}else{
			par = pwm;
		}
	TIM_SetCompare1(TIM1,par);
}

