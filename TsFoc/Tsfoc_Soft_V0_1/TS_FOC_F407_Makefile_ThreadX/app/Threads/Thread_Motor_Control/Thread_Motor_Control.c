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
Thread_Motor_Control_parama Thread_Motor_Control_parama1;	//实例化入口函数的参数




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
    OLED_Init();  //OLED初始化
    OLED_ShowStr(0,0,"OLED-TEXT",1);
    char temp[100];
    while(1)
    {

      sprintf(temp,"%ld",HAL_GetTick());
      OLED_ShowStr(0,10,temp,2);
      
      tx_thread_sleep(1);
    }
}
