#include "timer.h"
#include "led.h"
#include "remote.h"
#include "delay.h"
#include <math.h>
//#define HUANG//����ʹ�����Ҵ��Ĳ���

#ifdef DEBUG
u8 blue;
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
int par,k;//ƫ��ͱ���ϵ��

void TIM4_Int_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;  
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //ʹ��PORTBʱ�� 
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//�������� 
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
		TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;  // ѡ������� IC2ӳ�䵽TI5��
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 ���������˲��� 8����ʱ��ʱ�������˲�
  TIM_ICInit(TIM4, &TIM_ICInitStructure);//��ʼ����ʱ�����벶��ͨ��
	
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM4, //TIM4
		TIM_IT_Update|TIM_IT_CC2 ,
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
			cc9_more++;
			control();
	
	TIM_ClearFlag(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ
	}

	if(TIM_GetITStatus(TIM4,TIM_IT_CC2)!=RESET)
	{	  
		if(RDATB8)//�����ز���
		{
			TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���				
	    cc9_more=0;
			Dval_in8=TIM_GetCapture1(TIM4);	

		}else //�½��ز���
		{			
  		 Dval8=cc9_more*10000+TIM_GetCapture1(TIM4)-Dval_in8;//��ȡCCR1Ҳ������CC1IF��־λ
			 TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Rising); //CC4P=0	����Ϊ�����ز���

				if(Dval8>300&&Dval8<800)		//560Ϊ��׼ֵ
				{
					hw_cc9=1;
				}			else			 
				if(Dval8>1400&&Dval8<1800)		//Ϊ��׼ֵ
				{
					hw_cc9=1;
				}				else		 
 				if(Dval8>4200&&Dval8<4700)		//4500Ϊ��׼ֵ4.5ms
				{
					hw_cc9=1;
				}		else
				if(Dval8>2000&&Dval8<2600)		//4500Ϊ��׼ֵ4.5ms
				{
					hw_cc9=1;
				}		else	{hw_cc9=0;}			 
		}	
		TIM_ClearFlag(TIM4,TIM_IT_CC2);
	}
}

void control(void){
	#ifndef DEBUG
	#ifdef HUANG //�ƴ�
			if(hw_cc1&&hw_cc2&&hw_cc3&&hw_cc4&&hw_cc5&&hw_cc6&&hw_cc7){TIM_SetCompare1(TIM1,118);}
			else if(hw_cc2&&hw_cc3&&hw_cc4&&hw_cc5&&hw_cc6){TIM_SetCompare1(TIM1,118);}
			else if(hw_cc2&&hw_cc3&&hw_cc5&&hw_cc6&&!hw_cc4){
				//ѡ����
			}else
			{
		if(hw_cc3&&hw_cc4&&hw_cc5){TIM_SetCompare1(TIM1,118);}
		if(hw_cc1&&hw_cc2&&hw_cc3)TIM_SetCompare1(TIM1,125);
		
		if(hw_cc1)TIM_SetCompare1(TIM1,140);

		if(hw_cc1&&hw_cc2)TIM_SetCompare1(TIM1,135);

		if(hw_cc2)TIM_SetCompare1(TIM1,130);

		if(hw_cc2&&hw_cc3)TIM_SetCompare1(TIM1,124);

		if(hw_cc3)TIM_SetCompare1(TIM1,126);
		//if(hw_cc8)TIM_SetCompare1(TIM1,122);
		if(hw_cc3&&hw_cc4)par =(int)(l1+l2)/2;

		if(hw_cc4) par=pwm;//�м�ֵ
		//if(hw_cc4&&hw_cc8)p
		if(hw_cc4&&hw_cc9)TIM_SetCompare1(TIM1,110);
		if(hw_cc9&&hw_cc5)TIM_SetCompare1(TIM1,135);

		if(hw_cc5)TIM_SetCompare1(TIM1,110);

		if(hw_cc5&&hw_cc6)TIM_SetCompare1(TIM1,95);

		if(hw_cc6)TIM_SetCompare1(TIM1,90);

		if(hw_cc6&&hw_cc7)TIM_SetCompare1(TIM1,85);

		if(hw_cc7)TIM_SetCompare1(TIM1,80);
		if(hw_cc5&&hw_cc6&&hw_cc7)TIM_SetCompare1(TIM1,98);}
#endif
/*ȷ����һ�εı���ϵ��*/
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
/*�������pwm*/
		if(par<(pwm-1)){
			par = par +k;
		}else if(par>(pwm+1)){
			par = par -k;
		}else{
			par = pwm;
		}
	TIM_SetCompare1(TIM1,par);
#endif
		
#ifdef DEBUG
		TIM_SetCompare1(TIM1,blue);
#endif
}

