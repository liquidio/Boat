#ifndef __RED_H
#define __RED_H 
#include "sys.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK MiniSTM32开发板
//红外遥控解码驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/12
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
  
#define RDATA 	PAin(0)	 	//红外数据输入脚
#define RDATA1 	PAin(1)
#define RDATA2	PAin(2)
#define RDATA3 	PAin(3)
#define RDATA4	PAin(6)
#define RDATA5 	PAin(7)
#define RDATB6	PBin(0)
#define RDATB7 	PBin(1)
#define RDATB8 	PBin(6)
//红外遥控识别码(ID),每款遥控器的该值基本都不一样,但也有一样的.
//我们选用的遥控器识别码为0
//#define REMOTE_ID 0      		   

extern u8 hw_cc1,hw_cc2,hw_cc3,hw_cc4,hw_cc5,hw_cc6,hw_cc7,hw_cc8;
void Remote_Init(void);    	//红外传感器接收头引脚初始化
u8 Remote_Scan(void);	    
#endif















