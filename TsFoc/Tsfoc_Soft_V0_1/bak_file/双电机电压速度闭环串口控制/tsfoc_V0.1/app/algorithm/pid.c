#include "pid.h"

/// @brief PID_Vel控制器初始化
/// @param Pid 
/// @param P 
/// @param I 
/// @param D 
/// @param ramp     PID控制器加速度限幅
/// @param limit    PID控制器输出限幅
void Pid_Vel_Init(Pid_Typedef* Pid , float P, float I, float D, float ramp, float limit)
{
    Pid->P = P;
    Pid->I = I;
    Pid->D = D;
    Pid->output_ramp = ramp;                //PID控制器加速度限幅
    Pid->limit = limit;
    Pid->timestamp_prev = HAL_GetTick();
}

/// @brief PID_Ang控制器初始化
/// @param Pid 
/// @param P 
/// @param I 
/// @param D 
/// @param ramp     PID控制器加速度限幅
/// @param limit    PID控制器输出限幅
void Pid_Ang_Init(Pid_Typedef* Pid , float P, float I, float D, float ramp)
{
    Pid->P = P;
    Pid->I = I;
    Pid->D = D;
    Pid->output_ramp = ramp;                //PID控制器加速度限幅
    Pid->timestamp_prev = HAL_GetTick();
}

/// @brief PID_Curr控制器初始化
/// @param Pid 
/// @param P 
/// @param I 
/// @param D 
/// @param ramp     PID控制器加速度限幅
/// @param limit    PID控制器输出限幅
void Pid_Curr_Init(Pid_Typedef* Pid , float P, float I, float D, float ramp, float limit)
{
    Pid->P = P;
    Pid->I = I;
    Pid->D = D;
    Pid->output_ramp = ramp;                //PID控制器加速度限幅
    Pid->limit = limit;                     //PID控制器输出限幅
    Pid->timestamp_prev = HAL_GetTick();
}

/// @brief PID控制器
/// @param Pid PID控制器实例
/// @param error 
/// @return 
float Pid_clac(Pid_Typedef* Pid, float error)
{
    // 计算两次循环中间的间隔时间
    unsigned long timestamp_now = HAL_GetTick();
    float Ts = (timestamp_now - Pid->timestamp_prev) * 1e-3f;
    if(Ts <= 0 || Ts > 0.5f) Ts = 1e-3f;

    // P环
    float proportional = Pid->P * error;
    // Tustin 散点积分（I环）
    float integral = Pid->integral_prev + Pid->I*Ts*0.5f*(error + Pid->error_prev);
    integral = _constrain(integral, -Pid->limit, Pid->limit);
    // D环（微分环节）
    float derivative = Pid->D*(error - Pid->error_prev)/Ts;

    // 将P,I,D三环的计算值加起来
    float output = proportional + integral + derivative;
    output = _constrain(output, -Pid->limit, Pid->limit);

    if(Pid->output_ramp > 0){
        // 对PID的变化速率进行限制
        float output_rate = (output - Pid->output_prev)/Ts;
        if (output_rate > Pid->output_ramp)
            output = Pid->output_prev + Pid->output_ramp*Ts;
        else if (output_rate < -Pid->output_ramp)
            output = Pid->output_prev - Pid->output_ramp*Ts;
    }

    // 保存值（为了下一次循环）
    Pid->integral_prev = integral;
    Pid->output_prev = output;
    Pid->error_prev = error;
    Pid->timestamp_prev = timestamp_now;
    return output;
}















