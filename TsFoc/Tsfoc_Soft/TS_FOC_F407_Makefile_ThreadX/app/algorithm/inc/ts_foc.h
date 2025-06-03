/*
 * @Author: Dennis 488132230@qq.com
 * @Date: 2025-05-02 18:16:21
 * @LastEditors: Dennis 488132230@qq.com
 * @LastEditTime: 2025-05-19 12:16:28
 * @FilePath: \TS_FOC_F407_Makefile_ThreadX\app\algorithm\inc\ts_foc.h
 * @Description: 
 */
#ifndef _TS_FOC_H_
#define _TS_FOC_H_

#include "stdio.h"
#include "tx_api.h"

#include "user_api.h"
#include "as5600.h"
#include "main.h"
#include "pid.h"
#include "lowpass_filter.h"
#include "inlinecurrent.h"
#include "tim.h"




// /// @brief 电机状态
// /// @note 电机状态结构体
// /// 后续优化，上位机读取状态可直接从这个对象的示例中获取
// typedef struct 
// {
// }Motor_Staute_Typedef;

/// @brief 电机参数自动识别状态枚举
typedef enum {
    MOTOR_IDENTIFY_IDLE = 0,
    MOTOR_IDENTIFY_RESISTANCE,
    MOTOR_IDENTIFY_INDUCTANCE,
    MOTOR_IDENTIFY_POLE_PAIRS,
    MOTOR_IDENTIFY_ZERO_ANGLE,
    MOTOR_IDENTIFY_COMPLETE,
    MOTOR_IDENTIFY_ERROR
} MotorIdentifyState;

/// @brief 电机参数识别诊断结构体
typedef struct {
    uint32_t error_code;
    float resistance;
    float inductance;
    int pole_pairs;
    float zero_angle;
    float max_current;
    float max_voltage;
    uint32_t identify_time;
    MotorIdentifyState state;
} MotorIdentifyDiagnostic;


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

    Lowpass_Filter_Typedef * Lowpass_Filter_Vel;        //低通滤波器速度环
    Lowpass_Filter_Typedef * Lowpass_Filter_Curr;       //电流环

    Inlinecurrent_Typedef * Inlinecurrent;      //电流传感器
    MotorIdentifyDiagnostic* diagnostic;        //电机参数
    //void (*Motor_Set_Compare1)(uint32_t compare) = Motor1_Set_Compare1;   //赋值
    //Motor_Set_Compare1(1000);                                             //使用
    void (*Motor_Set_Compare1)(uint32_t compare);      //设置电机1通道PWM接口   
    void (*Motor_Set_Compare2)(uint32_t compare);      //设置电机1通道PWM接口   
    void (*Motor_Set_Compare3)(uint32_t compare);      //设置电机1通道PWM接口   


}Motor;

float _normalizeRadian(float angle);
float _normalizeAngle(float angle);
float _electricalAngle(Motor* motor);
void setPwm(Motor* motor, float Ua, float Ub, float Uc);
void setTorque(Motor* motor,float Uq,float angle_el);
int aligns_Motor_Zero_Angle(Motor* motor);
float cal_Iq_Id(Inlinecurrent_Typedef *Sensor, float angle_el);
float Get_Current(Motor* motor);
void Set_Velocity_Angle(Motor* motor, float target);
void Set_Velocity(Motor* motor, float target);
void Set_Force_Angle(Motor* motor, float target);
void setTorque_Curr(Motor* motor, float Target);
void set_Force_Angle_Curr(Motor* motor, float Target);
void set_Velocity_Curr(Motor* motor, float Target);
void set_Velocity_Angle_Curr(Motor* motor, float Target);
float Get_filter_Velocity(Motor* motor);



//PWM设置接口
void Motor1_Set_Compare1(uint32_t compare);
void Motor1_Set_Compare2(uint32_t compare);
void Motor1_Set_Compare3(uint32_t compare);

void Motor2_Set_Compare1(uint32_t compare);
void Motor2_Set_Compare2(uint32_t compare);
void Motor2_Set_Compare3(uint32_t compare);









#endif /* _TS_FOC_H_ */

