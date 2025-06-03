#ifndef _TS_FOC_H_
#define _TS_FOC_H_


#include "user_api.h"




typedef struct AS5600_SENSOR As5600_Sensor_Typedef;     // 前向声明
typedef struct INLINECURRENT Inlinecurrent_Typedef;     // 前向声明
typedef struct PID Pid_Typedef;                         // 前向声明
typedef struct LOWPASS_FILTER Lowpass_Filter_Typedef;   // 前向声明

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
    //void (*Motor_Set_Compare1)(uint32_t compare) = Motor1_Set_Compare1;   //赋值
    //Motor_Set_Compare1(1000);                                             //使用
    void (*Motor_Set_Compare1)(uint32_t compare);      //设置电机1通道PWM接口   
    void (*Motor_Set_Compare2)(uint32_t compare);      //设置电机1通道PWM接口   
    void (*Motor_Set_Compare3)(uint32_t compare);      //设置电机1通道PWM接口   


}Motor;

extern Motor * motor1;


typedef enum {
    INIT_OK = 0,
    INIT_ERR_MALLOC_MOTOR = -1,
    INIT_ERR_MALLOC_AS5600 = -2,
    INIT_ERR_MALLOC_INLINE_CURRENT = -3,
    INIT_ERR_MALLOC_FILTER_VEL = -4,
    INIT_ERR_MALLOC_FILTER_CURR = -5,
    INIT_ERR_MALLOC_PID_VEL = -6,
    INIT_ERR_MALLOC_PID_ANG = -7,
    INIT_ERR_MALLOC_PID_CURR = -8,
}Init_Result;


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
Init_Result Ts_Foc_Init(Motor** motor, int ID, int PP, int DIR, float voltage_power_supply);


//PWM设置接口
void Motor1_Set_Compare1(uint32_t compare);
void Motor1_Set_Compare2(uint32_t compare);
void Motor1_Set_Compare3(uint32_t compare);

void Motor2_Set_Compare1(uint32_t compare);
void Motor2_Set_Compare2(uint32_t compare);
void Motor2_Set_Compare3(uint32_t compare);









#endif /* _TS_FOC_H_ */

