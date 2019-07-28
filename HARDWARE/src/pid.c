#include"pid.h"


PID_Speed PID_Direction;


PID_Speed PID_SpeedInit=
{
	.Sp=0,
	.Ss=0,
	.Sev[0]=0,
	.Sev[1]=0,
	.Sev[2]=0,
	.Speed_out=0,
	.Speed_tie=0,
	.OutputMAX=3,
	.Output=0,
	.Skp=1,//50
	.Ski=0.1,//1
	.Skd=1,//10
};


void PID_Init()
{
	PID_Direction=PID_SpeedInit;
}


void PID_SpeedHuan(PID_Speed*Pid)
{
	Pid->Sev[0]=-(Pid->Ss -Pid->Sp);
	
	Pid->Speed_tie =Pid->Skp *Pid->Sev[0]+Pid->Ski*Pid->Sev[2]
	                +Pid->Skd*(Pid->Sev[0]-Pid->Sev[1]);
	
	Pid->Speed_out=Pid->Speed_tie;
	
	Pid->Sev[2]=+Pid->Sev[0];
	Pid->Sev[1]=Pid->Sev[0];
	
	if(Pid->Sev[2]>30)  Pid->Sev[2]=30;
	if(Pid->Sev[2]<-30) Pid->Sev[2]=-30;
	if(Pid->Speed_out > Pid->OutputMAX)  Pid->Speed_out=Pid->OutputMAX;
	if(Pid->Speed_out < -Pid->OutputMAX)  Pid->Speed_out=-Pid->OutputMAX;
	Pid->Output=Pid->Speed_out;
}
