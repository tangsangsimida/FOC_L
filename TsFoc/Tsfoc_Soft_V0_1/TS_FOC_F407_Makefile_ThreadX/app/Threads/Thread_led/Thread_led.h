#ifndef __THREAD_LED__
#define __THREAD_LED__


#include "tx_api.h"
#include "usart.h"
#include "stdio.h"
#include "stdint.h"
#include "main.h"
#include "tim.h"



// 线程变量
#define Thread_led_Size (512)			//线程堆栈大小
#define Thread_led_PRIO (19)			//线程优先级
#define Thread_led_threshold (19)		//线程抢占阈值
typedef struct							//线程执行函数的参数结构体
{
	uint8_t parama1;
	//讲参数定义成结构体,可以通过结构体一次性传入多个参数
}Thread_led_parama;
void Thread_led_Entry(ULONG thread_input);		//线程入口函数
extern TX_THREAD Thread_led;						//实例化线程句柄,线程相关信息
extern Thread_led_parama Thread_led_parama1;	//实例化入口函数的参数


// 其他变量……


// RGB线程 变量

#define RGB_FLOW_COLOR_CHANGE_TIME  500
#define RGB_FLOW_COLOR_LENGHT   3
void aRGB_led_show(uint32_t);


#endif

