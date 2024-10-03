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

	Systick_CountMode();  //初始化嘀嗒定时器
	
	while(1)
	{
/***********角度打印***********/
//		Serial_SendFloatNumber(GetAngle(),4, 2);	
//		Serial_SendString("\n");
		
		
/***********串口输入***********/
		Target = GetCommand();
		Set_Velocity(Target);     //速度模式


		
		
	}
	
}
