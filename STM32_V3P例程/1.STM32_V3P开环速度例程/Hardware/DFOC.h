#ifndef __DFOC_H
#define __DFOC_H

float constrain(float amt, float low, float high);
void SetPwm(float Ua, float Ub, float Uc);
float normalizeAngle(float angle);
void SetPhaseVoltage(float Uq, float Ud, float angle_el);
void Check_Sensor(void);
void FOC_Init(float power);
float electricAngle(void);
float GetCommand(void);
float velocityopenloop(float target);

#endif


