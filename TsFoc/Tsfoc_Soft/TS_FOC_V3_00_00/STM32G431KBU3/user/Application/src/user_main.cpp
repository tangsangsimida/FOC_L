#include "user_main.hpp"

#include "main.h"


int user_main()
{
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin); 
    HAL_Delay(100);
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    HAL_Delay(100); 
    HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
    HAL_Delay(100);
    return 0;
}