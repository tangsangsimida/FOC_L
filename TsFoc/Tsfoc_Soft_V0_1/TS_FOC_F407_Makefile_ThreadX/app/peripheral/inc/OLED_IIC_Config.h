/**
 * ************************************************************************
 * 
 * @file OLED_IIC_Config.h
 * @author zxr
 * @brief IIC和OLED基础配置头文件
 * 
 * ************************************************************************
 * @copyright Copyright (c) 2024 zxr 
 * ************************************************************************
 */
#ifndef OLED_IIC_CONFIG_H
#define OLED_IIC_CONFIG_H

#include "main.h"
#include "stdint.h"


#define OLED_IIC_Handler 	hi2c2	//IIC句柄   


#define  OLED_ADDRESS 		0x78	//OLED地址  默认0x78

//OLED命令控制宏
#define  	LEFT 			0x27
#define  	RIGHT 			0x26
#define  	UP 			0X29
#define  	DOWM			0x2A
#define  	ON			0xA7
#define  	OFF			0xA6


#define    	SCREEN_PAGE_NUM		(8)     //屏幕页数
#define    	SCREEN_PAGEDATA_NUM	(128)   //每页的数据个数
#define		SCREEN_COLUMN		(128)   //列数
#define  	SCREEN_ROW		(64)    //行数


void WriteCmd(unsigned char cmd);		//写命令
void WriteDat(unsigned char dat);		//写数据
void OLED_ON(void);				//开启OLED
void OLED_OFF(void);				//休眠OLED
void OLED_CLS(void);				//OLED清屏函数
void OLED_Init(void);				//OLED初始化函数
void OLED_RefreshRAM(void);			//更新数据缓冲区
void OLED_ClearRAM(void);			//清除数据缓冲区
void OLED_SetPixel(signed short int x, signed short int y, unsigned char set_pixel);	//设置坐标像素点数据
void OLED_DisplayMode(unsigned char mode);	//屏幕内容取反显示
void OLED_IntensityControl(unsigned char intensity);//屏幕亮度调节
void OLED_Shift(unsigned char shift_num);	//全屏内容偏移指定距离
void OLED_HorizontalShift(unsigned char start_page,unsigned char end_page,unsigned char direction);	//屏幕内容水平方向滚动播放

#endif  /*OLED_IIC_CONFIG_H*/

