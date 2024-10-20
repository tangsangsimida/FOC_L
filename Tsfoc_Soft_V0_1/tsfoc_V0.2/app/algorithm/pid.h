#ifndef __PID_H__
#define __PID_H__

#include "user_api.h"
#include "main.h"
#include "stm32f1xx_hal.h"





/// @brief PID控制器对象
typedef struct PID
{
    volatile float P; //!< 比例增益(P环增益)
    volatile float I; //!< 积分增益（I环增益）
    volatile float D; //!< 微分增益（D环增益）
    volatile float output_ramp; 
    volatile float limit; 
    volatile float error_prev; //!< 最后的跟踪误差值
    volatile float output_prev;  //!< 最后一个 pid 输出值
    volatile float integral_prev; //!< 最后一个积分分量值
    volatile unsigned long timestamp_prev; //!< 上次执行时间戳
}Pid_Typedef;




void Pid_Vel_Init(Pid_Typedef* Pid , float P, float I, float D, float ramp, float limit);
void Pid_Ang_Init(Pid_Typedef* Pid , float P, float I, float D, float ramp, float limit);
void Pid_Curr_Init(Pid_Typedef* Pid , float P, float I, float D, float ramp, float limit);

float Pid_clac(Pid_Typedef* Pid, float error);




#endif

