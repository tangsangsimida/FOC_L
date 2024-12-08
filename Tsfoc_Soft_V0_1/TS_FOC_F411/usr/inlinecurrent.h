#ifndef __INLINECURRENT_H__
#define __INLINECURRENT_H__


#include "user_api.h"


/// @brief 电流传感器对象
typedef struct INLINECURRENT
{
    volatile int A;
    volatile int B;

	volatile float I_a;
	volatile float I_b;
	volatile float offset_ia;
	volatile float offset_ib;
	volatile float gain_a, gain_b, gain_c;
	volatile float I_Q;

}Inlinecurrent_Typedef;

extern uint16_t Samp_volts[4]; // 存储 DMA 传输后的 ADC 数据





void CurrSense_Init(Inlinecurrent_Typedef *Sensor);
void Current_Update(Inlinecurrent_Typedef *Sensor);
void DriftOffsets(Inlinecurrent_Typedef *Sensor);


#endif
