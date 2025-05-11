#include "as5600.h"



// 读取AS5600角度(0-360°)
// 输入：as5600 结构体指针
// 返回：角度值 单位：弧度
// 说明：读取AS5600角度，{并更新}as5600结构体中的角度值和旋转数值
float Read_AS5600_Angle(As5600_Sensor_Typedef* as5600) 
{
    uint8_t data[2] = {0, 0};
    HAL_I2C_Mem_Read(as5600->i2c_handle, AS5600_ADDRESS, ANGLE_REGISTER, I2C_MEMADD_SIZE_8BIT, data, 2, HAL_MAX_DELAY);
    float angle = (data[0] << 8) | data[1];
    angle = (((angle * _2PI) / 4096.0f));
    float d_angle = angle - as5600->Angle;
    // 圈数检测
    if(fabs(d_angle) > (0.8f*_2PI) ) as5600->Rotations += ( d_angle > 0 ) ? -1 : 1; 
    as5600->Angle = angle;
    as5600->angle_ts = HAL_GetTick();
    // printf("angle:%f\n", as5600->Angle);
    return as5600->Angle;
}
// 获取AS5600角度(带旋转数值)
// 输入：as5600 结构体指针
// 返回：角度值 单位：弧度
// 说明：获取AS5600角度(带旋转数值)，不更新as5600结构体中的角度值和旋转数值
float Get_AS5600_Angle(As5600_Sensor_Typedef* as5600) 
{
    return as5600->Angle + as5600->Rotations*_2PI;
}




