#ifndef __RED_H
#define __RED_H 
#include "sys.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK MiniSTM32������
//����ң�ؽ������� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
  
#define RDATA 	PAin(0)	 	//�������������
#define RDATA1 	PAin(1)
#define RDATA2	PAin(2)
#define RDATA3 	PAin(3)
#define RDATA4	PAin(6)
#define RDATA5 	PAin(7)
#define RDATB6	PBin(0)
#define RDATB7 	PBin(1)
#define RDATB8 	PBin(6)
//����ң��ʶ����(ID),ÿ��ң�����ĸ�ֵ��������һ��,��Ҳ��һ����.
//����ѡ�õ�ң����ʶ����Ϊ0
//#define REMOTE_ID 0      		   

extern u8 hw_cc1,hw_cc2,hw_cc3,hw_cc4,hw_cc5,hw_cc6,hw_cc7,hw_cc8;
void Remote_Init(void);    	//���⴫��������ͷ���ų�ʼ��
u8 Remote_Scan(void);	    
#endif















