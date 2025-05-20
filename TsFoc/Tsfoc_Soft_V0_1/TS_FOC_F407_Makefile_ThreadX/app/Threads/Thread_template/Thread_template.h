#ifndef __THREAD_TEMPLATE__
#define __THREAD_TEMPLATE__

#include "tx_api.h"
#include "stdio.h"
#include "main.h"



// 线程变量
#define Thread_Template_Size (512)			//线程堆栈大小
#define Thread_Template_PRIO (31)			//线程优先级
#define Thread_Template_threshold (31)		//线程抢占阈值
typedef struct							//线程执行函数的参数结构体
{
	uint8_t parama1;
	//讲参数定义成结构体,可以通过结构体一次性传入多个参数
}Thread_Template_parama;
void Thread_Template_Entry(ULONG thread_input);		//线程入口函数
extern TX_THREAD Thread_Template;						//实例化线程句柄,线程相关信息
extern Thread_Template_parama Thread_Template_parama1;	//实例化入口函数的参数


#endif

