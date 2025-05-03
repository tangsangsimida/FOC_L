#include "ts_foc.h"


// 归一化角度到 [0,2PI]
float _normalizeRadian(float angle)
{
  float a = fmod(angle, 2*PI);   //取余运算可以用于归一化，列出特殊值例子算便知
  return a >= 0 ? a : (a + 2*PI);  
  //三目运算符。格式：condition ? expr1 : expr2 
  //其中，condition 是要求值的条件表达式，如果条件成立，则返回 expr1 的值，否则返回 expr2 的值。可以将三目运算符视为 if-else 语句的简化形式。
  //fmod 函数的余数的符号与除数相同。因此，当 angle 的值为负数时，余数的符号将与 _2PI 的符号相反。也就是说，如果 angle 的值小于 0 且 _2PI 的值为正数，则 fmod(angle, _2PI) 的余数将为负数。
  //例如，当 angle 的值为 -PI/2，_2PI 的值为 2PI 时，fmod(angle, _2PI) 将返回一个负数。在这种情况下，可以通过将负数的余数加上 _2PI 来将角度归一化到 [0, 2PI] 的范围内，以确保角度的值始终为正数。
}



// 归一化角度到 [0, 360] 度
float _normalizeAngle(float angle) {
    float a = fmod(angle, 360.0);   // 取余运算可以用于归一化
    return a >= 0 ? a : (a + 360.0);  
}



/// @brief 获取电角度
/// @return float
float _electricalAngle(Motor* motor)
{
    // return  _normalizeAngle((float)(motor->DIR *  motor->PP) * motor->As5600_Sensor->Angle-motor->zero_electric_angle);
    return  _normalizeRadian((float)(motor->DIR *  motor->PP) * motor->As5600_Sensor->Angle-motor->zero_electric_angle);
}


/// @brief 设置PWM到控制器输出
/// @param motor 电机对象
/// @param Ua 
/// @param Ub 
/// @param Uc 
void setPwm(Motor* motor, float Ua, float Ub, float Uc) 
{
  // 限制上限
  Ua = _constrain(Ua, 0.0f, motor->voltage_power_supply);
  Ub = _constrain(Ub, 0.0f, motor->voltage_power_supply);
  Uc = _constrain(Uc, 0.0f, motor->voltage_power_supply);
  // 计算占空比
  // 限制占空比从0到1
  float dc_a = _constrain(Ua / motor->voltage_power_supply, 0.0f , 1.0f );
  float dc_b = _constrain(Ub / motor->voltage_power_supply, 0.0f , 1.0f );
  float dc_c = _constrain(Uc / motor->voltage_power_supply, 0.0f , 1.0f );

  //写入PWM
  motor->Motor_Set_Compare1(dc_a*3300);
  motor->Motor_Set_Compare2(dc_b*3300);
  motor->Motor_Set_Compare3(dc_c*3300);
}

/// @brief 设置力矩
/// @param Uq 需求力矩
/// @param angle_el 
void setTorque(Motor* motor,float Uq,float angle_el)
{

  Uq=_constrain(Uq,-(motor->voltage_power_supply)/2,(motor->voltage_power_supply)/2);

  angle_el = _normalizeAngle(angle_el);
  // 帕克逆变换
  motor->Ualpha =  -Uq*sin(angle_el);
  motor->Ubeta =   Uq*cos(angle_el);

  // 克拉克逆变换
  motor->Ua = motor->Ualpha + motor->voltage_power_supply/2;
  motor->Ub = (sqrt(3)*motor->Ubeta-motor->Ualpha)/2 + motor->voltage_power_supply/2;
  motor->Uc = (-motor->Ualpha-sqrt(3)*motor->Ubeta)/2 + motor->voltage_power_supply/2;
  setPwm(motor,motor->Ua,motor->Ub,motor->Uc);
}















/// @brief 设置电机1通道1pwm输出占空比
/// @param compare 占空比值
void __attribute__((weak)) Motor1_Set_Compare1(uint32_t compare) 
{
    __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, compare);
}

/// @brief 设置电机1通道2pwm输出占空比
/// @param compare 占空比值
void __attribute__((weak)) Motor1_Set_Compare2(uint32_t compare) 
{
    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, compare);
}

/// @brief 设置电机1通道3pwm输出占空比
/// @param compare 占空比值
void __attribute__((weak)) Motor1_Set_Compare3(uint32_t compare) 
{
    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, compare);
}

/// @brief 设置电机2通道1pwm输出占空比
/// @param compare 占空比值
void __attribute__((weak)) Motor2_Set_Compare1(uint32_t compare) 
{
    __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, compare);
}

/// @brief 设置电机2通道2pwm输出占空比
/// @param compare 占空比值
void __attribute__((weak)) Motor2_Set_Compare2(uint32_t compare) 
{
    __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, compare);
}

/// @brief 设置电机2通道3pwm输出占空比
/// @param compare 占空比值
void __attribute__((weak)) Motor2_Set_Compare3(uint32_t compare) 
{
    __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, compare);
}


