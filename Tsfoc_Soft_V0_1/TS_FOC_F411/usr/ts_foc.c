#include "ts_foc.h"















/// @brief 设置电机1通道1pwm输出占空比
/// @param compare 占空比值
void __attribute__((weak)) Motor1_Set_Compare1(uint32_t compare) 
{
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, compare);
}

/// @brief 设置电机1通道2pwm输出占空比
/// @param compare 占空比值
void __attribute__((weak)) Motor1_Set_Compare2(uint32_t compare) 
{
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, compare);
}

/// @brief 设置电机1通道3pwm输出占空比
/// @param compare 占空比值
void __attribute__((weak)) Motor1_Set_Compare3(uint32_t compare) 
{
    __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, compare);
}

/// @brief 设置电机2通道1pwm输出占空比
/// @param compare 占空比值
void __attribute__((weak)) Motor2_Set_Compare1(uint32_t compare) 
{
    __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, compare);
}

/// @brief 设置电机2通道2pwm输出占空比
/// @param compare 占空比值
void __attribute__((weak)) Motor2_Set_Compare2(uint32_t compare) 
{
    __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, compare);
}

/// @brief 设置电机2通道3pwm输出占空比
/// @param compare 占空比值
void __attribute__((weak)) Motor2_Set_Compare3(uint32_t compare) 
{
    __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, compare);
}


