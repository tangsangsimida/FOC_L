#include "as5600.h"




uint16_t Read_AS5600_Angle(void) {
    uint8_t data[2] = {0, 0};
    HAL_I2C_Mem_Read(&hi2c1, AS5600_ADDRESS, ANGLE_REGISTER, I2C_MEMADD_SIZE_8BIT, data, 2, HAL_MAX_DELAY);
    uint16_t angle = (data[0] << 8) | data[1];
    angle = (angle * 360.0) / 4096.0;
#ifdef AS5600_DEBUG
    sprintf(Uart_DeBug_Buffer, "Angle: %d  \r\n", (int)angle);
    HAL_UART_Transmit(Uart_DeBug, (uint8_t*)Uart_DeBug_Buffer, strlen(Uart_DeBug_Buffer), 100);
    memset(Uart_DeBug_Buffer, 0, sizeof(Uart_DeBug_Buffer));
#endif

    return angle;
}




