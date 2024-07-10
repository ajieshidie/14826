
#include "stm32f10x.h"    //������Ҫ��ͷ�ļ�
#include "delay.h" 
/*-------------------------------------------------*/
/*����������ʼ���ӳټ���������                     */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void delay_init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	//SysTick_CLKSource_HCLK_Div8 = 8 ��ʾ��Ƶϵ��
	//��Ϊ��Ƶϵ����������Ϊ8������SysTick���δ�ʱ�ӣ���Ƶ������Ƶ�ʵ�1/8
	//ͨ����Ƶ��Ϊ����72M����ôSysTick��Ƶ����9M
	//��ôSysTick��������1����������(1/9)us
}

/*-------------------------------------------------*/
/*���������ӳ�΢�뺯��                             */
/*��  ����us����ʱ����΢��                         */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void delay_us(u16 us)
{		
	unsigned int temp;	                      //����һ����������    	 
	
	SysTick->LOAD=us*9;                       //������������ֵ��Ҫע��SysTick�ǵ���������
	                                          //SysTick������ÿ����һ������1/9΢�룬����������us*9�����Ǽ�����������ֵ
	SysTick->VAL=0x00;                        //��յ�ǰ��������ֵ
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;  //ʹ��SysTick������������ֵ���ص��������У���ʼ��������
	do{
		temp=SysTick->CTRL;                   //ѭ����ȡSysTick״̬�Ĵ����������жϼ�ʱ�������
	}while(temp&0x01&&!(temp&(1<<16)));       //�ȴ�ʱ�䵽��
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;  //�رռ����� 
}

/*-------------------------------------------------*/
/*���������ӳٺ��뺯��                             */
/*��  ����ms����ʱ���ٺ���                         */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void delay_ms(u32 ms)
{
	//��������ע��һ������SysTickʱ�Ӽ�������24λ�ģ�9MƵ���£��ܹ�����ʱ1864.135ms
	//����������1800Ϊ���ޣ�С��1800����ʱһ�μ������У�����1800�Ķ�μ���	
	
	unsigned char i;      //����һ����������  
	unsigned int temp;    //����һ����������  
	
	/*-----------if�жϣ�С��1800ms��ʱ�������ִ��if��֧------------------*/
	if(ms<1800){                                   
		SysTick->LOAD=(unsigned int)ms*9*1000;     //������������ֵ��Ҫע��SysTick�ǵ���������        
		                                           //SysTick��1������1/9΢�룬�����ms�Ļ�������9�ٳ���1000�����Ǽ�����������ֵ
		SysTick->VAL=0x00;                         //��յ�ǰ��������ֵ
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;   //ʹ��SysTick������������ֵ���ص��������У���ʼ����
		do{
			temp=SysTick->CTRL;                    //ѭ����ȡSysTick״̬�Ĵ������жϼ�ʱ�������
		}while(temp&0x01&&!(temp&(1<<16)));        //�ȴ�ʱ�䵽��
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;}  //�رռ����� 
	
	/*--------------����1800ms��ʱ�������ִ��else��֧-------------------*/
	else{ 
		for(i=0;i<(ms/1800);i++){                     //����1800���������֣���forѭ����ÿ����ʱ1800ms
			SysTick->LOAD=(unsigned int)1800*9*1000;  //������������ֵ��Ҫע��SysTick�ǵ���������        
													  //SysTickһ������1/9΢�룬1800ms���� 1800*9*1000
			SysTick->VAL=0x00;                        //��յ�ǰ��������ֵ
			SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;  //ʹ��SysTick������������ֵ���ص��������У���ʼ����
			do{
				temp=SysTick->CTRL;                   //ѭ����ȡSysTick״̬�Ĵ������жϼ�ʱ�������
			}while(temp&0x01&&!(temp&(1<<16)));       //�ȴ�ʱ�䵽��
			SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;} //�رռ����� 
		
		//�������������ֵ���ʱ
		SysTick->LOAD=(unsigned int)(ms%1800)*9*1000; //������������ֵ��Ҫע��SysTick�ǵ���������  
		                                              //SysTickһ������1/9΢�룬�������־���(ms%1800)*9*1000
		SysTick->VAL =0x00;                           //��յ�ǰ��������ֵ
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;      //ʹ��SysTick������������ֵ���ص��������У���ʼ����  
		do{
			temp=SysTick->CTRL;                       //ѭ����ȡSysTick״̬�Ĵ������жϼ�ʱ�������
		}while(temp&0x01&&!(temp&(1<<16)));           //�ȴ�ʱ�䵽��
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;      //�رռ�����
	}
} 
/**
  * @brief  ΢�뼶��ʱ
  * @param  xus ��ʱʱ������Χ��0~233015
  * @retval ��
  */
void Delay_us(uint32_t xus)
{
	SysTick->LOAD = 72 * xus;				//���ö�ʱ����װֵ
	SysTick->VAL = 0x00;					//��յ�ǰ����ֵ
	SysTick->CTRL = 0x00000005;				//����ʱ��ԴΪHCLK��������ʱ��
	while(!(SysTick->CTRL & 0x00010000));	//�ȴ�������0
	SysTick->CTRL = 0x00000004;				//�رն�ʱ��
}

/**
  * @brief  ���뼶��ʱ
  * @param  xms ��ʱʱ������Χ��0~4294967295
  * @retval ��
  */
void Delay_ms(uint32_t xms)
{
	while(xms--)
	{
		Delay_us(1000);
	}
}
 
/**
  * @brief  �뼶��ʱ
  * @param  xs ��ʱʱ������Χ��0~4294967295
  * @retval ��
  */
void Delay_s(uint32_t xs)
{
	while(xs--)
	{
		Delay_ms(1000);
	}
} 
