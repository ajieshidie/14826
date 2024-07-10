
#include "stm32f10x.h"  //包含需要的头文件
#include "anjian.h"        //包含需要的头文件


void anjian_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStructure;                     //定义一个设置GPIO的变量
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);   //使能GPIOB端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;   //设置PB0 PB1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //IO速率50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		 //推免输出方式
	GPIO_Init(GPIOA, &GPIO_InitStructure);            		 //设置PB0 PB1
	

}


