#include "get_cmd.h"

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
            //数据接收成功
            char *command = (char *)Uart_Receive_Data.rxbuf; // 指向 cmd_data.data 的起始地址
            char *angle_str;
			char *torque_str;

            angle_str = strstr(command, "set_angle:"); // 找到 "set_angle:" 的位置
            if (angle_str != NULL) {
                angle_str += strlen("set_angle:"); // 指向角度值的起始位置
                printf("%s\r\n",angle_str);
                float angle = atof(angle_str); // 将字符串转换为浮点数
                // printf("设置角度为:%f\r\n",angle);
                target_angle = angle;
            }
			torque_str = strstr(command, "set_torque:"); // 找到 "set_torque:" 的位置
            if (torque_str != NULL) {
                torque_str += strlen("set_torque:"); // 指向力矩值的起始位置
                printf("%s\r\n",torque_str);
                float torque = atof(torque_str); // 将字符串转换为浮点数
                // printf("设置力矩为:%f\r\n",torque);
                target_torque = torque;
            }


		}
		Uart_Receive_Data.rxok = 0;
		__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
		memset(Uart_Receive_Data.rxbuf, 0, UART_RECEIVE_IDLE_RX_MAXLEN);
		HAL_UART_Receive_DMA(huart, Uart_Receive_Data.rxbuf, UART_RECEIVE_IDLE_RX_MAXLEN);
	}
	return nret;
}

/// @brief 空闲中断回调函数
/// @param huart 
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

