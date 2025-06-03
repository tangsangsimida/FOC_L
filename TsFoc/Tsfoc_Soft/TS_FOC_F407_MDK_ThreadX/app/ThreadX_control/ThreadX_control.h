/**
  ******************************************************************************
  * @file           : ThreadX_control.h
  * @brief          : 
  ******************************************************************************
  * @author			: Dennis_Re_Yoonjiho
  * @attention		:
  *	
  *
  ******************************************************************************
  */
#ifndef __THREADX_CONTROL_H
#define __THREADX_CONTROL_H


#include "Thread_api.h"
#include "main.h"

/************************************************************************************************
 *
 *	
 *									
 *	
 *
 *************************************************************************************************/
#define TX_APP_MEM_POOL_SIZE                     80480
static UCHAR tx_byte_pool_buffer[TX_APP_MEM_POOL_SIZE];
static TX_BYTE_POOL tx_app_byte_pool;
typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed long long int64_t;




uint8_t ThreadX_Init(VOID *memory_ptr);

#endif

