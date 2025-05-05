#ifndef THREAD_MOTOR_CONTROL_H_
#define THREAD_MOTOR_CONTROL_H_


#include "tx_api.h"
#include "stdint.h"
#include "ts_foc.h"







#define Thread_Motor_Control_Size (4096)			//线程堆栈大小
#define Thread_Motor_Control_PRIO (15)			//线程优先级
#define Thread_Motor_Control_threshold (15)		//线程抢占阈值

typedef struct							//线程执行函数的参数结构体
{
	char Motor_Control_Name_last[32];				//命令名称
	double Motor_Control_Param_last;				//命令参数
    ULONG Motor1_handle;							    //电机句柄
	//将参数定义成结构体,可以通过结构体一次性传入多个参数
}Thread_Motor_Control_parama;

void Thread_Motor_Control_Entry(ULONG thread_input);		//线程入口函数
extern TX_THREAD Thread_Motor_Control;						//实例化线程句柄,线程相关信息
extern Thread_Motor_Control_parama Thread_Motor_Control_parama1;	//实例化入口函数的参数



#endif /* THREAD_MOTOR_CONTROL_H_ */
