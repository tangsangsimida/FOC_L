#ifndef _AS5600_H_
#define _AS5600_H_

#include "main.h"
#include "crc.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

#define AS5600_ADDRESS 0x36 << 1
#define ANGLE_REGISTER 0x0C


//需要配置的参数：
//1. IIC引脚，端口
/// @brief AS5600传感器对象
typedef struct AS5600_SENSOR
{
    // uint16_t        SCL_PIN;                //传感器SCL引脚
    // uint16_t        SDA_PIN;                //传感器SDA引脚
    // GPIO_TypeDef *  Port;                   //传感器iic端口
    // 后续再继续修改必要的参数变量
    volatile float angle_prev;                       // 最后一次调用 getSensorAngle() 的输出结果，用于得到完整的圈数和速度
    volatile long angle_prev_ts;                     // 上次调用 getAngle 的时间戳
    volatile float vel_angle_prev;                   // 最后一次调用 getVelocity 时的角度
    volatile long vel_angle_prev_ts;                 // 最后速度计算时间戳
    volatile int32_t full_rotations;                 // 总圈数计数
    volatile int32_t vel_full_rotations;             //用于速度计算的先前完整旋转圈数
    


    //电机有关状态变量
    float Velocity;
}As5600_Sensor_Typedef;


// #define AS5600_DEBUG 1

uint16_t Read_AS5600_Angle(void);



#endif /* _AS5600_H_ */
