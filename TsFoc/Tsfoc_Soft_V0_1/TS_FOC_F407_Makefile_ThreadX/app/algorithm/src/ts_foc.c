#include "ts_foc.h"

// 归一化角度到 [0,2PI]
float _normalizeRadian(float angle)
{
  float a = fmod(angle, 2*PI);   //取余运算可以用于归一化，列出特殊值例子算便知
  return a >= 0 ? a : (a + 2*PI);  
  //三目运算符。格式：condition ? expr1 : expr2 
  //其中，condition 是要求值的条件表达式，如果条件成立，则返回 expr1 的值，否则返回 expr2 的值。可以将三目运算符视为 if-else 语句的简化形式。
  //fmod 函数的余数的符号与除数相同。因此，当 angle 的值为负数时，余数的符号将与 _2PI 的符号相反。也就是说，如果 angle 的值小于 0 且 _2PI 的值为正数，则 fmod(angle, _2PI) 的余数将为负数。
  //例如，当 angle 的值为 -PI/2，_2PI 的值为 2PI 时，fmod(angle, _2PI) 将返回一个负数。在这种情况下，可以通过将负数的余数加上 _2PI 来将角度归一化到 [0, 2PI] 的范围内，以确保角度的值始终为正数。
}

// 归一化角度到 [0, 360] 度
float _normalizeAngle(float angle) {
    float a = fmod(angle, 360.0);   // 取余运算可以用于归一化
    return a >= 0 ? a : (a + 360.0);  
}


/// @brief 计算电机0电角度
/// @param motor 
int aligns_Motor_Zero_Angle(Motor* motor)
{
  setTorque(motor,3, _3PI_2);
  tx_thread_sleep(2000);  //在ThreadX中不能使用hal_Delay()
  Read_AS5600_Angle(motor->As5600_Sensor);
  motor->zero_electric_angle = _electricalAngle(motor);
  setTorque(motor,0, _3PI_2);
  printf("%d号电机零电角度:%.2f\r\n",motor->Motor_ID,motor->zero_electric_angle);
  return 0;
}

/// @brief 获取电机速度(带有滤波)
/// @param motor 
/// @return 
float Get_filter_Velocity(Motor* motor)
{
  //获取速度数据并滤波
  float vel_M0_ori=Get_AS5600_Velocity(motor->As5600_Sensor);
  float vel_M0_flit=Lowpass_Filter_calc(motor->Lowpass_Filter_Vel,motor->DIR*vel_M0_ori);
  // motor->As5600_Sensor->Velocity = vel_M0_flit;
  return vel_M0_flit;   //考虑方向
}

/// @brief 通过Ia,Ib,Ic计算Iq,Id(目前仅输出Iq)
/// @param Sensor 
/// @param angle_el 
/// @return 
float cal_Iq_Id(Inlinecurrent_Typedef *Sensor, float angle_el)
{
  float I_alpha = Sensor->I_a;
  float I_beta = _1_SQRT3 * Sensor->I_a + _2_SQRT3 * Sensor->I_b;

  float ct = cos(angle_el);
  float st = sin(angle_el);
  float I_q = I_beta * ct - I_alpha * st;
	
  return I_q;
}

/// @brief 获取电机总电流(带滤波)
/// @param motor 
/// @return 
float Get_Current(Motor* motor)
{
  float I_q_M0_ori=cal_Iq_Id(motor->Inlinecurrent,_electricalAngle(motor));
  float I_q_M0_flit=Lowpass_Filter_calc(motor->Lowpass_Filter_Curr, I_q_M0_ori);
  motor->Inlinecurrent->I_Q = I_q_M0_flit;
  return I_q_M0_flit;
}


/// @brief 角度,速度环(电压)
/// @param motor 
/// @param target 
void Set_Velocity_Angle(Motor* motor, float target)
{
 setTorque(motor,Pid_clac(motor->Pid_Vel,Pid_clac(motor->Pid_Ang,(target-Get_AS5600_Angle(motor->As5600_Sensor))*180/PI)),_electricalAngle(motor));   //角度,速度双闭环
}

/// @brief 速度环(电压)
/// @param motor 
/// @param target 
void Set_Velocity(Motor* motor, float target)
{
  setTorque(motor,Pid_clac(motor->Pid_Vel,(target-Get_filter_Velocity(motor))*180/PI),_electricalAngle(motor));   //速度闭环
}

/// @brief 力位(电压)
/// @param motor 
/// @param target 
void Set_Force_Angle(Motor* motor, float target)
{
  setTorque(motor,Pid_clac(motor->Pid_Ang,(target-Get_AS5600_Angle(motor->As5600_Sensor))*180/PI),_electricalAngle(motor));     //力位
}

/// @brief 力矩(电流)
/// @param motor 
/// @param Target 
void setTorque_Curr(Motor* motor, float Target)
{
  setTorque(motor,Pid_clac(motor->Pid_Curr,Target-Get_Current(motor)),_electricalAngle(motor));
}

/// @brief 力位闭环(电流)
/// @param motor 
/// @param Target 
void set_Force_Angle_Curr(Motor* motor, float Target)      //力位闭环
{
  setTorque_Curr(motor,Pid_clac(motor->Pid_Ang, (Target-Get_AS5600_Angle(motor->As5600_Sensor))*180/PI));
}

/// @brief 速度闭环(电流)
/// @param motor 
/// @param Target 
void set_Velocity_Curr(Motor* motor, float Target)          //速度闭环
{
  setTorque_Curr(motor,Pid_clac(motor->Pid_Vel,(Target-Get_filter_Velocity(motor))*180/PI)); 
}

/// @brief 角度-速度-力 位置闭环
/// @param motor 
/// @param Target 
void set_Velocity_Angle_Curr(Motor* motor, float Target)   //角度-速度-力 位置闭环
{
  setTorque_Curr(motor,Pid_clac(motor->Pid_Vel,Pid_clac(motor->Pid_Ang,(Target-Get_AS5600_Angle(motor->As5600_Sensor))*180/PI)-Get_filter_Velocity(motor))); 
}




/// @brief 获取电角度
/// @return float
float _electricalAngle(Motor* motor)
{
    // return  _normalizeAngle((float)(motor->DIR *  motor->PP) * motor->As5600_Sensor->Angle-motor->zero_electric_angle);
    return  _normalizeRadian((float)(motor->DIR *  motor->PP) * motor->As5600_Sensor->Angle-motor->zero_electric_angle);
}


/// @brief 设置PWM到控制器输出
/// @param motor 电机对象
/// @param Ua 
/// @param Ub 
/// @param Uc 
void setPwm(Motor* motor, float Ua, float Ub, float Uc) 
{
  // 限制上限
  Ua = _constrain(Ua, 0.0f, motor->voltage_power_supply);
  Ub = _constrain(Ub, 0.0f, motor->voltage_power_supply);
  Uc = _constrain(Uc, 0.0f, motor->voltage_power_supply);
  // 计算占空比
  // 限制占空比从0到1
  float dc_a = _constrain(Ua / motor->voltage_power_supply, 0.0f , 1.0f );
  float dc_b = _constrain(Ub / motor->voltage_power_supply, 0.0f , 1.0f );
  float dc_c = _constrain(Uc / motor->voltage_power_supply, 0.0f , 1.0f );

  //写入PWM
  motor->Motor_Set_Compare1(dc_a*3300);
  motor->Motor_Set_Compare2(dc_b*3300);
  motor->Motor_Set_Compare3(dc_c*3300);
}

/// @brief 设置力矩
/// @param Uq 需求力矩
/// @param angle_el 
void setTorque(Motor* motor,float Uq,float angle_el)
{

  Uq=_constrain(Uq,-(motor->voltage_power_supply)/2,(motor->voltage_power_supply)/2);

  angle_el = _normalizeAngle(angle_el);
  // 帕克逆变换
  motor->Ualpha =  -Uq*sin(angle_el);
  motor->Ubeta =   Uq*cos(angle_el);

  // 克拉克逆变换
  motor->Ua = motor->Ualpha + motor->voltage_power_supply/2;
  motor->Ub = (sqrt(3)*motor->Ubeta-motor->Ualpha)/2 + motor->voltage_power_supply/2;
  motor->Uc = (-motor->Ualpha-sqrt(3)*motor->Ubeta)/2 + motor->voltage_power_supply/2;
  setPwm(motor,motor->Ua,motor->Ub,motor->Uc);
}

/// @brief 电机参数自动识别
/// @param motor 电机对象
/// @param diagnostic 诊断信息结构体
/// @return 0:成功, -1:失败
int auto_identify_motor_params(Motor* motor) 
{
    int result = 0;  // 返回值
    
    // 参数检查
    if(motor == NULL || motor->diagnostic == NULL) {
        result = -1;
    }
    else {
        // 初始化诊断信息
        memset(motor->diagnostic, 0, sizeof(MotorIdentifyDiagnostic));
        motor->diagnostic->state = MOTOR_IDENTIFY_RESISTANCE;
        motor->diagnostic->identify_time = tx_time_get();
        
        // 安全限值
        const float MAX_CURRENT = 5.0f;      // 最大电流限制
        const float MAX_VOLTAGE = 12.0f;     // 最大电压限制
        const float MIN_CURRENT = 0.01f;     // 最小有效电流
        const uint32_t TIMEOUT_MS = 5000;    // 超时时间
        
        // 1. 识别电机电阻
        if(result == 0) 
        {
            motor->diagnostic->state = MOTOR_IDENTIFY_RESISTANCE;
            float voltage = 1.0f;  // 初始测试电压
            float current = 0.0f;
            uint32_t start_time = tx_time_get();
            
            // 渐进式增加电压，避免电流突变
            for(float v = 0.5f; v <= voltage && result == 0; v += 0.1f) {
                setTorque(motor, v, 0.0f);
                tx_thread_sleep(10);
                current = Get_Current(motor);
                
                // 电流监控
                if(fabs(current) > MAX_CURRENT) {
                    motor->diagnostic->error_code = 0x01;  // 电流超限
                    motor->diagnostic->state = MOTOR_IDENTIFY_ERROR;
                    result = -1;
                }
                
                // 超时检查
                if(tx_time_get() - start_time > TIMEOUT_MS) {
                    motor->diagnostic->error_code = 0x02;  // 识别超时
                    motor->diagnostic->state = MOTOR_IDENTIFY_ERROR;
                    result = -1;
                }
            }
            
            if(result == 0) 
            {
                if(fabs(current) < MIN_CURRENT) {
                    motor->diagnostic->error_code = 0x03;  // 电流太小
                    motor->diagnostic->state = MOTOR_IDENTIFY_ERROR;
                    result = -1;
                }
                else {
                    motor->diagnostic->resistance = voltage / current;
                    motor->diagnostic->max_current = fabs(current);
                }
            }
        }
        
        // 2. 识别电机电感
        if(result == 0) 
        {
            motor->diagnostic->state = MOTOR_IDENTIFY_INDUCTANCE;
            float voltage = 2.0f;
            float current1 = 0.0f;
            float current2 = 0.0f;
            
            // 多次测量取平均值
            float inductance_sum = 0.0f;
            const int MEASURE_COUNT = 5;
            
            for(int i = 0; i < MEASURE_COUNT && result == 0; i++) 
            {
                setTorque(motor, voltage, 0.0f);
                current1 = Get_Current(motor);
                tx_thread_sleep(10);
                current2 = Get_Current(motor);
                
                float di_dt = (current2 - current1) / 0.01f;
                inductance_sum += voltage / di_dt;
                
                // 电流监控
                if(fabs(current2) > MAX_CURRENT) {
                    motor->diagnostic->error_code = 0x04;
                    motor->diagnostic->state = MOTOR_IDENTIFY_ERROR;
                    result = -1;
                }
            }
            
            if(result == 0) {
                motor->diagnostic->inductance = inductance_sum / MEASURE_COUNT;
            }
        }
        
        // 3. 识别电机极对数
        if(result == 0) 
        {
            motor->diagnostic->state = MOTOR_IDENTIFY_POLE_PAIRS;
            float voltage = 3.0f;
            float angle1 = Get_AS5600_Angle(motor->As5600_Sensor);
            
            // 渐进式增加电压
            for(float v = 1.0f; v <= voltage && result == 0; v += 0.5f) 
            {
                setTorque(motor, v, 0.0f);
                tx_thread_sleep(100);
            }
            
            if(result == 0) 
            {
                tx_thread_sleep(1000);
                float angle2 = Get_AS5600_Angle(motor->As5600_Sensor);
                float angle_diff = fabs(angle2 - angle1);
                
                if(angle_diff < 0.1f) 
                {
                    motor->diagnostic->error_code = 0x05;  // 角度变化太小
                    motor->diagnostic->state = MOTOR_IDENTIFY_ERROR;
                    result = -1;
                }
                else {
                    motor->diagnostic->pole_pairs = (int)(2.0f * PI / angle_diff + 0.5f);
                }
            }
        }
        
        // 4. 识别零电角度
        if(result == 0) 
        {
            motor->diagnostic->state = MOTOR_IDENTIFY_ZERO_ANGLE;
            if(aligns_Motor_Zero_Angle(motor) != 0) {
                motor->diagnostic->error_code = 0x06;  // 零电角度识别失败
                motor->diagnostic->state = MOTOR_IDENTIFY_ERROR;
                result = -1;
            }
            else 
            {
                motor->diagnostic->zero_angle = motor->zero_electric_angle;
            }
        }
        
        // 更新电机参数
        if(result == 0) 
        {
            motor->PP = motor->diagnostic->pole_pairs;
            motor->zero_electric_angle = motor->diagnostic->zero_angle;
            
            // 完成识别
            motor->diagnostic->state = MOTOR_IDENTIFY_COMPLETE;
            motor->diagnostic->identify_time = tx_time_get() - motor->diagnostic->identify_time;
            
            // 输出诊断信息
            printf("电机参数识别完成\r\n");
            printf("识别时间: %lu ms\r\n", motor->diagnostic->identify_time);
            printf("电阻: %.3f Ohm\r\n", motor->diagnostic->resistance);
            printf("电感: %.3f H\r\n", motor->diagnostic->inductance);
            printf("极对数: %d\r\n", motor->diagnostic->pole_pairs);
            printf("零电角度: %.3f rad\r\n", motor->diagnostic->zero_angle);
            printf("最大电流: %.3f A\r\n", motor->diagnostic->max_current);
            printf("最大电压: %.3f V\r\n", motor->diagnostic->max_voltage);
        }
    }
    
    // 确保电机停止
    setTorque(motor, 0.0f, 0.0f);
    
    return result;
}













/// @brief 设置电机1通道1pwm输出占空比
/// @param compare 占空比值
void __attribute__((weak)) Motor1_Set_Compare1(uint32_t compare) 
{
    __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, compare);
}

/// @brief 设置电机1通道2pwm输出占空比
/// @param compare 占空比值
void __attribute__((weak)) Motor1_Set_Compare2(uint32_t compare) 
{
    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, compare);
}

/// @brief 设置电机1通道3pwm输出占空比
/// @param compare 占空比值
void __attribute__((weak)) Motor1_Set_Compare3(uint32_t compare) 
{
    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, compare);
}

/// @brief 设置电机2通道1pwm输出占空比
/// @param compare 占空比值
void __attribute__((weak)) Motor2_Set_Compare1(uint32_t compare) 
{
    __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, compare);
}

/// @brief 设置电机2通道2pwm输出占空比
/// @param compare 占空比值
void __attribute__((weak)) Motor2_Set_Compare2(uint32_t compare) 
{
    __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, compare);
}

/// @brief 设置电机2通道3pwm输出占空比
/// @param compare 占空比值
void __attribute__((weak)) Motor2_Set_Compare3(uint32_t compare) 
{
    __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, compare);
}


