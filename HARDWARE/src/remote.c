#include "remote.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌÐòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßÐí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ALIENTEK miniSTM32¿ª·¢°å
//ºìÍâÒ£¿Ø½âÂëÇý¶¯ ´úÂë	   
//ÕýµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.com
//ÐÞ¸ÄÈÕÆÚ:2012/9/12
//°æ±¾£ºV1.0
//°æÈ¨ËùÓÐ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ¹ãÖÝÊÐÐÇÒíµç×Ó¿Æ¼¼ÓÐÏÞ¹«Ë¾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//ºìÍâÒ£¿Ø³õÊ¼»¯
//ÉèÖÃIOÒÔ¼°¶¨Ê±Æ÷4µÄÊäÈë²¶»ñ
void Remote_Init(void)    			  
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;  
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE); //Ê¹ÄÜPORTBÊ±ÖÓ 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3,ENABLE);	//TIM5 Ê±ÖÓÊ¹ÄÜ 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_7;				 //PA1 ÊäÈë 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//ÉÏÀ­ÊäÈë 
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOA,GPIO_Pin_0);	//³õÊ¼»¯GPIOA1
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	GPIO_SetBits(GPIOA,GPIO_Pin_2);
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
	GPIO_SetBits(GPIOA,GPIO_Pin_6);
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//ÉÏÀ­ÊäÈë 
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_0);	//³õÊ¼»¯GPIOB
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
						  
 	TIM_TimeBaseStructure.TIM_Period = 10000; //Éè¶¨¼ÆÊýÆ÷×Ô¶¯ÖØ×°Öµ ×î´ó10msÒç³ö  
	TIM_TimeBaseStructure.TIM_Prescaler =(72-1); 	//Ô¤·ÖÆµÆ÷,1MµÄ¼ÆÊýÆµÂÊ,1us¼Ó1.	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ÉèÖÃÊ±ÖÓ·Ö¸î:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIMÏòÉÏ¼ÆÊýÄ£Ê½
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯TIMx
	
	TIM_TimeBaseStructure.TIM_Period = 10000; //Éè¶¨¼ÆÊýÆ÷×Ô¶¯ÖØ×°Öµ ×î´ó10msÒç³ö  
	TIM_TimeBaseStructure.TIM_Prescaler =(72-1); 	//Ô¤·ÖÆµÆ÷,1MµÄ¼ÆÊýÆµÂÊ,1us¼Ó1.	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ÉèÖÃÊ±ÖÓ·Ö¸î:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIMÏòÉÏ¼ÆÊýÄ£Ê½
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯TIMx


  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;  // Ñ¡ÔñÊäÈë¶Ë IC2Ó³Éäµ½TI5ÉÏ
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//ÉÏÉýÑØ²¶»ñ
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ 
  TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 ÅäÖÃÊäÈëÂË²¨Æ÷ 8¸ö¶¨Ê±Æ÷Ê±ÖÓÖÜÆÚÂË²¨
  TIM_ICInit(TIM2, &TIM_ICInitStructure);//³õÊ¼»¯¶¨Ê±Æ÷ÊäÈë²¶»ñÍ¨µÀ

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;  // Ñ¡ÔñÊäÈë¶Ë IC2Ó³Éäµ½TI5ÉÏ
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//ÉÏÉýÑØ²¶»ñ
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ 
  TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 ÅäÖÃÊäÈëÂË²¨Æ÷ 8¸ö¶¨Ê±Æ÷Ê±ÖÓÖÜÆÚÂË²¨
  TIM_ICInit(TIM2, &TIM_ICInitStructure);//³õÊ¼»¯¶¨Ê±Æ÷ÊäÈë²¶»ñÍ¨µÀ
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;  // Ñ¡ÔñÊäÈë¶Ë IC2Ó³Éäµ½TI5ÉÏ
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//ÉÏÉýÑØ²¶»ñ
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ 
  TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 ÅäÖÃÊäÈëÂË²¨Æ÷ 8¸ö¶¨Ê±Æ÷Ê±ÖÓÖÜÆÚÂË²¨
  TIM_ICInit(TIM2, &TIM_ICInitStructure);//³õÊ¼»¯¶¨Ê±Æ÷ÊäÈë²¶»ñÍ¨µÀ
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;  // Ñ¡ÔñÊäÈë¶Ë IC2Ó³Éäµ½TI5ÉÏ
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//ÉÏÉýÑØ²¶»ñ
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ 
  TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 ÅäÖÃÊäÈëÂË²¨Æ÷ 8¸ö¶¨Ê±Æ÷Ê±ÖÓÖÜÆÚÂË²¨
  TIM_ICInit(TIM2, &TIM_ICInitStructure);//³õÊ¼»¯¶¨Ê±Æ÷ÊäÈë²¶»ñÍ¨µÀ
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;  // Ñ¡ÔñÊäÈë¶Ë IC2Ó³Éäµ½TI5ÉÏ
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//ÉÏÉýÑØ²¶»ñ
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ 
  TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 ÅäÖÃÊäÈëÂË²¨Æ÷ 8¸ö¶¨Ê±Æ÷Ê±ÖÓÖÜÆÚÂË²¨
  TIM_ICInit(TIM3, &TIM_ICInitStructure);//³õÊ¼»¯¶¨Ê±Æ÷ÊäÈë²¶»ñÍ¨µÀ
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;  // Ñ¡ÔñÊäÈë¶Ë IC2Ó³Éäµ½TI5ÉÏ
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//ÉÏÉýÑØ²¶»ñ
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ 
  TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 ÅäÖÃÊäÈëÂË²¨Æ÷ 8¸ö¶¨Ê±Æ÷Ê±ÖÓÖÜÆÚÂË²¨
  TIM_ICInit(TIM3, &TIM_ICInitStructure);//³õÊ¼»¯¶¨Ê±Æ÷ÊäÈë²¶»ñÍ¨µÀ
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;  // Ñ¡ÔñÊäÈë¶Ë IC2Ó³Éäµ½TI5ÉÏ
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//ÉÏÉýÑØ²¶»ñ
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ 
  TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 ÅäÖÃÊäÈëÂË²¨Æ÷ 8¸ö¶¨Ê±Æ÷Ê±ÖÓÖÜÆÚÂË²¨
  TIM_ICInit(TIM3, &TIM_ICInitStructure);//³õÊ¼»¯¶¨Ê±Æ÷ÊäÈë²¶»ñÍ¨µÀ
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;  // Ñ¡ÔñÊäÈë¶Ë IC2Ó³Éäµ½TI5ÉÏ
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//ÉÏÉýÑØ²¶»ñ
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ 
  TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 ÅäÖÃÊäÈëÂË²¨Æ÷ 8¸ö¶¨Ê±Æ÷Ê±ÖÓÖÜÆÚÂË²¨
  TIM_ICInit(TIM3, &TIM_ICInitStructure);//³õÊ¼»¯¶¨Ê±Æ÷ÊäÈë²¶»ñÍ¨µÀ

	TIM_Cmd(TIM3,ENABLE ); 
  TIM_Cmd(TIM2,ENABLE ); 	//Ê¹ÄÜ¶¨Ê±Æ÷5
 
 						
	NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn;  //TIM5ÖÐ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //ÏÈÕ¼ÓÅÏÈ¼¶0¼¶
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //´ÓÓÅÏÈ¼¶3¼¶
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQÍ¨µÀ±»Ê¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);  //¸ù¾ÝNVIC_InitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèNVIC¼Ä´æÆ÷	
 
 	TIM_ITConfig( TIM2,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//ÔÊÐí¸üÐÂÖÐ¶Ï ,ÔÊÐíCC2IE²¶»ñÖÐ¶Ï	
	
	NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn;  //TIM5ÖÐ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //ÏÈÕ¼ÓÅÏÈ¼¶0¼¶
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //´ÓÓÅÏÈ¼¶3¼¶
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQÍ¨µÀ±»Ê¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);  //¸ù¾ÝNVIC_InitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèNVIC¼Ä´æÆ÷	
 
 	TIM_ITConfig( TIM3,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//ÔÊÐí¸üÐÂÖÐ¶Ï ,ÔÊÐíCC2IE²¶»ñÖÐ
}
  
u32 Dval0,Dval_in0,Dval1,Dval_in1,Dval2,Dval_in2,Dval3,Dval_in3,Dval4,Dval_in4,
    Dval5,Dval_in5,Dval6,Dval_in6,Dval7,Dval_in7,Dval_in8,Dval8;		//ÏÂ½µÑØÊ±¼ÆÊýÆ÷µÄÖµý	  
//¶¨Ê±Æ÷5ÖÐ¶Ï·þÎñ³ÌÐò	 
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
		if(RDATA)//ÉÏÉýÑØ²¶»ñ
		{
			TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 ÉèÖÃÎªÏÂ½µÑØ²¶»ñ				
	    cc1_more=0;
			Dval_in0=TIM_GetCapture1(TIM2);	
			RmtSta1|=0X10;
		}else //ÏÂ½µÑØ²¶»ñ
		{	
  		 Dval0=cc1_more*10000+TIM_GetCapture1(TIM2)-Dval_in0;//¶ÁÈ¡CCR1Ò²¿ÉÒÔÇåCC1IF±êÖ¾Î»
			 TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC4P=0	ÉèÖÃÎªÉÏÉýÑØ²¶»ñ
			 Remote(&RmtSta1,&HW1_MARK,Dval0,&hw_cc1);
		}
		TIM_ClearFlag(TIM2,TIM_IT_CC1);
	}	
	else
 	if(TIM_GetITStatus(TIM2,TIM_IT_CC2)!=RESET)
	{	  
		if(RDATA1)//ÉÏÉýÑØ²¶»ñ
		{
			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 ÉèÖÃÎªÏÂ½µÑØ²¶»ñ				
	    cc2_more=0;
			Dval_in1=TIM_GetCapture2(TIM2);	
			RmtSta2|=0X10;
		}else//ÏÂ½µÑØ²¶»ñ
		{	
  		 Dval1=cc2_more*10000+TIM_GetCapture2(TIM2)-Dval_in1;//¶ÁÈ¡CCR1Ò²¿ÉÒÔÇåCC1IF±êÖ¾Î»
			 TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC4P=0	ÉèÖÃÎªÉÏÉýÑØ²¶»ñ
 			
			 Remote(&RmtSta2,&HW2_MARK,Dval1,&hw_cc2);
		}	
		TIM_ClearFlag(TIM2,TIM_IT_CC2);
	}
else
	if(TIM_GetITStatus(TIM2,TIM_IT_CC3)!=RESET)
	{	  
		if(RDATA2)//ÉÏÉýÑØ²¶»ñ
		{
			TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 ÉèÖÃÎªÏÂ½µÑØ²¶»ñ				
	    cc3_more=0;
			Dval_in2=TIM_GetCapture3(TIM2);
			RmtSta3|=0X10;
		}else//ÏÂ½µÑØ²¶»ñ
		{			
  		 Dval2=cc3_more*10000+TIM_GetCapture3(TIM2)-Dval_in2;//¶ÁÈ¡CCR1Ò²¿ÉÒÔÇåCC1IF±êÖ¾Î»
			 TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC4P=0	ÉèÖÃÎªÉÏÉýÑØ²¶»ñ

				 Remote(&RmtSta3,&HW3_MARK,Dval2,&hw_cc3);
		}	
		TIM_ClearFlag(TIM2,TIM_IT_CC3);
	}	
	else
	if(TIM_GetITStatus(TIM2,TIM_IT_CC4)!=RESET)
	{	  
		if(RDATA3)//ÉÏÉýÑØ²¶»ñ
		{
			TIM_OC4PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 ÉèÖÃÎªÏÂ½µÑØ²¶»ñ				
	    cc4_more=0;
			Dval_in3=TIM_GetCapture4(TIM2);	
			RmtSta4|=0X10;
		}else//ÏÂ½µÑØ²¶»ñ
		{			
  		 Dval3=cc4_more*10000+TIM_GetCapture4(TIM2)-Dval_in3;//¶ÁÈ¡CCR1Ò²¿ÉÒÔÇåCC1IF±êÖ¾Î»
			 TIM_OC4PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC4P=0	ÉèÖÃÎªÉÏÉýÑØ²¶»ñ
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
		if(RDATA4)//ÉÏÉýÑØ²¶»ñ
		{
			TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 ÉèÖÃÎªÏÂ½µÑØ²¶»ñ				
	    cc5_more=0;
			Dval_in4=TIM_GetCapture1(TIM3);	
			RmtSta5|=0X10;
		}else //ÏÂ½µÑØ²¶»ñ
		{			
  		 Dval4=cc5_more*10000+TIM_GetCapture1(TIM3)-Dval_in4;//¶ÁÈ¡CCR1Ò²¿ÉÒÔÇåCC1IF±êÖ¾Î»
			 TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC4P=0	ÉèÖÃÎªÉÏÉýÑØ²¶»ñ
 			
			 Remote(&RmtSta5,&HW5_MARK,Dval4,&hw_cc5);
		}	
		TIM_ClearFlag(TIM3,TIM_IT_CC1);
	}	
	else
 	if(TIM_GetITStatus(TIM3,TIM_IT_CC2)!=RESET)
	{	  
		if(RDATA5)//ÉÏÉýÑØ²¶»ñ
		{
			TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 ÉèÖÃÎªÏÂ½µÑØ²¶»ñ				
	    cc6_more=0;
			Dval_in5=TIM_GetCapture2(TIM3);
			RmtSta6|=0X10;
		}else//ÏÂ½µÑØ²¶»ñ
		{			
  		 Dval5=cc6_more*10000+TIM_GetCapture2(TIM3)-Dval_in5;//¶ÁÈ¡CCR1Ò²¿ÉÒÔÇåCC1IF±êÖ¾Î»
			 TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC4P=0	ÉèÖÃÎªÉÏÉýÑØ²¶»ñ
				
				 Remote(&RmtSta6,&HW6_MARK,Dval5,&hw_cc6);
		}	
		TIM_ClearFlag(TIM3,TIM_IT_CC2);
	}
else
	if(TIM_GetITStatus(TIM3,TIM_IT_CC3)!=RESET)
	{	  
		if(RDATB6)//ÉÏÉýÑØ²¶»ñ
		{
			TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 ÉèÖÃÎªÏÂ½µÑØ²¶»ñ				
	    cc7_more=0;
			Dval_in6=TIM_GetCapture3(TIM3);	
			RmtSta7|=0X10;
		}else //ÏÂ½µÑØ²¶»ñ
		{			
  		 Dval6=cc7_more*10000+TIM_GetCapture3(TIM3)-Dval_in6;//¶ÁÈ¡CCR1Ò²¿ÉÒÔÇåCC1IF±êÖ¾Î»
			 TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC4P=0	ÉèÖÃÎªÉÏÉýÑØ²¶»ñ

			 Remote(&RmtSta7,&HW7_MARK,Dval6,&hw_cc7);
		}	
		TIM_ClearFlag(TIM3,TIM_IT_CC3);
	}else
		if(TIM_GetITStatus(TIM3,TIM_IT_CC4)!=RESET)
	{
		if(RDATB7)//ÉÏÉýÑØ²¶»ñ
		{
			TIM_OC4PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 ÉèÖÃÎªÏÂ½µÑØ²¶»ñ				
	    cc8_more=0;
			Dval_in7=TIM_GetCapture4(TIM3);	
			RmtSta8|=0X10;
		}else //ÏÂ½µÑØ²¶»ñ
		{			
  		 Dval7=cc8_more*10000+TIM_GetCapture4(TIM3)-Dval_in7;//¶ÁÈ¡CCR1Ò²¿ÉÒÔÇåCC1IF±êÖ¾Î»
			 TIM_OC4PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC4P=0	ÉèÖÃÎªÉÏÉýÑØ²¶»ñ

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




