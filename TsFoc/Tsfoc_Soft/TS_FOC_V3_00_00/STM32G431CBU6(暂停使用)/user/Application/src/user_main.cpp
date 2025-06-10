#include "user_main.hpp"

#include "main.h"


int user_main()
{
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); 
    HAL_Delay(1000);
    return 0;
}