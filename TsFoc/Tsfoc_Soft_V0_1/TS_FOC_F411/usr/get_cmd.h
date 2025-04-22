#ifndef __GET_CMD_h__
#define __GET_CMD_h__

#include "stdio.h"
#include "main.h"
#include "dma.h"
#include "usart.h"
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#define UART_RECEIVE_IDLE_RX_MAXLEN  256


extern struct UART_RECEIVE_IDLE_DataStructure Uart_Receive_Data;


int8_t Get_Cmd(UART_HandleTypeDef *huart);
void BSP_UART_IDLE_CallBack(UART_HandleTypeDef *huart);




#endif


