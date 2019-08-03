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
u8 k,k3=10,k4=15,s1=100;
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
u8 time0,time1,time2,time3,time4,time5,time6,time7,time8,time9,time10;
void control(void){
	
		if(hw_cc3&&hw_cc4&&hw_cc5){
			if(time3>=5){
				pwm=mid;time3=0;}
			time3++;
		}
		if(hw_cc1){
			if(time4>=5){
				pwm=l1;time4=0;}
			time4++;
		}
		if(hw_cc2){
			if(time5>=5){
				pwm=l2;time5=0;}
			time5++;
		}
		if(hw_cc3){
			if(time6>=5){
				pwm=l3;time6=0;}
			time6++;
		}
		if(hw_cc4){
			if(time7>=5){
				pwm=mid;time7=0;}
			time7++;
		}
		if(hw_cc5){
			if(time8>=5){
				pwm=r1;time8=0;}
			time8++;
		}
		if(hw_cc6){
			if(time9>=5){
				pwm=r2;time9=0;}
			time9++;
		}
		if(hw_cc7){
			if(time10>=5){
				pwm=r3;time10=0;}
			time10++;
		}
		if((hw_cc1||hw_cc2||hw_cc3)&&(hw_cc5||hw_cc6||hw_cc7)&& !hw_cc4){
			if(time2>=5){
				pwm=l1;time2=0;}
			time2++;
		}
		if(hw_cc4&&(hw_cc5||hw_cc6||hw_cc7)){
			if(time1>=10){
				pwm=mid;time1=0;}
			time1++;
		}
	if(hw_cc1&&hw_cc2&&hw_cc3&&hw_cc4&&hw_cc5&&hw_cc6&&hw_cc7){
		if(time0>=10){
			pwm=mid;time0=0;
		}
		time0++;
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
		par =(pre*6+par*4)/10;
		pre = par;
			if(par<l1)par=l1;
			if(par>r3)par =r3;

	TIM_SetCompare1(TIM1,round(par));
}
