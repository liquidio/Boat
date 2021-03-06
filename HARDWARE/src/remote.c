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
void TIM2_IRQHandler(void)
{ 		    	 
  if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	{
		cc1_more++;
		cc2_more++;
		cc3_more++;
		cc4_more++;
		TIM_ClearFlag(TIM2,TIM_IT_Update);	    
	}
	if(TIM_GetITStatus(TIM2,TIM_IT_CC1)!=RESET)
	{
		if(RDATA)//上升沿捕获
		{
			TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获				
	    cc1_more=0;
			Dval_in0=TIM_GetCapture1(TIM2);	
		}else //下降沿捕获
		{	
			
  		 Dval0=cc1_more*10000+TIM_GetCapture1(TIM2)-Dval_in0;//读取CCR1也可以清CC1IF标志位
			 TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC4P=0	设置为上升沿捕获
				if(Dval0>300&&Dval0<800)		//560为标准值
				{
					hw_cc1=1;
				}			else			 
				if(Dval0>1400&&Dval0<1800)		//为标准值
				{
					hw_cc1=1;
				}		else				 
 				if(Dval0>4200&&Dval0<4700)		//4500为标准值4.5ms
				{
					hw_cc1=1;
				}	else	
				if(Dval0>2000&&Dval0<2600)		//4500为标准值4.5ms
				{
					hw_cc1=1;
				}	else
					{hw_cc1=0;}
			
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
		}else//下降沿捕获
		{	
  		 Dval1=cc2_more*10000+TIM_GetCapture2(TIM2)-Dval_in1;//读取CCR1也可以清CC1IF标志位
			 TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC4P=0	设置为上升沿捕获
 			
				if(Dval1>300&&Dval1<800)		//560为标准值
				{
					hw_cc2=1;
				}	else					 
				if(Dval1>1400&&Dval1<1800)		//为标准值
				{
					hw_cc2=1;
				}	else					 
 				if(Dval1>4200&&Dval1<4700)		//4500为标准值4.5ms
				{
					hw_cc2=1;
				}else
				if(Dval1>2000&&Dval1<2600)		//4500为标准值4.5ms
				{
					hw_cc2=1;
				}	else{
					hw_cc2=0;
			}
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
		}else//下降沿捕获
		{			
  		 Dval2=cc3_more*10000+TIM_GetCapture3(TIM2)-Dval_in2;//读取CCR1也可以清CC1IF标志位
			 TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC4P=0	设置为上升沿捕获

				if(Dval2>300&&Dval2<800)		//560为标准值
				{
					hw_cc3=1;
				}else				 
				if(Dval2>1400&&Dval2<1800)		//为标准值
				{
					hw_cc3=1;
				}	else					 
 				if(Dval2>4200&&Dval2<4700)		//4500为标准值4.5ms
				{
					hw_cc3=1;
				}	else	
				if(Dval2>2000&&Dval2<2600)		//4500为标准值4.5ms
				{
					hw_cc3=1;
				}	else	
					{hw_cc3=0;}
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
		}else//下降沿捕获
		{			
  		 Dval3=cc4_more*10000+TIM_GetCapture4(TIM2)-Dval_in3;//读取CCR1也可以清CC1IF标志位
			 TIM_OC4PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC4P=0	设置为上升沿捕获
 			
				if(Dval3>300&&Dval3<800)		//560为标准值
				{
					hw_cc4=1;
				}		else				 
				if(Dval3>1400&&Dval3<1800)		//为标准值
				{
					hw_cc4=1;
				}		else				 
 				if(Dval3>4200&&Dval3<4700)		//4500为标准值4.5ms
				{
					hw_cc4=1;
				}		else
				if(Dval3>2000&&Dval3<2600)		//4500为标准值4.5ms
				{
					hw_cc4=1;
				}	else{hw_cc4=0;}					 
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
		TIM_ClearFlag(TIM3,TIM_IT_Update);	    
	}
	if(TIM_GetITStatus(TIM3,TIM_IT_CC1)!=RESET)
	{	  
		if(RDATA4)//上升沿捕获
		{
			TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获				
	    cc5_more=0;
			Dval_in4=TIM_GetCapture1(TIM3);	
		}else //下降沿捕获
		{			
  		 Dval4=cc5_more*10000+TIM_GetCapture1(TIM3)-Dval_in4;//读取CCR1也可以清CC1IF标志位
			 TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC4P=0	设置为上升沿捕获
 			
				if(Dval4>300&&Dval4<800)		//560为标准值
				{
					hw_cc5=1;
				}	else					 
				if(Dval4>1400&&Dval4<1800)		//为标准值
				{
					hw_cc5=1;
				}	else					 
 				if(Dval4>4200&&Dval4<4700)		//4500为标准值4.5ms
				{
					hw_cc5=1;
				}	else	
				if(Dval4>2000&&Dval4<2600)		//4500为标准值4.5ms
				{
					hw_cc5=1;
				}		else{hw_cc5=0;}				 
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
		}else//下降沿捕获
		{			
  		 Dval5=cc6_more*10000+TIM_GetCapture2(TIM3)-Dval_in5;//读取CCR1也可以清CC1IF标志位
			 TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC4P=0	设置为上升沿捕获
 			
				if(Dval5>300&&Dval5<800)		//560为标准值
				{
					hw_cc6=1;
				}		else				 
				if(Dval5>1400&&Dval5<1800)		//为标准值
				{
					hw_cc6=1;
				}		else				 
 				if(Dval5>4200&&Dval5<4700)		//4500为标准值4.5ms
				{
					hw_cc6=1;
				}		else
				if(Dval5>2000&&Dval5<2600)		//4500为标准值4.5ms
				{
					hw_cc6=1;
				}			else{hw_cc6=0;}			 
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

		}else //下降沿捕获
		{			
  		 Dval6=cc7_more*10000+TIM_GetCapture3(TIM3)-Dval_in6;//读取CCR1也可以清CC1IF标志位
			 TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC4P=0	设置为上升沿捕获

				if(Dval6>300&&Dval6<800)		//560为标准值
				{
					hw_cc7=1;
				}			else			 
				if(Dval6>1400&&Dval6<1800)		//为标准值
				{
					hw_cc7=1;
				}				else		 
 				if(Dval6>4200&&Dval6<4700)		//4500为标准值4.5ms
				{
					hw_cc7=1;
				}		else
				if(Dval6>2000&&Dval6<2600)		//4500为标准值4.5ms
				{
					hw_cc7=1;
				}		else	{hw_cc7=0;}			 
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

		}else //下降沿捕获
		{			
  		 Dval7=cc8_more*10000+TIM_GetCapture4(TIM3)-Dval_in7;//读取CCR1也可以清CC1IF标志位
			 TIM_OC4PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC4P=0	设置为上升沿捕获

				if(Dval7>300&&Dval7<800)		//560为标准值
				{
					hw_cc8=1;
				}			else			 
				if(Dval7>1400&&Dval7<1800)		//为标准值
				{
					hw_cc8=1;
				}				else		 
 				if(Dval7>4200&&Dval7<4700)		//4500为标准值4.5ms
				{
					hw_cc8=1;
				}		else
				if(Dval7>2000&&Dval7<2600)		//4500为标准值4.5ms
				{
					hw_cc8=1;
				}		else	{hw_cc8=0;}			 
		}	
		TIM_ClearFlag(TIM3,TIM_IT_CC4);
	}	
}
