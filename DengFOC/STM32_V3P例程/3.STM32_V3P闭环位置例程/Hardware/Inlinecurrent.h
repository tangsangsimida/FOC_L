#ifndef  __INLINECURRENT_H
#define  __INLINECURRENT_H


struct CurrentDetect{
	float I_a;
	float I_b;
	float U_a;
	float U_b;
};
//extern struct CurrentDetect Current;

void DriftOffsets(void);
void CurrSense_Init(void);
struct CurrentDetect GetPhaseCurrent(void);


#endif

