#include "timer.h"
#include "led.h"
#include "remote.h"
#include "delay.h"
#include <math.h>
#define HAI//����ʹ�����Ҵ��Ĳ���
//#define DEBUG
#ifdef DEBUG
u8 blue;
u8 pwm=118;//�м�ֵpwm
u8 l1,
	l2,
	l3,
	r1,
	r2,
	r3;
u8 k1=1,k2=1,k3=1,k4=1;
#endif

#ifdef HUANG
u16 pwm=800;//�м�ֵpwm
u16 l1=550,
	l2=600,
	l3=700,
	r1=850,
	r2=910,
	r3=1000;
u8 k1=1,k2=1,k3=2,k4=4;
#endif


#ifdef BAI //�״�
u16 pwm=1050;//�м�ֵpwm
u16 l1=800,
	l2=850,
	l3=900,
	r1=1200,
	r2=1350,
	r3=1500;
u8 k1=1,k2=1,k3=1,k4=1;//�ֶεı���ϵ��
#endif
#ifdef HONG //�촬
u16 pwm=1030;//�м�ֵpwm
u16 l1=550,
	l2=750,
	l3=900,
	r1=1200,
	r2=1350,
	r3=1500;
u8 k1=1,k2=1,k3=1,k4=1;//�ֶεı���ϵ��
#endif

#ifdef PO //�ƾ�
u16 pwm=1000;//�м�ֵpwm
u16 l1=700,
	l2=750,
	l3=925,
	r1=1100,
	r2=1200,
	r3=1370;
u8 k1=1,k2=1,k3=1,k4=0;//�ֶεı���ϵ��
#endif
#ifdef HAI //��ӥ
u16 mid = 1030,left=750,right=1300;
u8 k1=10,k2=15,k3=20,k4=50;//�ֶεı���ϵ��
u16 s3=300,s2=150,s1=70;
#endif


#ifdef WU //无名
u16 pwm=1050;
u16 l1=750,
	l2=850,
	l3=950,
	r1=1250,
	r2=1400,
	r3=1550;
u8 k1=1,k2=1,k3=1,k4=1;

#endif

u16 par=0,k=0,pwm;//舵机转动量和比例系数

void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	
	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM4, //TIM4
		TIM_IT_Update,
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����
							 
}

void TIM4_IRQHandler(void)   //TIM3�ж�
{
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			control();
	TIM_ClearFlag(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ
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
/*ȷ����һ�εı���ϵ��*/
		//*�Ѿ� #define HAI*/
		if(fabs(par-pwm)<=s1){
			k=k1;
		}else if((fabs(par-pwm)>s1)&&(fabs(par-pwm)<=s2)){
			k=k2;
		}else if ((fabs(par-pwm)>s2)&&(fabs(par-pwm)<= s3)){
			k=k3;
		}else {
		k=k4;
		}

/*�������pwm*/
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

