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
#include "Thread_template.h"

TX_THREAD Thread_Template;						//实例化线程句柄,线程相关信息
Thread_Template_parama Thread_Template_parama1;	//实例化入口函数的参数

/*******************************************************************************
* Function Name  : Thread_Template_Entry
* Description    : Thread_Template_Entry线程
* Input          : ULONG thread_input
* Output         : None
* Return         : None
* Note			 : 快速创建线程demo
*******************************************************************************/
void Thread_Template_Entry(ULONG thread_input)
{
  // Thread_Template_parama *parama = (Thread_Template_parama*)thread_input;	//获取入口函数的参数
  can_filter_init();							//初始化CAN滤波器
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  
  uint32_t RGB_flow_color[RGB_FLOW_COLOR_LENGHT + 1] = {0xFF0000FF, 0xFF00FF00, 0xFFFF0000, 0xFF0000FF};
  uint16_t i, j;
  float delta_alpha, delta_red, delta_green, delta_blue;
  float alpha,red,green,blue;
  uint32_t aRGB;

	while(1)
	{

    for(i = 0; i < RGB_FLOW_COLOR_LENGHT; i++)
    {
        alpha = (RGB_flow_color[i] & 0xFF000000) >> 24;
        red = ((RGB_flow_color[i] & 0x00FF0000) >> 16);
        green = ((RGB_flow_color[i] & 0x0000FF00) >> 8);
        blue = ((RGB_flow_color[i] & 0x000000FF) >> 0);

        delta_alpha = (float)((RGB_flow_color[i + 1] & 0xFF000000) >> 24) - (float)((RGB_flow_color[i] & 0xFF000000) >> 24);
        delta_red = (float)((RGB_flow_color[i + 1] & 0x00FF0000) >> 16) - (float)((RGB_flow_color[i] & 0x00FF0000) >> 16);
        delta_green = (float)((RGB_flow_color[i + 1] & 0x0000FF00) >> 8) - (float)((RGB_flow_color[i] & 0x0000FF00) >> 8);
        delta_blue = (float)((RGB_flow_color[i + 1] & 0x000000FF) >> 0) - (float)((RGB_flow_color[i] & 0x000000FF) >> 0);

        delta_alpha /= RGB_FLOW_COLOR_CHANGE_TIME;
        delta_red /= RGB_FLOW_COLOR_CHANGE_TIME;
        delta_green /= RGB_FLOW_COLOR_CHANGE_TIME;
        delta_blue /= RGB_FLOW_COLOR_CHANGE_TIME;
        for(j = 0; j < RGB_FLOW_COLOR_CHANGE_TIME; j++)
        {
            alpha += delta_alpha;
            red += delta_red;
            green += delta_green;
            blue += delta_blue;

            aRGB = ((uint32_t)(alpha)) << 24 | ((uint32_t)(red)) << 16 | ((uint32_t)(green)) << 8 | ((uint32_t)(blue)) << 0;
            aRGB_led_show(aRGB);
            tx_thread_sleep(1);
        }

    }
    
    CAN_cmd_gimbal(1, 2, 3, 4);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    tx_thread_sleep(1);
	}
}




void aRGB_led_show(uint32_t aRGB)
{
    static uint8_t alpha;
    static uint16_t red,green,blue;

    alpha = (aRGB & 0xFF000000) >> 24;
    red = ((aRGB & 0x00FF0000) >> 16) * alpha;
    green = ((aRGB & 0x0000FF00) >> 8) * alpha;
    blue = ((aRGB & 0x000000FF) >> 0) * alpha;

    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, blue);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, green);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, red);
}



