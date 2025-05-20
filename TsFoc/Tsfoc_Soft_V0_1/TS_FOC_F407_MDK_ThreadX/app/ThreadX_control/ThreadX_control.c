/**
  ******************************************************************************
  * @file           : ThreadX_control.c
  * @brief          : 
  ******************************************************************************
  * @author			    : Dennis_Re_Yoonjiho
  * @attention		  :
  *	                TsFoc_Soft_V0_1
  * 						    Copyright (c) 2021 Dennis_Re_Yoonjiho
  *						      All rights reserved.
  *					        Licensed under the MIT license.
  *					        See LICENSE file in the project root for full license information.
  ******************************************************************************
  */
#include "ThreadX_control.h"

/*******************************************************************************
* Function Name  : ThreadX_Init
* Description    : ThreadX_Init
* Input          : NONE
* Output         : None
* Return         : None
* Note			 : 
*******************************************************************************/
uint8_t ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_Init */
  (void)byte_pool;
  CHAR *pointer;

//************************************************template线程***********************************************
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       Thread_Template_Size, TX_NO_WAIT) != TX_SUCCESS)    ret = TX_POOL_ERROR;
  if(tx_thread_create(&Thread_Template, "Thread_Template",
                      Thread_Template_Entry, (ULONG)&Thread_Template_parama1, pointer, Thread_Template_Size,
                      Thread_Template_PRIO, Thread_Template_threshold,
                      TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS) ret = TX_THREAD_ERROR;
//************************************************Cmd线程***********************************************
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       Thread_Cmd_Size, TX_NO_WAIT) != TX_SUCCESS)    ret = TX_POOL_ERROR;
  if(tx_thread_create(&Thread_Cmd, "Thread_Cmd",
                      Thread_Cmd_Entry, (ULONG)&Thread_Cmd_parama1, pointer, Thread_Cmd_Size,
                      Thread_Cmd_PRIO, Thread_Cmd_threshold,
                      TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS) ret = TX_THREAD_ERROR;

  return ret;	
}
