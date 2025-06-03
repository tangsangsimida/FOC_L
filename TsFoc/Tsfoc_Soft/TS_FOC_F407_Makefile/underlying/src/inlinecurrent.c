#include "inlinecurrent.h"



#define DEBUG_CURRENT

#ifdef DEBUG_CURRENT
#include "stdio.h"
#endif


uint16_t adc_dma_buffer[ADC_DMA_BUFFER_SIZE]; // 定义数组存储 DMA 传输后的 ADC 数据




float _shunt_resistor = 0.01;
float amp_gain = 50;
float vlots_to_amps=0;

// /// @brief 零飘检测
// /// @param Sensor 
// void DriftOffsets(Inlinecurrent_Typedef *Sensor)
// {
// 	uint16_t detect_rounds = 1000;
// 	for(int i = 0; i < detect_rounds; i++)
// 	{
// 		Sensor->offset_ia += adc_dma_buffer[Sensor->A]*_ADC_CONV;
// 		Sensor->offset_ib += adc_dma_buffer[Sensor->B]*_ADC_CONV;
// 	}
// 	Sensor->offset_ia = Sensor->offset_ia / detect_rounds;
// 	Sensor->offset_ib = Sensor->offset_ib / detect_rounds;
// }
/// @brief 零飘检测
/// @param Sensor 
void DriftOffsets(Inlinecurrent_Typedef *Sensor)
{
    uint16_t detect_rounds = 1000;
    // 初始化偏移量为0
    Sensor->offset_ia = 0;
    Sensor->offset_ib = 0;
    Sensor->offset_ic = 0; // 如果适用
    for(int i = 0; i < detect_rounds; i++)
    {
        Sensor->offset_ia += adc_dma_buffer[Sensor->A]*_ADC_CONV;
        Sensor->offset_ib += adc_dma_buffer[Sensor->B]*_ADC_CONV;
        Sensor->offset_ic += adc_dma_buffer[Sensor->C]*_ADC_CONV; // 如果适用
    }
    Sensor->offset_ia /= detect_rounds;
    Sensor->offset_ib /= detect_rounds;
    Sensor->offset_ic /= detect_rounds; // 如果适用
    printf("offset_ia:%f\r\n",Sensor->offset_ia);
    printf("offset_ib:%f\r\n",Sensor->offset_ib);
    printf("offset_ic:%f\r\n",Sensor->offset_ic); // 如果适用
}
// /// @brief 初始化电流传感器
// /// @param Sensor 
// void CurrSense_Init(Inlinecurrent_Typedef *Sensor)
// {
// 	DriftOffsets(Sensor);
// 	_shunt_resistor = 0.01; 
// 	amp_gain = 50; 
// 	vlots_to_amps = 1.0f / _shunt_resistor / amp_gain;
// 	Sensor->gain_a = vlots_to_amps * -1;
// 	Sensor->gain_b = vlots_to_amps * -1;
// 	Sensor->gain_c = vlots_to_amps;
// }
/// @brief 初始化电流传感器
/// @param Sensor 
int CurrSense_Init(Inlinecurrent_Typedef *Sensor,int A,int B,int C,ADC_HandleTypeDef *hadc)
{
    HAL_ADC_Start_DMA(hadc, (uint32_t*)adc_dma_buffer, ADC_DMA_BUFFER_SIZE); // 启动 DMA 传输

    Sensor->A = A;
    Sensor->B = B;
    Sensor->C = C; // 如果适用
    DriftOffsets(Sensor);
    
    float _shunt_resistor = 0.01f; 
    float amp_gain = 50.0f; 
    
    float vlots_to_amps = 1.0f / _shunt_resistor / amp_gain;
    
    // 所有的增益系数都应该相同，除非有特殊原因
    Sensor->gain_a = vlots_to_amps*-1;
    Sensor->gain_b = vlots_to_amps*-1;
    Sensor->gain_c = vlots_to_amps;
    printf("CurrSense_Init success\r\n");
    return 0;
}
/// @brief 更新电流数值
/// @param Sensor 
void Current_Update(Inlinecurrent_Typedef *Sensor)
{
	float tran_vol_a = (float)adc_dma_buffer[Sensor->A]*_ADC_CONV;
	float tran_vol_b = (float)adc_dma_buffer[Sensor->B]*_ADC_CONV;
    float tran_vol_c = (float)adc_dma_buffer[Sensor->C]*_ADC_CONV; // 如果适用
// #ifdef DEBUG_CURRENT
//     printf("%f,",tran_vol_a);
// 	printf("%f,",tran_vol_b);
// 	printf("%f\r\n",tran_vol_c);
// #endif
	Sensor->I_a = (tran_vol_a - Sensor->offset_ia)*Sensor->gain_a;
	Sensor->I_b = (tran_vol_b - Sensor->offset_ib)*Sensor->gain_b;
    Sensor->I_c = (tran_vol_c - Sensor->offset_ic)*Sensor->gain_c; // 如果适用
}
