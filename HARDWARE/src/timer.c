#include "timer.h"
#include "led.h"
#include "remote.h"
#include "delay.h"
#include <math.h>
#define HUANG//定义使用哪艘船的参数

#ifdef HUANG
u16 mid=800;//中间值pwm
u16 left=550,right=1000;

double k1=1.1,k2=0.9,k3=2,k4=4;

u16 s1=70;

double k,pwm=800,par=800;

/*状态标识
0001左边收到
0010右边收到
0100前方位置的左阈值，
1000前方位置的右阈值，
*/
u8 pre;
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
	if(hw_cc4)LED1=0;
	else LED1=1;
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			control();
	}
TIM_ClearFlag(TIM4, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源
}
u8 flag =1;
u8 time0=0,time1=0;
u8 Location=0x00;//初始化当前位置
u8 pre_Location=0x02;//初始化上一次的位置为中位
void control(void){
	//确定PWM的值、位置状态
	flag=1;//每次只进入一个信号
	if(hw_cc1 && flag){//左边有信号
		pwm=left;
		flag=0;
		Location=0x04;//标志左边
	}
	if(hw_cc7 && flag){//右边有信号
		pwm=right;
		flag=0;
		Location=0x01;//标志右边
	}
	if(hw_cc4 && flag){//中位有信号
		pwm=mid;
		flag=0;
		Location=0x02;//标志中位
	}
	if(Location==0x02 && pre_Location==0x04){
		pwm=mid*k1;
	}
	else if(Location==0x02 && pre_Location==0x01){
		pwm=mid*k2;
	}
	if(Location!=0x02 && pre_Location==0x02){
		if(par>mid){
				pwm = mid*k2;
		}else
				pwm =mid*k1;
	}
	pre_Location=Location;//保存上一次的位置状态
	//确定调节系数k,自己可设分段
	if(fabs(par-pwm)<=s1){
		k=k3;
	}else{
		k=k4;
	}


	if(par<=(pwm-k)){
			par = par +k;
		}else if(par>=(pwm+k)){
			par = par -k;
		}else{
			par = pwm;
		}		
	//限制par的范围
	if(par<=left)par =left;
	if(par>=right)par=right;
	//输出	
	TIM_SetCompare1(TIM1,round(par));
	//清除接收到的信号状态
	hw_cc1=0;
	hw_cc7=0;
	hw_cc4=0;
}
