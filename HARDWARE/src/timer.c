#include "timer.h"
#include "led.h"
#include "remote.h"
#include "delay.h"
#include <math.h>
//#define HUANG//����ʹ�����Ҵ��Ĳ���
#define DEBUG
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

#ifdef HUANG //�ƴ�
u8 pwm=118;//�м�ֵpwm
u8 l1,
	l2,
	l3,
	r1,
	r2,
	r3;
u8 k1,k2,k3,k4;
#endif

#ifdef BAI //�״�
u8 mid;//�м�ֵpwm
u8 l1,
	l2,
	l3,
	r1,
	r2,
	r3;
u8 k1,k2,k3,k4;//�ֶεı���ϵ��
#endif
#ifdef HONG //�촬
u8 mid=118;//�м�ֵpwm
u8 l1,
	l2,
	l3,
	r1,
	r2,
	r3;
u8 k1,k2,k3,k4;//�ֶεı���ϵ��
#endif

#ifdef PO //�ƾ�
u8 mid;//�м�ֵpwm
u8 l1,
	l2,
	l3,
	r1,
	r2,
	r3
u8 k1,k2,k3,k4;//�ֶεı���ϵ��
#endif
#ifdef HAI //��ӥ
u8 mid;//�м�ֵpwm
u8 l1,
	l2,
	l3,
	r1,
	r2,
	r3;
u8 k1,k2,k3,k4;//�ֶεı���ϵ��
#endif
#ifdef WU //����
u8 mid;//�м�ֵpwm
u8 l1,
	l2,
	l3,
	r1,
	r2,
	r3;
u8 k1,k2,k3,k4;//�ֶεı���ϵ��
#endif

int par,k;//ƫ��ͱ���ϵ�
u8 blue;

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
			if(hw_cc1&&hw_cc2&&hw_cc3&&hw_cc4&&hw_cc5&&hw_cc6&&hw_cc7){par=pwm;}
			else if((hw_cc2||hw_cc3)&&(hw_cc5||hw_cc6)&&!hw_cc4){
				par=l2;//ѡ����
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

		if(hw_cc4) par=pwm;//�м�ֵ
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
/*ȷ����һ�εı���ϵ��*/
#ifdef DEBUG
			par = blue;
			//k1,k2,k3
			//l1,l2,l3
#endif
		u8 e;
		e=fabs(par-pwm);
		if(e<=l1){
			k=k1;
		}else if((e>l1)&&(e<=l2)){
			k=k2;
		}else if ((e>l2)&&(e<=l3)){
			k=k3;
		}else {
		k=k4;
		}
/*�������pwm*/
		if(par<(pwm-1)){
			par = par +k;
		}else if(par>(pwm+1)){
			par = par -k;
		}else{
			par = pwm;
		}
	TIM_SetCompare1(TIM1,par);
}

