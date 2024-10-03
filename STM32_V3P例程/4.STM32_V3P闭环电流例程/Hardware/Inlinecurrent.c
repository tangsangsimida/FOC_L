#include "stm32f10x.h"
#include "Inlinecurrent.h"
#include "AD.h"
#include "Serial.h"

#define _ADC_CONV    0.00080586f

float _shunt_resistor = 0;
float amp_gain = 0;
float vlots_to_amps = 0;
float gain_a, gain_b, gain_c;
float offset_ia = 0.0, offset_ib = 0.0;
float current_a, current_b, current_c;
	
// ¡„∆ÆºÏ≤‚
void DriftOffsets()
{
	uint16_t detect_rounds = 1000;
	for(int i = 0; i < detect_rounds; i++)
	{
		offset_ia += Samp_volts[0]*_ADC_CONV;
		offset_ib += Samp_volts[1]*_ADC_CONV;
	}
	offset_ia = offset_ia / detect_rounds;
	offset_ib = offset_ib / detect_rounds;

}

void CurrSense_Init()
{
	AD_Init();
	DriftOffsets();
	
	_shunt_resistor = 0.01; 
	amp_gain = 50; 
	
	vlots_to_amps = 1.0f / _shunt_resistor / amp_gain;
	
	gain_a = vlots_to_amps * -1;
	gain_b = vlots_to_amps * -1;
	gain_c = vlots_to_amps;
	
}

struct CurrentDetect GetPhaseCurrent()
{
	struct CurrentDetect current;
	float tran_vol_a = (float)Samp_volts[0]*_ADC_CONV;
	float tran_vol_b = (float)Samp_volts[1]*_ADC_CONV;
	
	current.I_a = (tran_vol_a - offset_ia)*gain_a;
	current.I_b = (tran_vol_b - offset_ib)*gain_b;
	
	current.U_a = (tran_vol_a - offset_ia) ;
	current.U_b = (tran_vol_b - offset_ib) ;


	return current;
}


