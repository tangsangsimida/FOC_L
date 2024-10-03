#include "stm32f10x.h"                  // Device header
#include "DFOC.h"

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
		
/***********串口输入***********/
		GetCommand();
		M0_Set_Angle(M0_Target());          //角度模式
		M1_Set_Angle(M1_Target());          //角度模式
	}

}
