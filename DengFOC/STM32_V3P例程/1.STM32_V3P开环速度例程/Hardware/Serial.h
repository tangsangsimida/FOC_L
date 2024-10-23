#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

extern char Serial_RxPacket[];


void Serial_Init(uint32_t Bond);
uint8_t Serial_GetRxFlag(void);

void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char*String);
uint32_t Serial_Pow(uint32_t x, uint32_t y);
void Serial_SendFloatNumber(float FloatNumber,uint8_t Int_Length, uint8_t Float_Length);
void Serial_SendNumber(uint32_t Number,uint8_t Length);

void Serial_SendPacket(void);

uint16_t Serial_ReceiveByte(void);

void USART1_IRQHandler(void);

int fputc(int ch, FILE * f);
void Serial_Printf(char * format, ...);




#endif
