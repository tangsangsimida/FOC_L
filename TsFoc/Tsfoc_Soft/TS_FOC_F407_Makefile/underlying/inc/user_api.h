#ifndef __USER_API_H__
#define __USER_API_H__


#include "stdint.h"
#include "stdlib.h"
#include "stddef.h"
#include "math.h"
#include "i2c.h"
#include "tim.h"


//常用数学关键字定义
#define PI       3.14159265359f
#define _2PI     6.28318530718f
#define _3PI_2 4.71238898038f
#define _1_SQRT3 0.57735026919f
#define _2_SQRT3 1.15470053838f


//高效宏函数
#define _constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))


// AS5600相关定义
#define AS5600_I2C_Handle hi2c1
#define AS5600_ADDRESS 0x36 << 1
#define ANGLE_REGISTER 0x0E


//电流传感器
#define CURRENT_ADC_HANLDE hadc1
#define ADC_DMA_BUFFER_SIZE 6
#define _ADC_VOLTAGE 3.3f            //ADC 电压
#define _ADC_RESOLUTION 4095.0f      //ADC 分辨率
#define _ADC_CONV    0.00080586f            //( (_ADC_VOLTAGE) / (_ADC_RESOLUTION) )    // ADC 计数到电压转换比率求解
#define NOT_SET -12345.0
#define _isset(a) ( (a) != (NOT_SET) )















#endif
