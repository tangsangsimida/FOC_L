/**
  ******************************************************************************
  * @file           : Thread_template.c
  * @brief          : 降低代码耦合
  ******************************************************************************
  * @author			  : Dennis_Re_Yoonjiho
  * @attention		:
  *	
  * 						   	Copyright (c) 2021 Dennis_Re_Yoonjiho
  ******************************************************************************
  */


#include "Thread_Motor_Control.h"


TX_THREAD Thread_Motor_Control;						//实例化线程句柄,线程相关信息
Thread_Motor_Control_parama Thread_Motor_Control_parama1={
   .Thread_param1_handle = &Thread_Cmd_parama1,
};	//实例化入口函数的参数




/*******************************************************************************
* Function Name  : Thread_Motor_Control_Entry
* Description    : Thread_Motor_Control_Entry线程
* Input          : ULONG thread_input
* Output         : None
* Return         : None
* Note			 : 快速创建线程demo
*******************************************************************************/
void Thread_Motor_Control_Entry(ULONG thread_input)
{
    Thread_Motor_Control_parama * thread_param = (Thread_Motor_Control_parama *)thread_input;
    Motor* motor1 = (Motor*)thread_param->Motor1_handle;
    Thread_Cmd_parama * thread_cmd_param = (Thread_Cmd_parama *)thread_param->Thread_param1_handle;
    // 计算电机零角度
    aligns_Motor_Zero_Angle(motor1);
    int count = 0;

    while(1)
    {
      Read_AS5600_Angle(motor1->As5600_Sensor);
      // Current_Update(motor1->Inlinecurrent);
      // 位置闭环
      // setTorque(motor1,.133*(thread_cmd_param->Cmd_Param_last-motor1->DIR*Get_AS5600_Angle(motor1->As5600_Sensor))*180.f/PI,_electricalAngle(motor1));   //位置闭环
      //力矩闭环
      // setTorque(motor1, thread_cmd_param->Cmd_Param_last, _electricalAngle(motor1));   
      // 电压速度环
      Set_Velocity(motor1, thread_cmd_param->Cmd_Param_last);
      // 电压力矩角度环
      // Set_Force_Angle(motor1, thread_cmd_param->Cmd_Param_last);
      // 电压角度速度环
      // Set_Velocity_Angle(motor1, thread_cmd_param->Cmd_Param_last);
      if(count++>10)
      {
        count = 0;
        printf("%f,", Get_AS5600_Angle(motor1->As5600_Sensor));
        printf("%f,", motor1->As5600_Sensor->Angle);
        printf("%f,", motor1->As5600_Sensor->Velocity);
        printf("%f\r\n",thread_cmd_param->Cmd_Param_last);
        // printf("%f,%f,%f\r\n",motor1->Inlinecurrent->I_a, motor1->Inlinecurrent->I_b, motor1->Inlinecurrent->I_a+motor1->Inlinecurrent->I_b);
      }
      // printf("Motor1 Angle: %f\n", motor1->As5600_Sensor->Angle);
      tx_thread_sleep(1);
    } 
}
