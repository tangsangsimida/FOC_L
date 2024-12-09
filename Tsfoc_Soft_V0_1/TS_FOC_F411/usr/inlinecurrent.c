#include "inlinecurrent.h"



uint16_t adc_buffer[6]; // 定义数组存储 DMA 传输后的 ADC 数据




float _shunt_resistor = 0.01;
float amp_gain = 50;
float vlots_to_amps=0;

/// @brief 零飘检测
/// @param Sensor 
void DriftOffsets(Inlinecurrent_Typedef *Sensor)
{
	uint16_t detect_rounds = 1000;
	for(int i = 0; i < detect_rounds; i++)
	{
		Sensor->offset_ia += adc_buffer[Sensor->A]*_ADC_CONV;
		Sensor->offset_ib += adc_buffer[Sensor->B]*_ADC_CONV;
	}
	
	Sensor->offset_ia = Sensor->offset_ia / detect_rounds;
	Sensor->offset_ib = Sensor->offset_ib / detect_rounds;

}

/// @brief 初始化电流传感器
/// @param Sensor 
void CurrSense_Init(Inlinecurrent_Typedef *Sensor)
{
	DriftOffsets(Sensor);
	
	_shunt_resistor = 0.01; 
	amp_gain = 50; 
	
	vlots_to_amps = 1.0f / _shunt_resistor / amp_gain;
	
	Sensor->gain_a = vlots_to_amps * -1;
	Sensor->gain_b = vlots_to_amps * -1;
	Sensor->gain_c = vlots_to_amps;
}

/// @brief 更新电流数值
/// @param Sensor 
void Current_Update(Inlinecurrent_Typedef *Sensor)
{
	float tran_vol_a = (float)adc_buffer[Sensor->A]*_ADC_CONV;
	float tran_vol_b = (float)adc_buffer[Sensor->B]*_ADC_CONV;
	
	Sensor->I_a = (tran_vol_a - Sensor->offset_ia)*Sensor->gain_a;
	Sensor->I_b = (tran_vol_b - Sensor->offset_ib)*Sensor->gain_b;
}
