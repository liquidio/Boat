#include "timer.h"
#include "led.h"
#include "pid.h"
#include "remote.h"
#include "delay.h"
#include <math.h>
#define HUANG
#ifdef HUANG
#define MID 118

#endif

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
u8 control_ms=0;//�������ʱ����1��10ms
u16 control_val=0,time;
void TIM4_IRQHandler(void)   //TIM3�ж�
{
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			cc9_more++;
			control_ms ++;
			time ++;
			if(control_ms==2)
			{
				control();
				control_ms =0;}
	
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
		if(hw_cc8)TIM_SetCompare1(TIM1,122);
		if(hw_cc3&&hw_cc4)TIM_SetCompare1(TIM1,120);

		if(hw_cc4) TIM_SetCompare1(TIM1,118);//�м�ֵ
		if(hw_cc4&&hw_cc8)TIM_SetCompare1(TIM1,122);
		if(hw_cc4&&hw_cc9)TIM_SetCompare1(TIM1,110);
		if(hw_cc9&&hw_cc5)TIM_SetCompare1(TIM1,135);

		if(hw_cc5)TIM_SetCompare1(TIM1,110);

		if(hw_cc5&&hw_cc6)TIM_SetCompare1(TIM1,95);

		if(hw_cc6)TIM_SetCompare1(TIM1,90);

		if(hw_cc6&&hw_cc7)TIM_SetCompare1(TIM1,85);

		if(hw_cc7)TIM_SetCompare1(TIM1,80);
		if(hw_cc5&&hw_cc6&&hw_cc7)TIM_SetCompare1(TIM1,98);}
			
	hw_cc1=0;hw_cc2=0;hw_cc3=0;hw_cc4=0;hw_cc5=0;hw_cc6=0;hw_cc7=0;hw_cc8=0;hw_cc9=0;
}

