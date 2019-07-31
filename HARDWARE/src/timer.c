#include "timer.h"
#include "led.h"
#include "remote.h"
#include "delay.h"
#include <math.h>
//#define HUANG//¶¨ÒåÊ¹ÓÃÄÄËÒ´¬µÄ²ÎÊı
#define DEBUG
#ifdef DEBUG
u8 blue;
u8 pwm=118;//ÖĞ¼äÖµpwm
u8 l1,
	l2,
	l3,
	r1,
	r2,
	r3;
u8 k1=1,k2=1,k3=1,k4=1;
#endif

#ifdef HUANG //»Æ´¬
u8 pwm=118;//ÖĞ¼äÖµpwm
u8 l1,
	l2,
	l3,
	r1,
	r2,
	r3;
u8 k1,k2,k3,k4;
#endif

#ifdef BAI //°×´¬
u8 mid;//ÖĞ¼äÖµpwm
u8 l1,
	l2,
	l3,
	r1,
	r2,
	r3;
u8 k1,k2,k3,k4;//·Ö¶ÎµÄ±ÈÀıÏµÊı
#endif
#ifdef HONG //ºì´¬
u8 mid=118;//ÖĞ¼äÖµpwm
u8 l1,
	l2,
	l3,
	r1,
	r2,
	r3;
u8 k1,k2,k3,k4;//·Ö¶ÎµÄ±ÈÀıÏµÊı
#endif

#ifdef PO //ÆÆ¾ü
u8 mid;//ÖĞ¼äÖµpwm
u8 l1,
	l2,
	l3,
	r1,
	r2,
	r3
u8 k1,k2,k3,k4;//·Ö¶ÎµÄ±ÈÀıÏµÊı
#endif
#ifdef HAI //º£Ó¥
u8 mid;//ÖĞ¼äÖµpwm
u8 l1,
	l2,
	l3,
	r1,
	r2,
	r3;
u8 k1,k2,k3,k4;//·Ö¶ÎµÄ±ÈÀıÏµÊı
#endif
#ifdef WU //ÎŞÃû
u8 mid;//ÖĞ¼äÖµpwm
u8 l1,
	l2,
	l3,
	r1,
	r2,
	r3;
u8 k1,k2,k3,k4;//·Ö¶ÎµÄ±ÈÀıÏµÊı
#endif

int par,k;//Æ«²îºÍ±ÈÀıÏµÊ
u8 blue;

void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //Ê±ÖÓÊ¹ÄÜ
	
	
	TIM_TimeBaseStructure.TIM_Period = arr; //ÉèÖÃÔÚÏÂÒ»¸ö¸üĞÂÊÂ¼ş×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµ	 ¼ÆÊıµ½5000Îª500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //ÉèÖÃÓÃÀ´×÷ÎªTIMxÊ±ÖÓÆµÂÊ³ıÊıµÄÔ¤·ÖÆµÖµ  10KhzµÄ¼ÆÊıÆµÂÊ  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //ÉèÖÃÊ±ÖÓ·Ö¸î:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIMÏòÉÏ¼ÆÊıÄ£Ê½
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //¸ù¾İTIM_TimeBaseInitStructÖĞÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯TIMxµÄÊ±¼ä»ùÊıµ¥Î»
 
	
	TIM_ITConfig(  //Ê¹ÄÜ»òÕßÊ§ÄÜÖ¸¶¨µÄTIMÖĞ¶Ï
		TIM4, //TIM4
		TIM_IT_Update,
		ENABLE  //Ê¹ÄÜ
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4ÖĞ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //ÏÈÕ¼ÓÅÏÈ¼¶0¼¶
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //´ÓÓÅÏÈ¼¶3¼¶
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQÍ¨µÀ±»Ê¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);  //¸ù¾İNVIC_InitStructÖĞÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèNVIC¼Ä´æÆ÷

	TIM_Cmd(TIM4, ENABLE);  //Ê¹ÄÜTIMxÍâÉè
							 
}

void TIM4_IRQHandler(void)   //TIM3ÖĞ¶Ï
{
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //¼ì²éÖ¸¶¨µÄTIMÖĞ¶Ï·¢ÉúÓë·ñ:TIM ÖĞ¶ÏÔ´ 
		{
			control();
	TIM_ClearFlag(TIM4, TIM_IT_Update  );  //Çå³ıTIMxµÄÖĞ¶Ï´ı´¦ÀíÎ»:TIM ÖĞ¶ÏÔ´
	}
}

void control(void){
	#ifndef DEBUG
			if(hw_cc1&&hw_cc2&&hw_cc3&&hw_cc4&&hw_cc5&&hw_cc6&&hw_cc7){par=pwm;}
			else if((hw_cc2||hw_cc3)&&(hw_cc5||hw_cc6)&&!hw_cc4){
				par=l2;//Ñ¡ÔñÃÅ
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

		if(hw_cc4) par=pwm;//ÖĞ¼äÖµ
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
/*È·¶¨ÄÄÒ»¶ÎµÄ±ÈÀıÏµÊı*/
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
/*¼ÆËãÊä³öpwm*/
		if(par<(pwm-1)){
			par = par +k;
		}else if(par>(pwm+1)){
			par = par -k;
		}else{
			par = pwm;
		}
	TIM_SetCompare1(TIM1,par);
}

