#include "stm32f10x.h"                  // Device header
#include "DFOC.h"

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
		
/***********��������***********/
		GetCommand();
		M0_Set_Velocity(M0_Target());     //�ٶ�ģʽ
		M1_Set_Velocity(M1_Target());     //�ٶ�ģʽ
	}

}
