
#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "anjian.h"        //������Ҫ��ͷ�ļ�


void anjian_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStructure;                     //����һ������GPIO�ı���
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);   //ʹ��GPIOB�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;   //����PB0 PB1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //IO����50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		 //���������ʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);            		 //����PB0 PB1
	

}


