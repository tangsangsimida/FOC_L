#include "stm32f10x.h"
#include "Delay.h"
#include "Serial.h"
#include "PID_Control.h"

#define  limit            6.3
#define  Output_ramp      10000

//�޷�
float  _constrain(float amt, float low, float high)    
{
	return ((amt<low)?(low):((amt)>(high)?(high):(amt)));
}


float PID_Controller(struct _PID *pid, float error) 
{
    float Ts = 0.0;
    uint32_t Timestamp = 0; // ������������ȷ��ȡʱ����ķ�ʽ
    if (Timestamp < pid->Timestamp_Last) 
        Ts = (float)(pid->Timestamp_Last - Timestamp) / 9 * 1e-6;
    else
        Ts = (0xFFFFFF - Timestamp + pid->Timestamp_Last) / 9 * 1e-6;

    if (Ts <= 0 || Ts > 0.05f) Ts = 0.001;

    float proportion = pid->Kp * error; // P��

    float intergration = pid->Last_intergration + pid->Ki * 0.5f * Ts * error; // I��
    // ���� _constrain �������Զ� intergration ��������

    float differential = pid->Kd * (error - pid->Last_Error) / Ts; // D��

    float Output = proportion + intergration + differential;
    // ���� _constrain �������Զ� Output ��������

    pid->Last_Error = error;
    pid->Last_intergration = intergration;
    pid->Last_Output = Output;
    pid->Timestamp_Last = Timestamp;

    return Output;
}
