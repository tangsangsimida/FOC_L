#include "as5600.h"




uint16_t Read_AS5600_Angle(As5600_Sensor_Typedef* as5600) 
{
    uint8_t data[2] = {0, 0};
    HAL_I2C_Mem_Read(&hi2c1, AS5600_ADDRESS, ANGLE_REGISTER, I2C_MEMADD_SIZE_8BIT, data, 2, HAL_MAX_DELAY);
    float angle = (data[0] << 8) | data[1];
    as5600->Angle = (((angle * _2PI) / 4096.0));
#ifdef AS5600_DEBUG
    sprintf(Uart_DeBug_Buffer, "Angle: %d  \r\n", (int)as5600->Angle);
    HAL_UART_Transmit(Uart_DeBug, (uint8_t*)Uart_DeBug_Buffer, strlen(Uart_DeBug_Buffer), 100);
    memset(Uart_DeBug_Buffer, 0, sizeof(Uart_DeBug_Buffer));
#endif
    as5600->angle_ts = HAL_GetTick();
    return as5600->Angle;
}




