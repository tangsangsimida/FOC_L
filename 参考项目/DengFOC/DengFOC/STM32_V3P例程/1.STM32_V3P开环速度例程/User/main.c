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

	Systick_CountMode();  //��ʼ����શ�ʱ��
	
	while(1)
	{

/***********��������***********/
		//ָ���ʽ��T+target������һ��һ�����ϼ�
		Target = GetCommand();
    velocityopenloop(60);
		
	}
	
}
