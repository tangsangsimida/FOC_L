#include "user_main.h"

#include "stdio.h"

#include "user_api.h"
#include "led.h"
#include "ts_foc.h"
#include "as5600.h"
#include "inlinecurrent.h"
#include "pid.h"
#include "lowpass_filter.h"

void user_main(void)
{
    LED_TOGGLE();

    FOC_Process(motor1); 


    // Get_Cmd_Process();

    // setTorque_Curr(motor1, 0.05f); // 电流力矩环 单环PID 5,200,0.000,10000,10000
    set_Force_Angle_Curr(motor1, 0); // 力位环 anglePID:0.5,0,0.003,10000,0.1 CURRPID:1.25,50,0.000,10000,10000
    // set_Velocity_Curr(motor1, 30); // 电流速度环 单环PID 5,200,0.000,10000,10000
    Print_Process();



    return;
}

/// @brief FOC处理函数
/// @param motor 电机句柄
void FOC_Process(Motor* motor)
{
    AS5600_Read_Angle(motor->As5600_Sensor);
    Current_Update(motor->Inlinecurrent);
}



void Print_Process(void)
{
    printf("%f,%f\r\n",AS5600_Get_Angle(motor1->As5600_Sensor)*motor1->DIR,AS5600_Get_Velocity(motor1->As5600_Sensor)*motor1->DIR);
    // printf("voltage_power_supply:%f\r\n",motor1->voltage_power_supply);
    // printf("%f,%f,%f\r\n",motor1->Inlinecurrent->I_a,motor1->Inlinecurrent->I_b,motor1->Inlinecurrent->I_c);
    // printf("%f\r\n",Get_Current(motor1));
}

