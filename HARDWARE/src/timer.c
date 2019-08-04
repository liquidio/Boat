#include "timer.h"
#include "led.h"
#include "remote.h"
#include "delay.h"
#include <math.h>
#define HUANG//����ʹ�����Ҵ��Ĳ���

#ifdef HUANG
double mid=800,pwm=800,par=800;//�м�ֵpwm
u16 l1=550,
	l2=600,
	l3=700,
	r1=850,
	r2=910,
	r3=1000;
double k,k3=10,k4=15;
u16 s1=100;
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
u8 k1=0,k2=0,k3=0,k4=0;//�ֶεı���ϵ��
#endif
#ifdef HAI //��ӥ
u16 pwm=1021;//�м�ֵpwm
u16 l1=1150,
	l2=1100,
	l3= 1060,
	r1=1000,
	r2=980 ,
	r3=900;
u8 k1=8,k2=6,k3=3,k4=1,d=1;

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
u16 pre;
u8 time[11];
void clear_timex(){
	u8 x;
	for (x=0;x<11;x++){
		time[x] = 0;
	}
}
void scan(u8* time,u8 x,double direct){
	if(*(time+x)>=8){
		pwm = direct;
		clear_timex();
	}
	(*(time+x))++;
}
void control(void){
	
		if(hw_cc3&&hw_cc4&&hw_cc5){
			scan(time,0,mid);
		}
		if(hw_cc1){
			scan(time,1,l1);
		}
		if(hw_cc2){
			scan(time,2,l2);
		}
		if(hw_cc3){
			scan(time,3,l3);
		}
		if(hw_cc4){
			scan(time,4,mid);
		}
		if(hw_cc5){
			scan(time,5,r1);
		}
		if(hw_cc6){
			scan(time,6,r2);;
		}
		if(hw_cc7){
			scan(time,7,r3);
		}
		if((hw_cc1||hw_cc2||hw_cc3)&&(hw_cc5||hw_cc6||hw_cc7)&& !hw_cc4){
			scan(time,8,l1);
		}
		if(hw_cc4&&(hw_cc5||hw_cc6||hw_cc7)){
			scan(time,9,mid);
		}
	if(hw_cc1&&hw_cc2&&hw_cc3&&hw_cc4&&hw_cc5&&hw_cc6&&hw_cc7){
		scan(time,10,mid);
	}
	
		hw_cc1=0;
		hw_cc2=0;
		hw_cc3=0;
		hw_cc4=0;
		hw_cc5=0;
		hw_cc6=0;
		hw_cc7=0;
/*ȷ����һ�εı���ϵ��*/

		if(fabs(par-pwm)<=s1){
			k=k3;
		}else {
			k=k4;
		}
/*�������pwm*/
		if(par<=(pwm-k))
			{
			par = par +k;
		}else if(par>=(pwm+k))
			{
			par = par -k;
		}else
			{
			par =pwm;
		}
		par =(pre*6+par*4)/10;//rc�˲�����
		pre = par;
		
		if(par<l1)par=l1;
		if(par>r3)par =r3;

	TIM_SetCompare1(TIM1,round(par));
}
