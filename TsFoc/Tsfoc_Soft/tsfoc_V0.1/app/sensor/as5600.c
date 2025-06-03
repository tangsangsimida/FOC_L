#include "as5600.h"

/// @brief IIC写SCL引脚数据             
/// @param Sensor_Port SCL引脚的端口    exam: GPIOB
/// @param Sensor_Pin_SCL SCL引脚编号   exam: GPIO_PIN_0
/// @param BitValue 引脚状态            exam: GPIO_PIN_RESET
void I2C_W_SCL(GPIO_TypeDef * Sensor_Port, uint16_t  Sensor_Pin_SCL, GPIO_PinState BitValue)
{
	HAL_GPIO_WritePin(Sensor_Port, Sensor_Pin_SCL, BitValue);
}
 
/// @brief IIC写SDA引脚数据             
/// @param Sensor_Port SDA引脚的端口    exam: GPIOB
/// @param Sensor_Pin_SDA SDA引脚编号   exam: GPIO_PIN_0
/// @param BitValue 引脚状态            exam: GPIO_PIN_RESET
void I2C_W_SDA(GPIO_TypeDef * Sensor_Port, uint16_t  Sensor_Pin_SDA, GPIO_PinState BitValue)
{
	HAL_GPIO_WritePin((GPIO_TypeDef *)Sensor_Port, Sensor_Pin_SDA, (GPIO_PinState)BitValue);
}

/// @brief 读取IIC-SDA引脚数据
/// @param Sensor_Port SDA引脚的端口    exam: GPIOB
/// @param Sensor_Pin_SDA SDA引脚编号   exam: GPIO_PIN_0
/// @return 引脚状态
uint8_t I2C_R_SDA(GPIO_TypeDef * Sensor_Port, uint16_t Sensor_Pin_SDA)
{
	uint8_t BitValue;
	BitValue = HAL_GPIO_ReadPin(Sensor_Port, Sensor_Pin_SDA);
	return BitValue;
}

/// @brief IIC起始信号
/// @param sensor as5600传感器对象
void I2C_Start(As5600_Sensor_Typedef* sensor)
{
    I2C_W_SCL(sensor->Port,sensor->SCL_PIN,(GPIO_PinState)1);
    I2C_W_SDA(sensor->Port,sensor->SDA_PIN,(GPIO_PinState)1);
    I2C_W_SDA(sensor->Port,sensor->SDA_PIN,(GPIO_PinState)0);
    I2C_W_SCL(sensor->Port,sensor->SCL_PIN,(GPIO_PinState)0);
}

/// @brief IIC结束信号
/// @param sensor as5600传感器对象
void I2C_Stop(As5600_Sensor_Typedef* sensor)
{
    I2C_W_SDA(sensor->Port,sensor->SDA_PIN,(GPIO_PinState)0);
    I2C_W_SCL(sensor->Port,sensor->SCL_PIN,(GPIO_PinState)1);
    I2C_W_SDA(sensor->Port,sensor->SDA_PIN,(GPIO_PinState)1);
}

/// @brief IIC发送一个字节
/// @param sensor as5600传感器对象
/// @param Byte 需要发送的字节数据
void I2C_SendByte(As5600_Sensor_Typedef* sensor, uint8_t Byte)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		I2C_W_SDA(sensor->Port, sensor->SDA_PIN, (GPIO_PinState)(Byte & (0x80>>i)));
        I2C_W_SCL(sensor->Port,sensor->SCL_PIN,(GPIO_PinState)1);
        I2C_W_SCL(sensor->Port,sensor->SCL_PIN,(GPIO_PinState)0);
	}
}

/// @brief IIC读取数据
/// @param sensor as5600传感器对象
/// @return (uint8_t) Byte
uint8_t I2C_RecviveData(As5600_Sensor_Typedef* sensor)
{
    uint8_t i, Byte = 0x00;
    I2C_W_SDA(sensor->Port,sensor->SDA_PIN,(GPIO_PinState)1);
	for(i=0;i<8;i++)
	{
        I2C_W_SCL(sensor->Port,sensor->SCL_PIN,(GPIO_PinState)1);
		if(I2C_R_SDA(sensor->Port,sensor->SDA_PIN) == 1) {Byte |= (0x80 >> i);}
        I2C_W_SCL(sensor->Port,sensor->SCL_PIN,(GPIO_PinState)0);
	}
	return Byte;
}

/// @brief 发送应达位
/// @param sensor as5600传感器对象
/// @param AckBit 应达位
void I2C_SendAck(As5600_Sensor_Typedef* sensor, uint8_t AckBit)
{
    I2C_W_SDA(sensor->Port,sensor->SDA_PIN,(GPIO_PinState)AckBit);
    I2C_W_SCL(sensor->Port,sensor->SCL_PIN,(GPIO_PinState)1);
    I2C_W_SCL(sensor->Port,sensor->SCL_PIN,(GPIO_PinState)0);
}

/// @brief 接收应答位
/// @param sensor as5600传感器对象
/// @return uint8_t
uint8_t I2C_RecviveAck(As5600_Sensor_Typedef* sensor)
{
	uint8_t AckBit;
    I2C_W_SDA(sensor->Port,sensor->SDA_PIN,(GPIO_PinState)1);
    I2C_W_SCL(sensor->Port,sensor->SCL_PIN,(GPIO_PinState)1);
    AckBit = I2C_R_SDA(sensor->Port,sensor->SDA_PIN);
    I2C_W_SCL(sensor->Port,sensor->SCL_PIN,(GPIO_PinState)0);
    return AckBit;
}

/// @brief IIC读取寄存器
/// @param sensor as5600传感器对象
/// @param RegAddress 寄存器地址
/// @return uint8_t
uint8_t AS5600_ReadReg(As5600_Sensor_Typedef* sensor, uint8_t RegAddress)
{
	uint8_t Data;
    I2C_Start(sensor);
    I2C_SendByte(sensor, AS5600_ADDRESS);
	I2C_RecviveAck(sensor);
	I2C_SendByte(sensor, RegAddress);
	I2C_RecviveAck(sensor);

	I2C_Start(sensor);
	I2C_SendByte(sensor, AS5600_ADDRESS | 0x01);
	I2C_RecviveAck(sensor);
	Data = I2C_RecviveData(sensor);
	I2C_SendAck(sensor, 1);
	I2C_Stop(sensor);
	
	return Data;
}

/// @brief 获取as5600角度寄存器中的内容
/// @param sensor as5600传感器对象
/// @return uint8_t
float AS5600_GetRawData(As5600_Sensor_Typedef* sensor)
{
	uint8_t Data_L;
	uint8_t Data_H;
	float Raw_Data = 0;
	
	I2C_Start(sensor);
	I2C_SendByte(sensor, AS5600_ADDRESS);
	I2C_RecviveAck(sensor);
	I2C_SendByte(sensor, AS5600_RAW_ANGLE_H);
	I2C_RecviveAck(sensor);
	
	I2C_Start(sensor);
	I2C_SendByte(sensor, AS5600_ADDRESS | 0x01);
	I2C_RecviveAck(sensor);
	Data_H = I2C_RecviveData(sensor);
	I2C_RecviveAck(sensor);
	
	I2C_Start(sensor);
	I2C_SendByte(sensor, AS5600_ADDRESS | 0x01);
	I2C_RecviveAck(sensor);
	Data_L = I2C_RecviveData(sensor);
	I2C_SendAck(sensor, 1);
	I2C_Stop(sensor);
	
	Raw_Data = (Data_H << 8) | Data_L;
	
	return Raw_Data;
}

/// @brief 获取会复位角度(0-360)
/// @param sensor as5600传感器对象
/// @return float
float AS5600_GetAngle_NoTrack(As5600_Sensor_Typedef * sensor)
{
	float Angle = 0.0;
	Angle = (AS5600_GetRawData(sensor)/4096.) * _2PI;
	return Angle;
}

/// @brief 获取不复位的角度
/// @param AS5600 as5600传感器对象
/// @return float
float AS5600_getAngle(As5600_Sensor_Typedef * AS5600)
{
    // float val = AS5600_GetAngle_NoTrack(AS5600);
    // float d_angle = val - AS5600->angle_prev;
    // //计算旋转的总圈数
    // //通过判断角度变化是否大于80%的一圈(0.8f*_2PI)来判断是否发生了溢出，如果发生了，则将full_rotations增加1（如果d_angle小于0）或减少1（如果d_angle大于0）。
    // if(fabs(d_angle) > (0.8f*_2PI) ) AS5600->full_rotations += ( d_angle > 0 ) ? -1 : 1; 
    // AS5600->angle_prev = val;
    // return (float)(AS5600->full_rotations) * _2PI + AS5600->angle_prev;
	
	return (float)(AS5600->full_rotations) * _2PI + AS5600->angle_prev;
}

/// @brief 更新角度
/// @param AS5600 as5600传感器对象
void AS5600_Sensor_update(As5600_Sensor_Typedef * AS5600) 
{
    float val = (AS5600_GetRawData(AS5600)/4096.) * _2PI;
    AS5600->angle_prev_ts = HAL_GetTick();
    float d_angle = val - AS5600->angle_prev;
    // 圈数检测
    if(fabs(d_angle) > (0.8f*_2PI) ) AS5600->full_rotations += ( d_angle > 0 ) ? -1 : 1; 
    AS5600->angle_prev = val;
}

/// @brief 初始化传感器
/// @param AS5600 as5600传感器对象
void AS5600_Init(As5600_Sensor_Typedef * AS5600) 
{
	AS5600->full_rotations = 0;
	AS5600->vel_full_rotations = 0;
	AS5600->angle_prev = AS5600_GetAngle_NoTrack(AS5600);
	AS5600->angle_prev_ts = HAL_GetTick();
}


float AS5600_getVelocity(As5600_Sensor_Typedef * AS5600) 
{
    // 计算采样时间
    float Ts = (AS5600->angle_prev_ts - AS5600->vel_angle_prev_ts)*1e-3;
    // 快速修复奇怪的情况（微溢出）
    if(Ts <= 0) Ts = 1e-3f;
    // 速度计算
    float vel = ( (float)(AS5600->full_rotations - AS5600->vel_full_rotations)*_2PI + (AS5600->angle_prev - AS5600->vel_angle_prev) ) / Ts;    
    // 保存变量以待将来使用
    AS5600->vel_angle_prev = AS5600->angle_prev;
    AS5600->vel_full_rotations = AS5600->full_rotations;
    AS5600->vel_angle_prev_ts = AS5600->angle_prev_ts;
    return vel;		//返回速度
}

