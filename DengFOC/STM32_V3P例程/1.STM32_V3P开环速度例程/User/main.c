#include "stm32f10x.h"                  // Device header
#include <math.h>
#include <stdio.h>
#include "pwm.h"
#include "Delay.h"
#include "DFOC.h"
#include "Serial.h"


float Target = 0.0;
int main(void)
{
  Serial_Init(115200);
	FOC_Init(12.6);

	Systick_CountMode();  //初始化嘀嗒定时器
	
	while(1)
	{

/***********串口输入***********/
		//指令格式：T+target，建议一点一点往上加
		Target = GetCommand();
    velocityopenloop(60);
		
	}
	
}
