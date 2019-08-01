#include "timer.h"
#include "led.h"
#include "remote.h"
#include "delay.h"
#include <math.h>
#define HUANG//����ʹ�����Ҵ��Ĳ���

#ifdef HUANG
u16 mid=800;//�м�ֵpwm
u16 left=550,right=1000;
u8 k1=5,k2=10,k3=15,k4=20;
u16 s1=70,s2=150,s3=300;
u16 k,pwm,par=800;

/*״̬��ʶ
0001����յ�
0010�ұ��յ�
0100ǰ��λ�õ�����ֵ��
1000ǰ��λ�õ�����ֵ��
*/
u8 pre;
u8 yu,yu1;//������ֵ��
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
	//start@�������м�{		
		if(hw_cc1&&!hw_cc4&&!hw_cc7){//������ź�
			pwm=left;
			pre &=0xfd;//��־��ߣ�����ұ�
			control();
			}
		
		if(!hw_cc1 && !hw_cc4 && hw_cc7){
			pwm=right;
			pre &=0xfe;//��־�ұߣ�������
			control();
		}
		
		if(hw_cc7&&hw_cc1){//�������߶��յ��Ļ���ѡ��ߣ�
			pwm=left;
			control();
		}
		//end@������}
		
//		/*���ӳ�ʼλ�õ���Ŀ�ĵأ�����ֱ����ʻ���������У��ı�����������з��ȡ�
//		�ȶ�״̬�£�����һ��ģ��Ŀɼ�ⷶΧ�����Ұڶ�ǰ��
//		*/
		//start@�������η���{//	ϵ��k1,k2
		if(!hw_cc1 && !hw_cc7 && hw_cc4){
			if(pre&0x1){//��һ�����յ���λ����
				pwm=mid*k1;
				pre=0x4;
				control();
			}else if(pre&0x2){//��һ�����յ���λ����
				pwm=mid*k2;
				pre=0x8;
				control();
			}else{
				pwm=mid;
				control();
			}
		}
		//stop@�������η���}
	}
		
	
TIM_ClearFlag(TIM4, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ
}

void control(void){
/*ȷ����һ�εı���ϵ��*/
		//*�Ѿ� #define HUANG*/
		if(fabs(par-pwm)<=s1){
			k=k2;
		}else
			k=k3;
		
/*����ת���ٶȣ�ϵ��K*/
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
	if(par<=left)par =left;//��ֵ����
	if(par>=right)par=right;
		
	TIM_SetCompare1(TIM1,par);
		
	hw_cc1=0;//����źŽ��յ���״̬
	hw_cc7=0;
	hw_cc4=0;
	return;
}

