#include "stm32f10x.h"                  // Device header
#include <math.h>
#include <stdio.h>
#include "pwm.h"
#include "Delay.h"
#include "DFOC.h"
#include "AS5600.h"
#include "Serial.h"

#define    PI    3.14159265359f

float Target = 0.0;
float angle = 0.0;
int main(void)
{
  Serial_Init(115200);
	FOC_Init(12.6);

	Systick_CountMode();  //��ʼ����શ�ʱ��
	
	while(1)
	{
/***********�Ƕȴ�ӡ***********/
//		Serial_SendFloatNumber(GetAngle(),4, 2);	
//		Serial_SendString("\n");
		
		
/***********��������***********/
		Target = GetCommand();
		Set_Velocity(Target);     //�ٶ�ģʽ


		
		
	}
	
}
