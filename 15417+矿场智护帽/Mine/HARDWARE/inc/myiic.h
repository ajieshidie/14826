#ifndef __MAX30102_H
#define __MAX30102_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//IIC 驱动函数	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/6/10 
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  

   	   		   
//IO方向设置
#define SDA_IN()  {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=4;}	
#define SDA_OUT() {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=7;}

//IO操作函数	 
#define IIC_SCL    PBout(7) //SCL
#define IIC_SDA    PBout(8) //SDA	 
#define READ_SDA   PBin(8)  //输入SDA 

//初始化IIC
void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
void IIC_Ack(void);	    
void IIC_NAck(void);			  
void IIC_Send_Byte(u8 txd);   
u8 IIC_Read_Byte(unsigned char ack);
void IIC_WriteBytes(u8 WriteAddr,u8* data,u8 dataLength);
void IIC_ReadBytes(u8 deviceAddr, u8 writeAddr,u8* data,u8 dataLength);
void IIC_Read_One_Byte(u8 daddr,u8 addr,u8* data);
void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);

#endif
