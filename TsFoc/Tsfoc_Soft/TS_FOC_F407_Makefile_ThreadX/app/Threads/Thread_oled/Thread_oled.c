#include "Thread_oled.h"


TX_THREAD Thread_oled;						//实例化线程句柄,线程相关信息
Thread_oled_parama Thread_oled_parama1;	//实例化入口函数的参数


/*******************************************************************************
* Function Name  : Thread_oled_Entry
* Description    : Thread_oled_Entry线程
* Input          : ULONG thread_input
* Output         : None
* Return         : None
* Note			 : 快速创建线程demo
*******************************************************************************/
void Thread_oled_Entry(ULONG thread_input)
{
    Thread_oled_parama * thread_param = (Thread_oled_parama *)thread_input;

    OLED_Init();  //OLED初始化
    tx_thread_sleep(500);
    OLED_ShowStr(0,0,"OLED-TEXT",1);
    unsigned char temp[100];
    while(1)
    {
      sprintf((char *)temp,"%ld",HAL_GetTick());
      OLED_ShowStr(0,10,temp,2);
      
      tx_thread_sleep(1);
    }
}







