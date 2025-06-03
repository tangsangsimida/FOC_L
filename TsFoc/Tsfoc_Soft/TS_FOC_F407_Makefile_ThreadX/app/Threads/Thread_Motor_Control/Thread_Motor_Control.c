/*
 * @Author: Dennis 488132230@qq.com
 * @Date: 2025-05-03 11:43:38
 * @LastEditors: Dennis 488132230@qq.com
 * @LastEditTime: 2025-05-19 11:41:41
 * @FilePath: \TS_FOC_F407_Makefile_ThreadX\app\Threads\Thread_Motor_Control\Thread_Motor_Control.c
 * @Description: 
 */
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
    uint16_t count;
    tx_thread_sleep(3000);
    aligns_Motor_Zero_Angle(motor1);  //对电机进行零点对齐
    tx_thread_sleep(1000);
    CurrSense_Init(motor1->Inlinecurrent);

    while(1)
    {
      Read_AS5600_Angle(motor1->As5600_Sensor);
      Current_Update(motor1->Inlinecurrent);
      // 位置闭环
      // setTorque(motor1,.133*(thread_cmd_param->Cmd_Param_last-motor1->DIR*Get_AS5600_Angle(motor1->As5600_Sensor))*180.f/PI,_electricalAngle(motor1));   //位置闭环
      //力矩闭环
      // setTorque(motor1, thread_cmd_param->Cmd_Param_last, _electricalAngle(motor1));   
      // 电压速度环
      // Set_Velocity(motor1, thread_cmd_param->Cmd_Param_last);
      // 电压力矩角度环
      // Set_Force_Angle(motor1, thread_cmd_param->Cmd_Param_last);
      // 电压角度速度环
      // Set_Velocity_Angle(motor1, thread_cmd_param->Cmd_Param_last);
      // 电流力矩闭环
      setTorque_Curr(motor1, thread_cmd_param->Cmd_Param_last); 
      if(count++>5)
      {
        count = 0;
        // printf("%f,", Get_AS5600_Angle(motor1->As5600_Sensor));
        // printf("%f,", thread_cmd_param->Cmd_Param_last);
        // printf("%f\r\n", Get_Current(motor1));
        // printf("%f,%f,%f\r\n",motor1->Inlinecurrent->offset_ia,motor1->Inlinecurrent->offset_ib,motor1->Inlinecurrent->offset_ic);
        // printf("%f,%f,%f\r\n",motor1->Inlinecurrent->I_a,motor1->Inlinecurrent->I_b,motor1->Inlinecurrent->I_c);
        printf("%f,%f,%f,%f\r\n",Get_Current(motor1),motor1->Inlinecurrent->I_a,motor1->Inlinecurrent->I_b,motor1->Inlinecurrent->I_c);

      }
      // printf("Motor1 Angle: %f\n", motor1->As5600_Sensor->Angle);
      tx_thread_sleep(1);
    } 
}
