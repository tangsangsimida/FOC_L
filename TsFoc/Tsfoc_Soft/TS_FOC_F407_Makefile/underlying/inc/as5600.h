#ifndef _AS5600_H_
#define _AS5600_H_

#include "main.h"
#include "crc.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "user_api.h"


// #define AS5600_DEBUG 1

//需要配置的参数：
//1. IIC引脚，端口
/// @brief AS5600传感器对象
typedef struct AS5600_SENSOR
{
    float Velocity_old;                             //上一次的电机速度;
    float Velocity;                                 //当前的电机速度;
    float Angle_old;                                //上一次的角度;  
    float Angle;                                    //当前的角度;                               
    uint32_t angle_ts_old;                          //上次调用获取角度时间戳
    uint32_t angle_ts;                              //当前调用获取角度时间戳
    uint32_t Velocity_ts_old;                            //上次调用获取速度时间戳
    uint32_t Velocity_ts;                                //当前调用获取速度时间戳
    int Rotations_old;                              //上一次电机旋转圈数
    int Rotations;                                  //当前电机旋转圈数
    I2C_HandleTypeDef *i2c_handle;                  //IIC句柄
}As5600_Sensor_Typedef;


// #define AS5600_DEBUG 1
int AS5600_Init(As5600_Sensor_Typedef* as5600, I2C_HandleTypeDef* i2c_handle);
float AS5600_Read_Angle(As5600_Sensor_Typedef* as5600);
float AS5600_Get_Angle(As5600_Sensor_Typedef* as5600);
float AS5600_Get_Velocity(As5600_Sensor_Typedef* as5600);

#endif /* _AS5600_H_ */
