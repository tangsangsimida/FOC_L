#ifndef __USER_API_H__
#define __USER_API_H__

//常用数学关键字定义
#define PI       3.14159265359f
#define _2PI     6.28318530718f
#define _3PI_2 4.71238898038f




//高效宏函数
#define _constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))





// AS5600相关定义
#define AS5600_ADDRESS        0x6C //加上读写位（1位1/0）
#define AS5600_RAW_ANGLE_H    0x0C
#define AS5600_RAW_ANGLE_L    0x0D








#endif

