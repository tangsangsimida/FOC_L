#ifndef __AS5600_H__
#define __AS5600_H__

#include "gpio.h"
#include "user_api.h"
#include "math.h"

//需要配置的参数：
//1. IIC引脚，端口
/// @brief AS5600传感器对象
typedef struct AS5600_SENSOR
{
    uint16_t        SCL_PIN;                //传感器SCL引脚
    uint16_t        SDA_PIN;                //传感器SDA引脚
    GPIO_TypeDef *  Port;                   //传感器iic端口
    volatile float angle_prev;                       // 最后一次调用 getSensorAngle() 的输出结果，用于得到完整的圈数和速度
    volatile long angle_prev_ts;                     // 上次调用 getAngle 的时间戳
    volatile float vel_angle_prev;                   // 最后一次调用 getVelocity 时的角度
    volatile long vel_angle_prev_ts;                 // 最后速度计算时间戳
    volatile int32_t full_rotations;                 // 总圈数计数
    volatile int32_t vel_full_rotations;             //用于速度计算的先前完整旋转圈数
    


    //电机有关状态变量
    float Velocity;
}As5600_Sensor_Typedef;


float AS5600_GetAngle_NoTrack(As5600_Sensor_Typedef * sensor);          //获取没有圈数的角度
float AS5600_getAngle(As5600_Sensor_Typedef * AS5600);                  //获取带有圈数的角度
void AS5600_Sensor_update(As5600_Sensor_Typedef * AS5600);              //更新传感器角度
void AS5600_Init(As5600_Sensor_Typedef * AS5600);                       //初始化as5600传感器
float AS5600_getVelocity(As5600_Sensor_Typedef * AS5600);               //获取当前速度












#endif



