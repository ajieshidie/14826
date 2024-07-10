#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
/*-------------------------------------------------*/
/*����������ʱ��5ʹ��40s��ʱ                        */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM5_ENABLE_40S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //����һ�����ö�ʱ���ı���
	NVIC_InitTypeDef NVIC_InitStructure;                          //����һ�������жϵı���
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //�����ж��������飺��2�� �������ȼ���0 1 2 3 �����ȼ���0 1 2 3		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);           //ʹ��TIM5ʱ��
	TIM_DeInit(TIM5);                                             //��ʱ��5�Ĵ����ָ�Ĭ��ֵ	
	TIM_TimeBaseInitStructure.TIM_Period = 50000-1; 	          //�����Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=57600-1;               //���ö�ʱ��Ԥ��Ƶ��
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1��Ƶ
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);            //����TIM5
	
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);                    //�������жϱ�־λ
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);                      //ʹ��TIM5����ж�    
	TIM_Cmd(TIM5,ENABLE);                                         //��TIM5                          
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn;                 //����TIM5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;       //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;              //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);                               //�����ж�
}
