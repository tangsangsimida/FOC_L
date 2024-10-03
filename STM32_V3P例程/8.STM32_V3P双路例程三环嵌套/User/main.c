#include "stm32f10x.h"                  // Device header
#include "DFOC.h"

#define    PI    3.14159265359f

int main(void)
{
	Serial_Init(115200);
	FOC_Init(12.6);

	Systick_CountMode();  //��ʼ����શ�ʱ��
	
	while(1)
	{
		
/***********��������***********/
		GetCommand();
		M0_Set_Velocity_Angle(M0_Target());
		M1_Set_Velocity_Angle(M1_Target());

	}

}
