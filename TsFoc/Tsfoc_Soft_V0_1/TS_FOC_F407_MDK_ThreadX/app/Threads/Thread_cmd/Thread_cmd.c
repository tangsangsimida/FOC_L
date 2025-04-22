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


int8_t Get_Cmd(UART_HandleTypeDef *huart)
{

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
            char *command = (char *)Uart_Receive_Data.rxbuf; // 指向 cmd_data.data 的起始地址
            char *angle_str;
			      char *torque_str;

            angle_str = strstr(command, "set_angle:"); // 找到 "set_angle:" 的位置
            if (angle_str != NULL) {
              strcpy((char *)Thread_Cmd_parama1.Cmd_Name,(char *)"set_angle");
                angle_str += strlen("set_angle:"); // 指向角度值的起始位置
                // printf("%s\r\n",angle_str);
                float angle = atof(angle_str); // 将字符串转换为浮点数
                // printf("设置角度为:%f\r\n",angle);
                Thread_Cmd_parama1.Cmd_Param = angle;
            }
			      torque_str = strstr(command, "set_torque:"); // 找到 "set_torque:" 的位置
            if (torque_str != NULL) {
                strcpy((char *)Thread_Cmd_parama1.Cmd_Name,(char *)"set_torque");
                torque_str += strlen("set_torque:"); // 指向力矩值的起始位置
                // printf("%s\r\n",torque_str);
                float torque = atof(torque_str); // 将字符串转换为浮点数
                // printf("设置力矩为:%f\r\n",torque);
                Thread_Cmd_parama1.Cmd_Param = torque;
            }


		}
		Uart_Receive_Data.rxok = 0;
		__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
		memset(Uart_Receive_Data.rxbuf, 0, UART_RECEIVE_IDLE_RX_MAXLEN);
		HAL_UART_Receive_DMA(huart, Uart_Receive_Data.rxbuf, UART_RECEIVE_IDLE_RX_MAXLEN);
	}
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
	thread_input = (uint8_t )thread_input;

	while(1)
	{
    if (Get_Cmd(&DEBUG_UART) == 0)   //接收到数据
    {
        //处理数据
        printf("Thread_Cmd_Entry_cmd_name: %s\r\n",Thread_Cmd_parama1.Cmd_Name);
        printf("Thread_Cmd_Entry_cmd_param: %f\r\n",Thread_Cmd_parama1.Cmd_Param);
    }
    else
    {
        //处理接收错误
    }
    
        tx_thread_sleep(2);

	}
}

