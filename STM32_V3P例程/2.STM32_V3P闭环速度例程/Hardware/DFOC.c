#include "stm32f10x.h"
#include <math.h>
#include <stdlib.h>
#include "pwm.h"
#include "AS5600.h"
#include "Delay.h"
#include "Serial.h"
#include "Lowpass_Filter.h"
#include "PID_Control.h"
#include "Inlinecurrent.h"

#define PI         3.14159265359f
#define _3PI_2     4.71238898f
#define _1_SQRT3 	 0.57735026919f
#define _2_SQRT3   1.15470053838f

float Ua=0,Ub=0,Uc=0,Ualpha,Ubeta=0,dc_a=0,dc_b=0,dc_c=0;
float voltage_limit = 8;
float voltage_power_supply = 0;
float zero_electric_Angle=0.0;

int pp = 7,Dir = 1;

//限制幅值
float constrain(float amt, float low, float high)
{
	 return ((amt<low)?(low):((amt)>(high)?(high):(amt)));
}

//将角度归化到0-2PI
float normalizeAngle(float angle)
{
	float a = fmod(angle, 2*PI);
	return ((a>=0) ? a : (a + 2*PI));
}

float electricAngle(void)
{
	return normalizeAngle((GetAngle_NoTrack() * pp * Dir) - zero_electric_Angle);
}

void SetPwm(float Ua, float Ub, float Uc)
{
	float U_a=0.0;
	float U_b=0.0;
	float U_c=0.0;
	
	U_a = constrain(Ua, 0.0f, voltage_limit);
	U_b = constrain(Ub, 0.0f, voltage_limit);
	U_c = constrain(Uc, 0.0f, voltage_limit);
	
	dc_a = constrain(U_a / voltage_power_supply, 0.0f, 1.0f);
	dc_b = constrain(U_b / voltage_power_supply, 0.0f, 1.0f);
	dc_c = constrain(U_c / voltage_power_supply, 0.0f, 1.0f);
	
	PWM_Channel1(dc_a * 4800.0f);  // 频率15k
	PWM_Channel2(dc_b * 4800.0f);
	PWM_Channel3(dc_c * 4800.0f);
	
}

//FOC核心算法，克拉克逆变换/帕克逆变换
void SetPhaseVoltage(float Uq, float Ud, float angle_el)
{
//	angle_el = normalizeAngle(angle_el);
	
	Ualpha = -Uq*sin(angle_el);
	Ubeta = Uq*cos(angle_el);
	
	Ua = Ualpha + voltage_power_supply / 2;
	Ub = (sqrt(3)*Ubeta - Ualpha) / 2 + voltage_power_supply / 2;
	Uc = -(Ualpha + sqrt(3)*Ubeta) / 2 + voltage_power_supply / 2;
	
	SetPwm(Ua,Ub,Uc);
	
}

void Check_Sensor(void)
{
	SetPhaseVoltage(3, 0, _3PI_2);
	delay_ms(3000);
	zero_electric_Angle = electricAngle();
	SetPhaseVoltage(0, 0, _3PI_2);
	delay_ms(500);
}

void FOC_Init(float power)
{
	voltage_power_supply = power;
	PWM_Init();
	CurrSense_Init();
	AS5600_Init();
	
	Check_Sensor();
}

// 串口接收
float Last_C_Num = 0.0;
float GetCommand(void)
{
	float C_Num = 0.0;
	char *Getchar;
		
	if(Serial_GetRxFlag() == 1)
	{
		C_Num = strtof(Serial_RxPacket, &Getchar);				
		Last_C_Num = C_Num;
		
		return C_Num;
	}
	return Last_C_Num;
}

// 速度环
void Set_Velocity(float Target)
{
	float Vel = Lowpassfilter(0.4, GetVelocity());
  float Uq  = PID_Controller(0.4, 5, 0, Dir*(Target - Vel));   
	SetPhaseVoltage(Uq,0,electricAngle());
	/*********实际速度值***********/
	Serial_SendFloatNumber(Vel,3, 2);	
	Serial_SendString("\n");
}



