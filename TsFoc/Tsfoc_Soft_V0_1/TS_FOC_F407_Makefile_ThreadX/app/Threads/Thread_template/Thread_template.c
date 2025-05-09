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
#include "Thread_template.h"

TX_THREAD Thread_Template;						//实例化线程句柄,线程相关信息
Thread_Template_parama Thread_Template_parama1;	//实例化入口函数的参数






/*******************************************************************************
* Function Name  : Thread_Template_Entry
* Description    : Thread_Template_Entry线程
* Input          : ULONG thread_input
* Output         : None
* Return         : None
* Note			 : 快速创建线程demo
*******************************************************************************/
void Thread_Template_Entry(ULONG thread_input)
{

	while(1)
	{

    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    tx_thread_sleep(200);
	}
}



