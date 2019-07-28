#include "ano_y.h"
#include "usart.h"
#include "math.h"
//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0XA0~0XAF
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数

float KP_ANO_Y,KI_ANO_Y,KD_ANO_Y;
float KP_ANO_Y1,KI_ANO_Y1,KD_ANO_Y1;
float KP_ANO_Y2,KI_ANO_Y2,KD_ANO_Y2;

uint16_t KP_ANO_Y3,KI_ANO_Y3,KD_ANO_Y3;
uint16_t KP_ANO_Y4,KI_ANO_Y4,KD_ANO_Y4;
uint16_t KP_ANO_Y5,KI_ANO_Y5,KD_ANO_Y5;

uint16_t KP_ANO_Y6,KI_ANO_Y6,KD_ANO_Y6;
uint16_t KP_ANO_Y7,KI_ANO_Y7,KD_ANO_Y7;
uint16_t KP_ANO_Y8,KI_ANO_Y8,KD_ANO_Y8;

void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//最多28字节数据 
	send_buf[len+4]=0;	//校验数置零
	send_buf[0]=0xAA;	//帧头
	send_buf[1]=0XAA;	
	send_buf[2]=fun;	//功能字
	send_buf[3]=len;  //数据长度
	for(i=0;i<len;i++)send_buf[4+i]=data[i];			//复制数据
	for(i=0;i<len+4;i++)send_buf[len+4]+=send_buf[i];	//计算校验和	
	for(i=0;i<len+5;i++)usart1_send_char(send_buf[i]);	//发送数据到串口1 
}

   //已修改，可以输入不同的功能字，传输int数据

void mpu6050_send_data(int16_t mouth,int16_t aacx,int16_t aacy,int16_t aacz,int16_t gyrox,int16_t gyroy,int16_t gyroz)
{
	u8 tbuf[12]; 
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF;
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
	usart1_niming_report(mouth,tbuf,12);//自定义帧,0XA1
}

//传输float数据
void ANO_DT_send_data(int16_t mouth,float data,float data1,float data2,float data3,float data4,float data5)
{
	u8 tbuf[24]; 
	tbuf[0]=BYTE3(data);
	tbuf[1]=BYTE2(data);
	tbuf[2]=BYTE1(data);
	tbuf[3]=BYTE0(data);
	tbuf[4]=BYTE3(data1);
	tbuf[5]=BYTE2(data1);
	tbuf[6]=BYTE1(data1);
	tbuf[7]=BYTE0(data1);
	tbuf[8]=BYTE3(data2);
	tbuf[9]=BYTE2(data2);
	tbuf[10]=BYTE1(data2);
	tbuf[11]=BYTE0(data2);
	tbuf[12]=BYTE3(data3);
	tbuf[13]=BYTE2(data3);
	tbuf[14]=BYTE1(data3);
	tbuf[15]=BYTE0(data3);
	tbuf[16]=BYTE3(data4);
	tbuf[17]=BYTE2(data4);
	tbuf[18]=BYTE1(data4);
	tbuf[19]=BYTE0(data4);
	tbuf[20]=BYTE3(data5);
	tbuf[21]=BYTE2(data5);
	tbuf[22]=BYTE1(data5);
	tbuf[23]=BYTE0(data5);
	usart1_niming_report(mouth,tbuf,24);//自定义帧,0XA1
}

void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, u8 fly_model, u8 armed)
{
	u8 _cnt=0;
	vs16 _temp;
	vs32 _temp2 = alt;
	u8 tbuf2[25];
	u8 i;
		u8 sum = 0;
	
	tbuf2[_cnt++]=0xAA;
	tbuf2[_cnt++]=0xAA;
	tbuf2[_cnt++]=0x01;
	tbuf2[_cnt++]=0;
	
	_temp = (int)(angle_rol*100);
	tbuf2[_cnt++]=BYTE1(_temp);
	tbuf2[_cnt++]=BYTE0(_temp);
	_temp = (int)(angle_pit*100);
	tbuf2[_cnt++]=BYTE1(_temp);
	tbuf2[_cnt++]=BYTE0(_temp);
	_temp = (int)(angle_yaw*100);
	tbuf2[_cnt++]=BYTE1(_temp);
	tbuf2[_cnt++]=BYTE0(_temp);
	
	tbuf2[_cnt++]=BYTE3(_temp2);
	tbuf2[_cnt++]=BYTE2(_temp2);
	tbuf2[_cnt++]=BYTE1(_temp2);
	tbuf2[_cnt++]=BYTE0(_temp2);
	
	tbuf2[_cnt++] = fly_model;
	
	tbuf2[_cnt++] = armed;
	
	tbuf2[3] = _cnt-4;
	

	for(i=0;i<_cnt;i++)
		sum += tbuf2[i];
	tbuf2[_cnt++]=sum;
	
	for(i=0;i<_cnt;i++)
	    usart1_send_char(tbuf2[i]);
}

void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z,s32 bar)
{
	u8 _cnt=0;
	vs16 _temp;
	u8 tbuf1[25];
  int i;
   u8 sum = 0;	
	tbuf1[_cnt++]=0xAA;
	tbuf1[_cnt++]=0xAA;
	tbuf1[_cnt++]=0x02;
	tbuf1[_cnt++]=0;
	
	_temp = a_x;
	tbuf1[_cnt++]=BYTE1(_temp);
	tbuf1[_cnt++]=BYTE0(_temp);
	_temp = a_y;
	tbuf1[_cnt++]=BYTE1(_temp);
	tbuf1[_cnt++]=BYTE0(_temp);
	_temp = a_z;	
	tbuf1[_cnt++]=BYTE1(_temp);
	tbuf1[_cnt++]=BYTE0(_temp);
	
	_temp = g_x;	
	tbuf1[_cnt++]=BYTE1(_temp);
	tbuf1[_cnt++]=BYTE0(_temp);
	_temp = g_y;	
	tbuf1[_cnt++]=BYTE1(_temp);
	tbuf1[_cnt++]=BYTE0(_temp);
	_temp = g_z;	
	tbuf1[_cnt++]=BYTE1(_temp);
	tbuf1[_cnt++]=BYTE0(_temp);
	
	_temp = m_x;	
	tbuf1[_cnt++]=BYTE1(_temp);
	tbuf1[_cnt++]=BYTE0(_temp);
	_temp = m_y;	
	tbuf1[_cnt++]=BYTE1(_temp);
	tbuf1[_cnt++]=BYTE0(_temp);
	_temp = m_z;	
	tbuf1[_cnt++]=BYTE1(_temp);
	tbuf1[_cnt++]=BYTE0(_temp);
	
	tbuf1[3] = _cnt-4;
	
	
	for(i=0;i<_cnt;i++)
		sum += tbuf1[i];
	tbuf1[_cnt++] = sum;
	
   for(i=0;i<_cnt;i++)
	    usart1_send_char(tbuf1[i]);
}

//函数名：ANO_DT_Send_PID()
//作用：发送PID数据
//参数:group(1~6)PID1、PID2等等
void ANO_DT_Send_PID(u8 group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d)
{
	u8 i;
	u8 _cnt=0;
	u16 _temp;
	u8  tbuf[23];
	u8 sum = 0;
	
	tbuf[_cnt++]=0xAA;
	tbuf[_cnt++]=0xAA;
	tbuf[_cnt++]=0x10+group-1;
	tbuf[_cnt++]=18;
	
	
	_temp = p1_p * 100;
	tbuf[_cnt++]=BYTE1(_temp);
	tbuf[_cnt++]=BYTE0(_temp);
	_temp = p1_i  * 100;
	tbuf[_cnt++]=BYTE1(_temp);
	tbuf[_cnt++]=BYTE0(_temp);
	_temp = p1_d  * 100;
	tbuf[_cnt++]=BYTE1(_temp);
	tbuf[_cnt++]=BYTE0(_temp);
	_temp = p2_p  * 100;
	tbuf[_cnt++]=BYTE1(_temp);
	tbuf[_cnt++]=BYTE0(_temp);
	_temp = p2_i  * 100;
	tbuf[_cnt++]=BYTE1(_temp);
	tbuf[_cnt++]=BYTE0(_temp);
	_temp = p2_d * 100;
	tbuf[_cnt++]=BYTE1(_temp);
	tbuf[_cnt++]=BYTE0(_temp);
	_temp = p3_p  * 100;
	tbuf[_cnt++]=BYTE1(_temp);
	tbuf[_cnt++]=BYTE0(_temp);
	_temp = p3_i  * 100;
	tbuf[_cnt++]=BYTE1(_temp);
	tbuf[_cnt++]=BYTE0(_temp);
	_temp = p3_d * 100;
	tbuf[_cnt++]=BYTE1(_temp);
	tbuf[_cnt++]=BYTE0(_temp);
	
	tbuf[3] = _cnt-4;
	for(i=0;i<_cnt;i++)
		sum += tbuf[i];
	
	tbuf[_cnt++]=sum;
   for(i=0;i<_cnt;i++)
	    usart1_send_char(tbuf[i]);
}

void ANO_DT_Send_RCData(u16 p_1,u16 i_1,u16 d_1,u16 p_2,u16 i_2,u16 d_2,u16 p_3,u16 i_3,u16 d_3,u16 w)
{
	u8 _cnt=0;
	u8 i;
	u8  tbuff[23];
	u8 sum=0;
	tbuff[_cnt++]=0xAA;
	tbuff[_cnt++]=0xAA;
	tbuff[_cnt++]=0x03;
	tbuff[_cnt++]=0;
	tbuff[_cnt++]=BYTE1(p_1);
	tbuff[_cnt++]=BYTE0(p_1);
	tbuff[_cnt++]=BYTE1(i_1);
	tbuff[_cnt++]=BYTE0(i_1);
	tbuff[_cnt++]=BYTE1(d_1);
	tbuff[_cnt++]=BYTE0(d_1);
	
	tbuff[_cnt++]=BYTE1(p_2);
	tbuff[_cnt++]=BYTE0(p_2);
	tbuff[_cnt++]=BYTE1(i_2);
	tbuff[_cnt++]=BYTE0(i_2);
	tbuff[_cnt++]=BYTE1(d_2);
	tbuff[_cnt++]=BYTE0(d_2);
	
	tbuff[_cnt++]=BYTE1(p_3);
	tbuff[_cnt++]=BYTE0(p_3);
	tbuff[_cnt++]=BYTE1(i_3);
	tbuff[_cnt++]=BYTE0(i_3);
	tbuff[_cnt++]=BYTE1(d_3);
	tbuff[_cnt++]=BYTE0(d_3);
	
	tbuff[_cnt++]=BYTE1(w);
	tbuff[_cnt++]=BYTE0(w);

	tbuff[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += tbuff[i];
	
	tbuff[_cnt++]=sum;
	
	 for(i=0;i<_cnt;i++)
	    usart1_send_char(tbuff[i]);
	
			
}

void Data_Receive_Anl(u8 *data_buf,u8 num)
{
	u8 sum = 0;
	u8 i;
	
	for(i=0;i<(num-1);i++)
		sum += *(data_buf+i);
	if(!(sum==*(data_buf+num-1)))		return;		

	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;		
	
/////////////////////////////////////////////////////////////////////////////////////
	switch(*(data_buf+2))
	{
		case 0X02:								//
		  ANO_DT_Send_PID(1,KP_ANO_Y,KI_ANO_Y,KD_ANO_Y,KP_ANO_Y1,KI_ANO_Y1,KD_ANO_Y1,KP_ANO_Y2,KI_ANO_Y2,KD_ANO_Y2);
//			Data_Send_Check(0X02,sum);
			break;
		case 0x10:							//PID1 外环参数
		  KP_ANO_Y = (float)((*(data_buf+4)<<8)|*(data_buf+5))*0.01f;
			KI_ANO_Y = (float)((*(data_buf+6)<<8)|*(data_buf+7))*0.01f;
			KD_ANO_Y = (float)((*(data_buf+8)<<8)|*(data_buf+9))*0.01f;
		
			KP_ANO_Y1 = (float)((*(data_buf+10)<<8)|*(data_buf+11))*0.01f;
			KI_ANO_Y1 = (float)((*(data_buf+12)<<8)|*(data_buf+13))*0.01f;
		  KD_ANO_Y1 = (float)((*(data_buf+14)<<8)|*(data_buf+15))*0.01f;
		 
		  KP_ANO_Y2 = (float)((*(data_buf+16)<<8)|*(data_buf+17))/100;
			KI_ANO_Y2 = (float)((*(data_buf+18)<<8)|*(data_buf+19))/100;
			KD_ANO_Y2 = (float)((*(data_buf+20)<<8)|*(data_buf+21))/100;
			Data_Send_Check(0x10,sum);
			break;
		case 0X11:							//PID2 外环参数
		  KP_ANO_Y3 = (float)((*(data_buf+4)<<8)|*(data_buf+5));
			KI_ANO_Y3= (float)((*(data_buf+6)<<8)|*(data_buf+7));
			KD_ANO_Y3 = (float)((*(data_buf+8)<<8)|*(data_buf+9));
		
			KP_ANO_Y4 = (float)((*(data_buf+10)<<8)|*(data_buf+11));
			KI_ANO_Y4 = (float)((*(data_buf+12)<<8)|*(data_buf+13));
		  KD_ANO_Y4 = (float)((*(data_buf+14)<<8)|*(data_buf+15));
		 
		  KP_ANO_Y5 = (float)((*(data_buf+16)<<8)|*(data_buf+17));
			KI_ANO_Y5 = (float)((*(data_buf+18)<<8)|*(data_buf+19));
			KD_ANO_Y5 = (float)((*(data_buf+20)<<8)|*(data_buf+21));
			Data_Send_Check(0X11,sum);
			break;
		case 0X12:							//PID3 外环参数
		  KP_ANO_Y6 = (float)((*(data_buf+4)<<8)|*(data_buf+5))*0.01f;
			KI_ANO_Y6 = (float)((*(data_buf+6)<<8)|*(data_buf+7))*0.01f;
			KD_ANO_Y6 = (float)((*(data_buf+8)<<8)|*(data_buf+9))*0.01f;
		
			KP_ANO_Y7 = (float)((*(data_buf+10)<<8)|*(data_buf+11))*0.01f;
			KI_ANO_Y7 = (float)((*(data_buf+12)<<8)|*(data_buf+13))*0.01f;
		  KD_ANO_Y7 = (float)((*(data_buf+14)<<8)|*(data_buf+15))*0.01f;
		 
		  KP_ANO_Y8 = (float)((*(data_buf+16)<<8)|*(data_buf+17))/100;
			KI_ANO_Y8 = (float)((*(data_buf+18)<<8)|*(data_buf+19))/100;
			KD_ANO_Y8 = (float)((*(data_buf+20)<<8)|*(data_buf+21))/100;
			Data_Send_Check(0X12,sum);
			break;
		case 0X13:							//PID4 外环参数
//		  KP_ANO_Y = (float)((*(data_buf+4)<<8)|*(data_buf+5))*0.01f;
//			KI_ANO_Y = (float)((*(data_buf+6)<<8)|*(data_buf+7))*0.001f;
//			KD_ANO_Y = (float)((*(data_buf+8)<<8)|*(data_buf+9))*0.01f;
//		
//			KP_ANO_Y1 = (float)((*(data_buf+10)<<8)|*(data_buf+11))*0.01f;
//			KI_ANO_Y1 = (float)((*(data_buf+12)<<8)|*(data_buf+13))*0.01f;
//		  KD_ANO_Y1 = (float)((*(data_buf+14)<<8)|*(data_buf+15))*0.01f;
//		 
//		  KP_ANO_Y2 = (float)((*(data_buf+16)<<8)|*(data_buf+17))/100;
//			KI_ANO_Y2 = (float)((*(data_buf+18)<<8)|*(data_buf+19))/100;
//			KD_ANO_Y2 = (float)((*(data_buf+20)<<8)|*(data_buf+21))/100;
			Data_Send_Check(0X13,sum);
			break;
		case 0X14:							//PID5 外环参数
//		  KP_ANO_Y = (float)((*(data_buf+4)<<8)|*(data_buf+5))*0.01f;
//			KI_ANO_Y = (float)((*(data_buf+6)<<8)|*(data_buf+7))*0.01f;
//			KD_ANO_Y = (float)((*(data_buf+8)<<8)|*(data_buf+9))*0.01f;
//		
//			KP_ANO_Y1 = (float)((*(data_buf+10)<<8)|*(data_buf+11))*0.01f;
//			KI_ANO_Y1 = (float)((*(data_buf+12)<<8)|*(data_buf+13))*0.01f;
//		  KD_ANO_Y1 = (float)((*(data_buf+14)<<8)|*(data_buf+15))*0.01f;
//		 
//		  KP_ANO_Y2 = (float)((*(data_buf+16)<<8)|*(data_buf+17))/100;
//			KI_ANO_Y2 = (float)((*(data_buf+18)<<8)|*(data_buf+19))/100;
//			KD_ANO_Y2 = (float)((*(data_buf+20)<<8)|*(data_buf+21))/100;
			Data_Send_Check(0X14,sum);
			break;
		case 0X15:							//PID6 外环参数
//		  KP_ANO_Y = (float)((*(data_buf+4)<<8)|*(data_buf+5))*0.01f;
//			KI_ANO_Y = (float)((*(data_buf+6)<<8)|*(data_buf+7))*0.01f;
//			KD_ANO_Y = (float)((*(data_buf+8)<<8)|*(data_buf+9))*0.01f;
//		
//			KP_ANO_Y1 = (float)((*(data_buf+10)<<8)|*(data_buf+11))*0.01f;
//			KI_ANO_Y1 = (float)((*(data_buf+12)<<8)|*(data_buf+13))*0.01f;
//		  KD_ANO_Y1 = (float)((*(data_buf+14)<<8)|*(data_buf+15))*0.01f;
//		 
//		  KP_ANO_Y2 = (float)((*(data_buf+16)<<8)|*(data_buf+17))/100;
//			KI_ANO_Y2 = (float)((*(data_buf+18)<<8)|*(data_buf+19))/100;
//			KD_ANO_Y2 = (float)((*(data_buf+20)<<8)|*(data_buf+21))/100;
			Data_Send_Check(0X15,sum);
			break;
	}
}


void Data_Send_Check(u8 mouth,u8 check)//数据校验
{
	u8 check_sum = 0;
	u8 i;
	u8 tbuf[7]; 
	tbuf[0]=0xAA;
	tbuf[1]=0xAA;
	tbuf[2]=0xEF;
	tbuf[3]=0X07;
	tbuf[4]=0x10;
	tbuf[5]=check;
	
	for(i=0;i<6;i++)
		check_sum += tbuf[i];
	tbuf[6]=check_sum;
  for(i=0;i<7;i++)	
	   usart1_send_char(tbuf[i]);
}


