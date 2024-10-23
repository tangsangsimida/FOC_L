#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "math.h"
#include "stdarg.h"

char Serial_RxPacket[100];
uint8_t Serial_TxPacket[100];
uint8_t Serial_RxFlag;

void Serial_Init(uint32_t Bond)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = Bond;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_Init(USART1, &USART_InitStructure); 
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
  USART_Cmd(USART1, ENABLE);  
}


//自动清除标志位
uint8_t Serial_GetRxFlag(void)
{
	if(Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

//USART中断函数（数据包状态机接收）
void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0;
	static uint8_t pRxState = 0;
	
	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET)//判断是否受到数据,接收的话标志位会置1
	{
		uint8_t RXData = USART_ReceiveData(USART1);

		if(RxState == 0)
		{
			if(RXData == 'T')
			{
				RxState = 1;
			}
		}
		
		else if(RxState == 1)
		{
			if(RXData == '\r')
			{
				RxState = 2;
			}
			else
			{
			 	Serial_RxPacket[pRxState] = RXData;
			  pRxState ++;
			}
		}
		
		else if(RxState == 2)
		{
			if(RXData == '\n')
			{
				RxState = 0;
				Serial_RxPacket[pRxState] = '\0';
				pRxState = 0;
				Serial_RxFlag = 1;
			}
		}
		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);//手动清除标志位，正常来说读取DR便会自动清除，可加可不加
	}
}

//单片机串口发送一个字节
void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);//等待数据转运完整，等待标志位值1，没传完数据前都是0。传完后寄存器被清空，当下次执行SendData时，标志位自动清零
}

//将数字每一位拆开
uint32_t Serial_Pow(uint32_t x, uint32_t y)
{
	uint32_t Result = 1;
	while(y--)
		{
			Result = Result * x;
		}
		return Result;
}

//发送数组
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for(i = 0; i < Length; i++)
	{
		Serial_SendByte(Array[i]);
	}
}

//发送字符串
void Serial_SendString(char*String)
{
	uint8_t i;
	for(i = 0; String[i] != '\0'; i++)
	{
		Serial_SendByte(String[i]);
	}
}

//发送一串数字
void Serial_SendNumber(uint32_t Number,uint8_t Length)
{
	uint8_t i;
	for(i = 0 ; i < Length; i++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1)%10 + '0');
	}
}

//发送正负浮点数
void Serial_SendFloatNumber(float FloatNumber,uint8_t Int_Length, uint8_t Float_Length)
{
	if (FloatNumber > 0 || FloatNumber == 0)
	{
		uint32_t Int_num = (uint32_t)FloatNumber;
		uint32_t Float_num = FloatNumber * Serial_Pow(10,Float_Length);
		
//		Float_num = Float_num % 600;
		for(int i = 0 ; i < Int_Length; i++)
		{
			Serial_SendByte(Int_num / Serial_Pow(10, Int_Length - i - 1)%10 + '0');
		}
		Serial_SendString(".");
		for(int j = 0 ; j < Float_Length; j++)
		{
			Serial_SendByte(Float_num / Serial_Pow(10, Float_Length - j - 1)%10 + '0');
		}
	}
	else if(FloatNumber < 0)
	{
		FloatNumber = -FloatNumber;
		uint32_t Int_num = (uint32_t)FloatNumber;
		uint32_t Float_num = FloatNumber * Serial_Pow(10,Float_Length);
//		Float_num = Float_num % 600;
		Serial_SendString("-");
		for(int i = 0 ; i < Int_Length; i++)
		{
			Serial_SendByte(Int_num / Serial_Pow(10, Int_Length - i - 1)%10 + '0');
		}
		Serial_SendString(".");
		for(int j = 0 ; j < Float_Length; j++)
		{
			Serial_SendByte(Float_num / Serial_Pow(10, Float_Length - j - 1)%10 + '0');
		}
	}
//	Serial_SendString("\n");
}

void Serial_SendPacket(void)//发送数据包
{
	Serial_SendByte(0xFF);
	Serial_SendArray(Serial_TxPacket,4);
	Serial_SendByte(0xFE);
}

//兼容printf打印
int fputc(int ch, FILE * f)
{
	Serial_SendByte(ch);
	return ch;
}

//打印可变参数函数
void Serial_Printf(char * format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg,format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}







