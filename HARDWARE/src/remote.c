#include "remote.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK miniSTM32开发板
//红外遥控解码驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/12
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//红外遥控初始化
//设置IO以及定时器4的输入捕获
void Remote_Init(void)    			  
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;  
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE); //使能PORTB时钟 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3,ENABLE);	//TIM5 时钟使能 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_7;				 //PA1 输入 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//上拉输入 
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOA,GPIO_Pin_0);	//初始化GPIOA1
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	GPIO_SetBits(GPIOA,GPIO_Pin_2);
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
	GPIO_SetBits(GPIOA,GPIO_Pin_6);
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//上拉输入 
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_0);	//初始化GPIOB
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
						  
 	TIM_TimeBaseStructure.TIM_Period = 10000; //设定计数器自动重装值 最大10ms溢出  
	TIM_TimeBaseStructure.TIM_Prescaler =(72-1); 	//预分频器,1M的计数频率,1us加1.	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx
	
	TIM_TimeBaseStructure.TIM_Period = 10000; //设定计数器自动重装值 最大10ms溢出  
	TIM_TimeBaseStructure.TIM_Prescaler =(72-1); 	//预分频器,1M的计数频率,1us加1.	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx


  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;  // 选择输入端 IC2映射到TI5上
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 配置输入滤波器 8个定时器时钟周期滤波
  TIM_ICInit(TIM2, &TIM_ICInitStructure);//初始化定时器输入捕获通道

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;  // 选择输入端 IC2映射到TI5上
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 配置输入滤波器 8个定时器时钟周期滤波
  TIM_ICInit(TIM2, &TIM_ICInitStructure);//初始化定时器输入捕获通道
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;  // 选择输入端 IC2映射到TI5上
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 配置输入滤波器 8个定时器时钟周期滤波
  TIM_ICInit(TIM2, &TIM_ICInitStructure);//初始化定时器输入捕获通道
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;  // 选择输入端 IC2映射到TI5上
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 配置输入滤波器 8个定时器时钟周期滤波
  TIM_ICInit(TIM2, &TIM_ICInitStructure);//初始化定时器输入捕获通道
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;  // 选择输入端 IC2映射到TI5上
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 配置输入滤波器 8个定时器时钟周期滤波
  TIM_ICInit(TIM3, &TIM_ICInitStructure);//初始化定时器输入捕获通道
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;  // 选择输入端 IC2映射到TI5上
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 配置输入滤波器 8个定时器时钟周期滤波
  TIM_ICInit(TIM3, &TIM_ICInitStructure);//初始化定时器输入捕获通道
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;  // 选择输入端 IC2映射到TI5上
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 配置输入滤波器 8个定时器时钟周期滤波
  TIM_ICInit(TIM3, &TIM_ICInitStructure);//初始化定时器输入捕获通道
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;  // 选择输入端 IC2映射到TI5上
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 配置输入滤波器 8个定时器时钟周期滤波
  TIM_ICInit(TIM3, &TIM_ICInitStructure);//初始化定时器输入捕获通道

	TIM_Cmd(TIM3,ENABLE ); 
  TIM_Cmd(TIM2,ENABLE ); 	//使能定时器5
 
 						
	NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn;  //TIM5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器	
 
 	TIM_ITConfig( TIM2,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//允许更新中断 ,允许CC2IE捕获中断	
	
	NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn;  //TIM5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器	
 
 	TIM_ITConfig( TIM3,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//允许更新中断 ,允许CC2IE捕获中
}
  
u32 Dval0,Dval_in0,Dval1,Dval_in1,Dval2,Dval_in2,Dval3,Dval_in3,Dval4,Dval_in4,
    Dval5,Dval_in5,Dval6,Dval_in6,Dval7,Dval_in7,Dval_in8,Dval8;		//下降沿时计数器的值�	  
//定时器5中断服务程序	 
u8 cc1_more=0,cc2_more=0,cc3_more=0,cc4_more=0,cc5_more=0,cc6_more=0,cc7_more=0,cc8_more=0;
u8 hw_cc1=0,hw_cc2=0,hw_cc3=0,hw_cc4=0,hw_cc5=0,hw_cc6=0,hw_cc7=0,hw_cc8=0;

u8 	RmtSta1=0,RmtSta2=0,RmtSta3=0,RmtSta4=0,RmtSta5=0,RmtSta6=0,RmtSta7=0,RmtSta8=0;

u8 HW1_MARK=0,HW2_MARK=0,HW3_MARK=0,HW4_MARK=0,HW5_MARK=0,HW6_MARK=0,HW7_MARK=0,HW8_MARK=0;

void Remote(u8*RmtSta,u8*HW_MARK,u16 Dval,u8* hw_cch);

void TIM2_IRQHandler(void)
{ 		    	 
  if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	{
		cc1_more++;
		cc2_more++;
		cc3_more++;
		cc4_more++;
		if(cc1_more>=2) {cc1_more=0;HW1_MARK=0;}
		if(cc2_more>=2) {cc2_more=0;HW2_MARK=0;}
		if(cc3_more>=2) {cc3_more=0;HW3_MARK=0;}
		if(cc4_more>=2) {cc4_more=0;HW4_MARK=0;}
		TIM_ClearFlag(TIM2,TIM_IT_Update);	    
	}
	if(TIM_GetITStatus(TIM2,TIM_IT_CC1)!=RESET)
	{
		if(RDATA)//上升沿捕获
		{
			TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获				
	    cc1_more=0;
			Dval_in0=TIM_GetCapture1(TIM2);	
			RmtSta1|=0X10;
		}else //下降沿捕获
		{	
  		 Dval0=cc1_more*10000+TIM_GetCapture1(TIM2)-Dval_in0;//读取CCR1也可以清CC1IF标志位
			 TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC4P=0	设置为上升沿捕获
			 Remote(&RmtSta1,&HW1_MARK,Dval0,&hw_cc1);
		}
		TIM_ClearFlag(TIM2,TIM_IT_CC1);
	}	
	else
 	if(TIM_GetITStatus(TIM2,TIM_IT_CC2)!=RESET)
	{	  
		if(RDATA1)//上升沿捕获
		{
			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获				
	    cc2_more=0;
			Dval_in1=TIM_GetCapture2(TIM2);	
			RmtSta2|=0X10;
		}else//下降沿捕获
		{	
  		 Dval1=cc2_more*10000+TIM_GetCapture2(TIM2)-Dval_in1;//读取CCR1也可以清CC1IF标志位
			 TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC4P=0	设置为上升沿捕获
 			
			 Remote(&RmtSta2,&HW2_MARK,Dval1,&hw_cc2);
		}	
		TIM_ClearFlag(TIM2,TIM_IT_CC2);
	}
else
	if(TIM_GetITStatus(TIM2,TIM_IT_CC3)!=RESET)
	{	  
		if(RDATA2)//上升沿捕获
		{
			TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获				
	    cc3_more=0;
			Dval_in2=TIM_GetCapture3(TIM2);
			RmtSta3|=0X10;
		}else//下降沿捕获
		{			
  		 Dval2=cc3_more*10000+TIM_GetCapture3(TIM2)-Dval_in2;//读取CCR1也可以清CC1IF标志位
			 TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC4P=0	设置为上升沿捕获

				 Remote(&RmtSta3,&HW3_MARK,Dval2,&hw_cc3);
		}	
		TIM_ClearFlag(TIM2,TIM_IT_CC3);
	}	
	else
	if(TIM_GetITStatus(TIM2,TIM_IT_CC4)!=RESET)
	{	  
		if(RDATA3)//上升沿捕获
		{
			TIM_OC4PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获				
	    cc4_more=0;
			Dval_in3=TIM_GetCapture4(TIM2);	
			RmtSta4|=0X10;
		}else//下降沿捕获
		{			
  		 Dval3=cc4_more*10000+TIM_GetCapture4(TIM2)-Dval_in3;//读取CCR1也可以清CC1IF标志位
			 TIM_OC4PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC4P=0	设置为上升沿捕获
 			 Remote(&RmtSta4,&HW4_MARK,Dval3,&hw_cc4);
		}	
		TIM_ClearFlag(TIM2,TIM_IT_CC4);
	}	    
}


void TIM3_IRQHandler(void){ 		    	 
  if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{
		cc5_more++;
		cc6_more++;
		cc7_more++;
		cc8_more++;
		if(cc5_more>=2) {cc5_more=0;HW5_MARK=0;}
		if(cc6_more>=2) {cc6_more=0;HW6_MARK=0;}
		if(cc7_more>=2) {cc7_more=0;HW7_MARK=0;}
		if(cc8_more>=2) {cc8_more=0;HW8_MARK=0;}
		TIM_ClearFlag(TIM3,TIM_IT_Update);	    
	}
	if(TIM_GetITStatus(TIM3,TIM_IT_CC1)!=RESET)
	{	  
		if(RDATA4)//上升沿捕获
		{
			TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获				
	    cc5_more=0;
			Dval_in4=TIM_GetCapture1(TIM3);	
			RmtSta5|=0X10;
		}else //下降沿捕获
		{			
  		 Dval4=cc5_more*10000+TIM_GetCapture1(TIM3)-Dval_in4;//读取CCR1也可以清CC1IF标志位
			 TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC4P=0	设置为上升沿捕获
 			
			 Remote(&RmtSta5,&HW5_MARK,Dval4,&hw_cc5);
		}	
		TIM_ClearFlag(TIM3,TIM_IT_CC1);
	}	
	else
 	if(TIM_GetITStatus(TIM3,TIM_IT_CC2)!=RESET)
	{	  
		if(RDATA5)//上升沿捕获
		{
			TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获				
	    cc6_more=0;
			Dval_in5=TIM_GetCapture2(TIM3);
			RmtSta6|=0X10;
		}else//下降沿捕获
		{			
  		 Dval5=cc6_more*10000+TIM_GetCapture2(TIM3)-Dval_in5;//读取CCR1也可以清CC1IF标志位
			 TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC4P=0	设置为上升沿捕获
				
				 Remote(&RmtSta6,&HW6_MARK,Dval5,&hw_cc6);
		}	
		TIM_ClearFlag(TIM3,TIM_IT_CC2);
	}
else
	if(TIM_GetITStatus(TIM3,TIM_IT_CC3)!=RESET)
	{	  
		if(RDATB6)//上升沿捕获
		{
			TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获				
	    cc7_more=0;
			Dval_in6=TIM_GetCapture3(TIM3);	
			RmtSta7|=0X10;
		}else //下降沿捕获
		{			
  		 Dval6=cc7_more*10000+TIM_GetCapture3(TIM3)-Dval_in6;//读取CCR1也可以清CC1IF标志位
			 TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC4P=0	设置为上升沿捕获

			 Remote(&RmtSta7,&HW7_MARK,Dval6,&hw_cc7);
		}	
		TIM_ClearFlag(TIM3,TIM_IT_CC3);
	}else
		if(TIM_GetITStatus(TIM3,TIM_IT_CC4)!=RESET)
	{
		if(RDATB7)//上升沿捕获
		{
			TIM_OC4PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获				
	    cc8_more=0;
			Dval_in7=TIM_GetCapture4(TIM3);	
			RmtSta8|=0X10;
		}else //下降沿捕获
		{			
  		 Dval7=cc8_more*10000+TIM_GetCapture4(TIM3)-Dval_in7;//读取CCR1也可以清CC1IF标志位
			 TIM_OC4PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC4P=0	设置为上升沿捕获

				 Remote(&RmtSta8,&HW8_MARK,Dval7,&hw_cc8);
		}	
		TIM_ClearFlag(TIM3,TIM_IT_CC4);
	}	
}

int hw_xh=0,hw_ch0=0;

void Remote(u8*RmtSta,u8*HW_MARK,u16 Dval,u8* hw_cch)
{
		 if(*RmtSta&0X10)
			 {
				 if(*RmtSta&0X80)
				 {
					if(Dval>500&&Dval<1500)
					{
							(*hw_cch)++;
						  if(*hw_cch==20)  {*HW_MARK=1;*RmtSta&=0X7F;*hw_cch=0;}
					}
					else  
					{
						*HW_MARK=0;
						hw_cch=0;
						*RmtSta&=0X7F;
					}
				 }
				 else if(Dval>3800&&Dval<4200)
				 {
						*RmtSta|=0X80;
				 }
				 else   
				 {
					 *HW_MARK=0;
				 }
			 }
}




