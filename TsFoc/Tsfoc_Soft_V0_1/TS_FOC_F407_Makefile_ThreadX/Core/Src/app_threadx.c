/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
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

//************************************************Motor_Ctrl线程***********************************************
if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                      Thread_Motor_Control_Size, TX_NO_WAIT) != TX_SUCCESS)    ret = TX_POOL_ERROR;
if(tx_thread_create(&Thread_Motor_Control, "Thread_Motor_Control",
                      Thread_Motor_Control_Entry, (ULONG)&Thread_Motor_Control_parama1, pointer, Thread_Motor_Control_Size,
                      Thread_Motor_Control_PRIO, Thread_Motor_Control_threshold,
                      TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS) ret = TX_THREAD_ERROR;

//************************************************oled线程***********************************************
if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                      Thread_oled_Size, TX_NO_WAIT) != TX_SUCCESS)    ret = TX_POOL_ERROR;
if(tx_thread_create(&Thread_oled, "Thread_oled",
                      Thread_oled_Entry, (ULONG)&Thread_oled_parama1, pointer, Thread_oled_Size,
                      Thread_oled_PRIO, Thread_oled_threshold,
                      TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS) ret = TX_THREAD_ERROR;

//************************************************led线程***********************************************
if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                      Thread_led_Size, TX_NO_WAIT) != TX_SUCCESS)    ret = TX_POOL_ERROR;
if(tx_thread_create(&Thread_led, "Thread_led",
                      Thread_led_Entry, (ULONG)&Thread_led_parama1, pointer, Thread_led_Size,
                      Thread_led_PRIO, Thread_led_threshold,
                      TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS) ret = TX_THREAD_ERROR;








  /* USER CODE END App_ThreadX_Init */

  return ret;
}

/**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
