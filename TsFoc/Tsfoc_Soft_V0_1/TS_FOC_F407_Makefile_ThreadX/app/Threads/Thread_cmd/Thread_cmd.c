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
#include "Thread_cmd.h"

TX_THREAD Thread_Cmd;						//实例化线程句柄,线程相关信息
Thread_Cmd_parama Thread_Cmd_parama1;	//实例化入口函数的参数



struct UART_RECEIVE_IDLE_DataStructure
{
	volatile uint8_t rxlen; 
	volatile uint8_t rxok; 
	uint8_t rxbuf[UART_RECEIVE_IDLE_RX_MAXLEN]; 
} Uart_Receive_Data ={ 0, 1 };


int8_t Get_Cmd(UART_HandleTypeDef *huart, ULONG THREAD_CMD)
{
    Thread_Cmd_parama * Thread_Cmd = (Thread_Cmd_parama *)THREAD_CMD;
	int8_t nret = 0;
	if (Uart_Receive_Data.rxok == 0)
	{
		nret = -1;
	}
	else
	{
		if (nret == 0)
		{
            //数据接收成功，清洗数据
            char command[UART_RECEIVE_IDLE_RX_MAXLEN] = {0};
            memcpy(command, Uart_Receive_Data.rxbuf, Uart_Receive_Data.rxlen);
            char *angle_str = NULL;
			char *torque_str = NULL;
            char *angle_num = NULL;
            char *torque_num = NULL;

            angle_str = strstr(command, "set_angle:"); // 找到 "set_angle:" 的位置
            if (angle_str != NULL) {
                strcpy(Thread_Cmd->Cmd_Name_last, "set_angle:"); // 保存命令名称
                angle_num = angle_str + strlen("set_angle:"); // 指向角度值的起始位置
                double angle = atof(angle_num); // 将字符串转换为浮点数
                Thread_Cmd->Cmd_Param_last = angle; // 保存角度值
            }
			torque_str = strstr(command, "set_torque:"); // 找到 "set_torque:" 的位置
            if (torque_str != NULL) {
                strcpy(Thread_Cmd->Cmd_Name_last, "set_torque:"); // 保存命令名称
                torque_num = torque_str + strlen("set_torque:"); // 指向力矩值的起始位置
                double torque = atof(torque_num); // 将字符串转换为浮点数
                Thread_Cmd->Cmd_Param_last = torque; // 保存力矩值
            }
            //这个地方不要直接打印参数,建议到线程中去处理
            memset(command, 0, UART_RECEIVE_IDLE_RX_MAXLEN); // 清空接收缓存
		}
		Uart_Receive_Data.rxok = 0;
		__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
		memset(Uart_Receive_Data.rxbuf, 0, UART_RECEIVE_IDLE_RX_MAXLEN);
		HAL_UART_Receive_DMA(huart, Uart_Receive_Data.rxbuf, UART_RECEIVE_IDLE_RX_MAXLEN);
	}
    // printf("Get_Cmd_nret: %d\r\n",nret);
	return nret;
}


void BSP_UART_IDLE_CallBack(UART_HandleTypeDef *huart)
{
	if ((__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET))
	{
		HAL_UART_DMAStop(huart);
		__HAL_UART_CLEAR_IDLEFLAG(huart);
		Uart_Receive_Data.rxlen = UART_RECEIVE_IDLE_RX_MAXLEN - __HAL_DMA_GET_COUNTER(huart->hdmarx);
		Uart_Receive_Data.rxok = 1;
	}
}

/*******************************************************************************
* Function Name  : Thread_Cmd_Entry
* Description    : Thread_Cmd_Entry线程
* Input          : ULONG thread_input
* Output         : None
* Return         : None
* Note			 : 快速创建线程demo
*******************************************************************************/
void Thread_Cmd_Entry(ULONG thread_input)
{
    Thread_Cmd_parama * thread_param = (Thread_Cmd_parama *)thread_input;
	while(1)
	{
        if (Get_Cmd(&DEBUG_UART, thread_input) == 0)   //接收到数据
        {
            //处理数据
            printf("Cmd_Name_last: %s\r\n", thread_param->Cmd_Name_last);
            printf("Cmd_Param_last: %lf\r\n", thread_param->Cmd_Param_last);
        }
        else
        {
            //处理接收错误
        }
        
    tx_thread_sleep(2);
	}
}

