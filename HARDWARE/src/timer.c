#include "timer.h"
#include "led.h"
#include "remote.h"
#include "delay.h"
#include <math.h>
#define HUANG//定义使用哪艘船的参数

#ifdef HUANG
u16 mid=800;//中间值pwm
u16 left=550,right=1000;
u8 k1=5,k2=10,k3=15,k4=20;
u16 s1=70,s2=150,s3=300;
u16 k,pwm,par=800;

/*状态标识
0001左边收到
0010右边收到
0100前方位置的左阈值，
1000前方位置的右阈值，
*/
u8 pre;
u8 yu,yu1;//左右阈值；
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
	//start@矫正到中间{		
		if(hw_cc1&&!hw_cc4&&!hw_cc7){//左边有信号
			pwm=left;
			pre &=0xfd;//标志左边，清除右边
			control();
			}
		
		if(!hw_cc1 && !hw_cc4 && hw_cc7){
			pwm=right;
			pre &=0xfe;//标志右边，清除左边
			control();
		}
		
		if(hw_cc7&&hw_cc1){//左右两边都收到的话，选左边；
			pwm=left;
			control();
		}
		//end@矫正到}
		
//		/*船从初始位置到达目的地，不是直线行驶。而是蛇行，改变参数控制蛇行幅度。
//		稳定状态下，船在一个模块的可检测范围内左右摆动前行
//		*/
		//start@减少蛇形幅度{//	系数k1,k2
		if(!hw_cc1 && !hw_cc7 && hw_cc4){
			if(pre&0x1){//上一个接收到的位置左
				pwm=mid*k1;
				pre=0x4;
				control();
			}else if(pre&0x2){//上一个接收到的位置右
				pwm=mid*k2;
				pre=0x8;
				control();
			}else{
				pwm=mid;
				control();
			}
		}
		//stop@减少蛇形幅度}
	}
		
	
TIM_ClearFlag(TIM4, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源
}

void control(void){
/*确定哪一段的比例系数*/
		//*已经 #define HUANG*/
		if(fabs(par-pwm)<=s1){
			k=k2;
		}else
			k=k3;
		
/*控制转舵速度，系数K*/
		if(par<=(pwm-k))
			{
			par = par +k;
		}else if(par>=(pwm+k))
			{
			par = par -k;
		}else
			{
			par = pwm;
		}	
	if(par<=left)par =left;//阈值控制
	if(par>=right)par=right;
		
	TIM_SetCompare1(TIM1,par);
		
	hw_cc1=0;//清楚信号接收到的状态
	hw_cc7=0;
	hw_cc4=0;
	return;
}

