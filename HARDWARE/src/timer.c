#include "timer.h"
#include "led.h"
#include "remote.h"
#include "delay.h"
#include <math.h>
#define HUANG//����ʹ�����Ҵ��Ĳ���

#ifdef HUANG
u16 mid=800;//�м�ֵpwm
u16 left=550,right=1000;

double k1=1.1,k2=0.9,k3=2,k4=4;

u16 s1=70;

double k,pwm=800,par=800;

/*״̬��ʶ
0001����յ�
0010�ұ��յ�
0100ǰ��λ�õ�����ֵ��
1000ǰ��λ�õ�����ֵ��
*/
u8 pre;
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
	if(hw_cc4)LED1=0;
	else LED1=1;
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			control();
	}
TIM_ClearFlag(TIM4, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ
}
u8 flag =1;
u8 time0=0,time1=0;
u8 Location=0x00;//��ʼ����ǰλ��
u8 pre_Location=0x02;//��ʼ����һ�ε�λ��Ϊ��λ
void control(void){
	//ȷ��PWM��ֵ��λ��״̬
	flag=1;//ÿ��ֻ����һ���ź�
	if(hw_cc1 && flag){//������ź�
		pwm=left;
		flag=0;
		Location=0x04;//��־���
	}
	if(hw_cc7 && flag){//�ұ����ź�
		pwm=right;
		flag=0;
		Location=0x01;//��־�ұ�
	}
	if(hw_cc4 && flag){//��λ���ź�
		pwm=mid;
		flag=0;
		Location=0x02;//��־��λ
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
	pre_Location=Location;//������һ�ε�λ��״̬
	//ȷ������ϵ��k,�Լ�����ֶ�
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
	//����par�ķ�Χ
	if(par<=left)par =left;
	if(par>=right)par=right;
	//���	
	TIM_SetCompare1(TIM1,round(par));
	//������յ����ź�״̬
	hw_cc1=0;
	hw_cc7=0;
	hw_cc4=0;
}
