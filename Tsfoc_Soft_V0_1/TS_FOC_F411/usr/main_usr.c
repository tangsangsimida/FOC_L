#include "main_usr.h"



#define AS5600_ADDRESS 0x36 << 1
#define ANGLE_REGISTER 0x0C

float Read_AS5600_Angle(void) {
    uint8_t data[2] = {0, 0};
    HAL_I2C_Mem_Read(&hi2c1, AS5600_ADDRESS, ANGLE_REGISTER, I2C_MEMADD_SIZE_8BIT, data, 2, HAL_MAX_DELAY);
    uint16_t angle = (data[0] << 8) | data[1];

    return (angle * 360.0) / 4096.0;
}

