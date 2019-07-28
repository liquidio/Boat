#ifndef _PID_H
#define _PID_H
#include"sys.h"

typedef struct PID_Speed
{
	int Sp;
	int Ss;
	int Sev[3];
	float Speed_out;
	float Speed_tie;
	int OutputMAX;
	float Output;
	float Skp;
	float Ski;
	float Skd;
}PID_Speed;

extern PID_Speed PID_Direction;

void PID_Init(void);
void PID_SpeedHuan(PID_Speed*Pid);

#endif

