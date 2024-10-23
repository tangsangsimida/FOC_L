#ifndef __DFOC_H
#define __DFOC_H

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "pwm.h"
#include "AS5600.h"
#include "Delay.h"
#include "Serial.h"
#include "Lowpass_Filter.h"
#include "PID_Control.h"
#include "Inlinecurrent.h"

struct Motor_{
	int Mot_num;
	float Ua;
	float Ub;
	float Uc;
	float Ubeta;
	float Ualpha;
	float dc_a;
	float dc_b;
	float dc_c;
};

float constrain(float amt, float low, float high);
void SetPwm(int Mot_num, float Ua, float Ub, float Uc);
float normalizeAngle(float angle);
void SetPhaseVoltage(struct Motor_ *Motor, float Uq, float angle_el);
float cal_Iq_Id(float current_a,float current_b,float angle_el);
void Check_Sensor(void);
void FOC_Init(float power);
float M0_electricAngle(void);
float M1_electricAngle(void);
void GetCommand(void);
float M0_Target(void);
float M1_Target(void);
void M0_Set_Velocity(float Target);
void M0_Set_CurTorque(float Target);
void M1_Set_Velocity(float Target);
void M1_Set_CurTorque(float Target);
void M0_Set_Velocity_Angle(float target);
void M1_Set_Velocity_Angle(float Target);

#endif


