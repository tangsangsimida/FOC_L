#ifndef __THREAD_CMD__
#define __THREAD_CMD__


#include "tx_api.h"
#include "usart.h"
#include "stdio.h"
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "dma.h"
#define Thread_Cmd_Size (2048)			//线程堆栈大小
#define Thread_Cmd_PRIO (17)			//线程优先级
#define Thread_Cmd_threshold (17)		//线程抢占阈值

#define UART_RECEIVE_IDLE_RX_MAXLEN  256

#define DEBUG_UART huart1

typedef struct							//线程执行函数的参数结构体
{
	char Cmd_Name_last[32];				//命令名称
	double Cmd_Param_last;				//命令参数
	//将参数定义成结构体,可以通过结构体一次性传入多个参数
}Thread_Cmd_parama;

void Thread_Cmd_Entry(ULONG thread_input);		//线程入口函数
extern TX_THREAD Thread_Cmd;						//实例化线程句柄,线程相关信息
extern Thread_Cmd_parama Thread_Cmd_parama1;	//实例化入口函数的参数

// 其他函数声明……



extern struct UART_RECEIVE_IDLE_DataStructure Uart_Receive_Data;



int8_t Get_Cmd(UART_HandleTypeDef *, ULONG);
void BSP_UART_IDLE_CallBack(UART_HandleTypeDef *);





#endif



