#ifndef __LOWPASS_FILTER_H__
#define __LOWPASS_FILTER_H__

#include "user_api.h"



/// @brief 低通滤波器对象
typedef struct LOWPASS_FILTER
{
    float Tf;                       //!< 低通滤波时间常数
    unsigned long timestamp_prev;   //!< 最后执行时间戳
    float y_prev;                   //!< 上一个循环中的过滤后的值
}Lowpass_Filter_Typedef;




void Lowpass_Filter_Init(Lowpass_Filter_Typedef* flt, float Tf);
float Lowpass_Filter_calc(Lowpass_Filter_Typedef* flt, float x);











#endif
