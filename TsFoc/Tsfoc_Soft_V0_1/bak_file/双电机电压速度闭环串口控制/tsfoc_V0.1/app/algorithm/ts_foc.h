#ifndef __TS_FOC_H__
#define __TS_FOC_H__

#include "as5600.h"
#include "tim.h"
#include "stdio.h"
#include "main.h"
#include "get_cmd.h"
#include "pid.h"
#include "lowpass_filter.h"


//需要配置的参数：
//1. PWM定时器与通道
//2. 电机：电源电压，极对数，方向，设置PWM函数接口，传感器对象各个参数
/// @brief 电机对象
typedef struct MOTOR
{
    uint8_t Motor_ID;
    int PP,DIR;                                 //极对数,旋转方向
    float voltage_power_supply;                 //电源电压
    float zero_electric_angle;                  //零角度
    float Ualpha,Ubeta,Ua,Ub,Uc;

    As5600_Sensor_Typedef * As5600_Sensor;      //角度传感器对象

    Pid_Typedef * Pid_Vel;                      //速度PID
    Pid_Typedef * Pid_Ang;                      //角度PID
    Pid_Typedef * Pid_Curr;                     //电流PID

    Lowpass_Filter_Typedef * Lowpass_Filter;    //低通滤波器
    //void (*Motor_Set_Compare1)(uint32_t compare) = Motor1_Set_Compare1;   //赋值
    //Motor_Set_Compare1(1000);                                             //使用
    void (*Motor_Set_Compare1)(uint32_t compare);      //设置电机1通道PWM接口   
    void (*Motor_Set_Compare2)(uint32_t compare);      //设置电机1通道PWM接口   
    void (*Motor_Set_Compare3)(uint32_t compare);      //设置电机1通道PWM接口   


}Motor;


float _normalizeAngle(float angle);
float _electricalAngle(Motor* motor);
void aligns_Motor_Zero_Angle(Motor* motor);
void setTorque(Motor* motor,float Uq,float angle_el);
float Motor_Get_Velocity(Motor* motor);












//PWM设置接口
void Motor1_Set_Compare1(uint32_t compare);
void Motor1_Set_Compare2(uint32_t compare);
void Motor1_Set_Compare3(uint32_t compare);

void Motor2_Set_Compare1(uint32_t compare);
void Motor2_Set_Compare2(uint32_t compare);
void Motor2_Set_Compare3(uint32_t compare);

#endif

