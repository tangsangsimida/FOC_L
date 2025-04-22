#include "lowpass_filter.h"


/// @brief 初始化低通滤波器
/// @param flt 
/// @param Tf 
void Lowpass_Filter_Init(Lowpass_Filter_Typedef* flt, float Tf)
{
    flt->Tf = Tf;
}





/// @brief 低通滤波器
/// @param flt 
/// @param x 
/// @return 
float Lowpass_Filter_calc(Lowpass_Filter_Typedef* flt, float x)
{
    unsigned long timestamp = HAL_GetTick();
    float dt = (timestamp - flt->timestamp_prev)*1e-3f;

    if (dt < 0.0f ) dt = 1e-3f;
    else if(dt > 0.3f) {
        flt->y_prev = x;
        flt->timestamp_prev = timestamp;
        return x;
    }

    float alpha = flt->Tf/(flt->Tf + dt);
    float y = alpha*flt->y_prev + (1.0f - alpha)*x;
    flt->y_prev = y;
    flt->timestamp_prev = timestamp;
    return y;
}







