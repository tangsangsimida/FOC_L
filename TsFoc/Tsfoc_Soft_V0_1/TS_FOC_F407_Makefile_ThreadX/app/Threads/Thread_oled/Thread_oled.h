#ifndef THREAD_OLED_H_
#define THREAD_OLED_H_


#include "tx_api.h"
#include "stdint.h"
#include "OLED_IIC_Config.h"
#include "OLED_Function.h"
#include "OLED_Front.h"


#define Thread_oled_Size (2048)			//线程堆栈大小
#define Thread_oled_PRIO (20)			//线程优先级
#define Thread_oled_threshold (20)		//线程抢占阈值

typedef struct							//线程执行函数的参数结构体
{
	uint8_t data;						//数据指针
}Thread_oled_parama;

void Thread_oled_Entry(ULONG thread_input);		//线程入口函数
extern TX_THREAD Thread_oled;						//实例化线程句柄,线程相关信息
extern Thread_oled_parama Thread_oled_parama1;	//实例化入口函数的参数




#endif /* THREAD_OLED_H_ */

