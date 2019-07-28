#ifndef __ANO_H
#define __ANO_H
#include "sys.h"

#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)		) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )
	

extern float KP_ANO_Y,KI_ANO_Y,KD_ANO_Y,KP_ANO_Y1,KI_ANO_Y1,KD_ANO_Y1,KP_ANO_Y2,KI_ANO_Y2,KD_ANO_Y2;
extern uint16_t KP_ANO_Y3,KI_ANO_Y3,KD_ANO_Y3,KP_ANO_Y4,KI_ANO_Y4,KD_ANO_Y4,KP_ANO_Y5,KI_ANO_Y5,KD_ANO_Y5;


void mpu6050_send_data(int16_t mouth,int16_t aacx,int16_t aacy,int16_t aacz,int16_t gyrox,int16_t gyroy,int16_t gyroz);//发送用户数据给上位机
void ANO_DT_send_data(int16_t mouth,float data,float data1,float data2,float data3,float data4,float data5);
void usart1_niming_report(u8 fun,u8*data,u8 len);
void ANO_DT_Send_PID(u8 group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d); //发送当前PID数据给上位机
void ANO_DT_Send_RCData(u16 thr,u16 yaw,u16 rol,u16 pit,u16 aux1,u16 aux2,u16 aux3,u16 aux4,u16 aux5,u16 aux6);
void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z,s32 bar);
void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, u8 fly_model, u8 armed);

void Data_Receive_Anl(u8 *data_buf,u8 num);//上位机发送数据解析函数
void Data_Send_Check(u8 mouth,u8 sum);         //返回校验数据给上位机

#endif
