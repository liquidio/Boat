#include "timer.h"
#include "led.h"
#include "remote.h"
#include "delay.h"
#include <math.h>
#define HAI//定义使用哪艘船的参数
//#define DEBUG
#ifdef DEBUG
u8 blue;
u8 pwm=118;//中间值pwm
u8 l1,
	l2,
	l3,
	r1,
	r2,
	r3;
u8 k1=1,k2=1,k3=1,k4=1;
#endif

#ifdef HUANG
u16 pwm=800;//中间值pwm
u16 l1=550,
	l2=600,
	l3=700,
	r1=850,
	r2=910,
	r3=1000;
u8 k1=1,k2=1,k3=2,k4=4;
#endif


#ifdef BAI //白船
u16 pwm=1050;//中间值pwm
u16 l1=800,
	l2=850,
	l3=900,
	r1=1200,
	r2=1350,
	r3=1500;
u8 k1=1,k2=1,k3=1,k4=1;//分段的比例系数
#endif
#ifdef HONG //红船
u16 pwm=1030;//中间值pwm
u16 l1=550,
	l2=750,
	l3=900,
	r1=1200,
	r2=1350,
	r3=1500;
u8 k1=1,k2=1,k3=1,k4=1;//分段的比例系数
#endif

#ifdef PO //破军
u16 pwm=1000;//中间值pwm
u16 l1=700,
	l2=750,
	l3=925,
	r1=1100,
	r2=1200,
	r3=1370;
u8 k1=1,k2=1,k3=1,k4=0;//分段的比例系数
#endif
#ifdef HAI //海鹰
u16 mid = 1030,left=750,right=1300;
u8 k1=10,k2=15,k3=20,k4=50;//分段的比例系数
u16 s3=300,s2=150,s1=70;
#endif


#ifdef WU //鏃犲悕
u16 pwm=1050;
u16 l1=750,
	l2=850,
	l3=950,
	r1=1250,
	r2=1400,
	r3=1550;
u8 k1=1,k2=1,k3=1,k4=1;

#endif

u16 par=0,k=0,pwm;//鑸垫満杞姩閲忓拰姣斾緥绯绘暟

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
		if(hw_cc4)pwm=mid;
		if(hw_cc1)pwm=left;
		if(hw_cc7)pwm=right;
		hw_cc1=0;
		hw_cc7=0;
			hw_cc4=0;
#endif
/*确定哪一段的比例系数*/
		//*已经 #define HAI*/
		if(fabs(par-pwm)<=s1){
			k=k1;
		}else if((fabs(par-pwm)>s1)&&(fabs(par-pwm)<=s2)){
			k=k2;
		}else if ((fabs(par-pwm)>s2)&&(fabs(par-pwm)<= s3)){
			k=k3;
		}else {
		k=k4;
		}

/*计算输出pwm*/
		if(par<(pwm-1))
			{
			par = par +k;
		}else if(par>(pwm+1))
			{
			par = par -k;
		}else
			{
			par = pwm;
		}
	TIM_SetCompare1(TIM1,par);
}

