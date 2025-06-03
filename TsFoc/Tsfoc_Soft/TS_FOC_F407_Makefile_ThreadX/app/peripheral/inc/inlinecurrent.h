#ifndef __INLINECURRENT_H__
#define __INLINECURRENT_H__


#include "user_api.h"


/// @brief 电流传感器对象
typedef struct INLINECURRENT
{
    volatile int A; // 电流信号A
    volatile int B; // 电流信号B
	volatile int C; // 电流信号C

	volatile float I_a; // 电流A
	volatile float I_b; // 电流B
	volatile float I_c; // 电流C
	
	volatile float offset_ia; // 电流A 偏移量
	volatile float offset_ib; // 电流B 偏移量
	volatile float offset_ic; // 电流C 偏移量
	volatile float gain_a, gain_b, gain_c; // 电流增益
	volatile float I_Q; // 电流Q

}Inlinecurrent_Typedef;




extern uint16_t adc_buffer[]; // 存储 DMA 传输后的 ADC 数据





void CurrSense_Init(Inlinecurrent_Typedef *Sensor);
void Current_Update(Inlinecurrent_Typedef *Sensor);
void DriftOffsets(Inlinecurrent_Typedef *Sensor);


#endif
