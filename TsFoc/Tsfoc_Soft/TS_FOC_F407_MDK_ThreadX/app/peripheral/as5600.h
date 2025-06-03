#ifndef _AS5600_H_
#define _AS5600_H_

#include "main.h"
#include "crc.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "user_api.h"

#define AS5600_ADDRESS 0x36 << 1
#define ANGLE_REGISTER 0x0E

// #define AS5600_DEBUG 1

//需要配置的参数：
//1. IIC引脚，端口
/// @brief AS5600传感器对象
typedef struct AS5600_SENSOR
{
    I2C_HandleTypeDef* hi2c;                       //IIC端口
    float Velocity;                                 //电机速度
    float Angle;                                    //电机角度
    uint32_t angle_ts;                              //上次调用获取角度时间戳
    uint32_t vel_ts;                                //上次调用获取速度时间戳
    int Rotations;                                  //电机旋转圈数
}As5600_Sensor_Typedef;


// #define AS5600_DEBUG 1

float Read_AS5600_Angle(As5600_Sensor_Typedef* as5600);
float Get_AS5600_Angle(As5600_Sensor_Typedef* as5600);


#endif /* _AS5600_H_ */
