#include "stm32f10x.h"
#include "string.h"
#include "stdio.h"
unsigned char UARTbuff[100];  
unsigned char AT1[32]="AT+PSWD=\"6789\"\r\n";
unsigned char AT2[32]="AT+ROLE=0\r\n";
unsigned char AT3[32]="AT+RESET\r\n";
int k=0;
static unsigned char count=0;
void delayms(unsigned int t)

{

unsigned x,y;

for(x=t;x>0;x--)

for(y=12400;y>0;y--);

}

void My_USART1_Init(unsigned long int bound)
{
	GPIO_InitTypeDef GPIO_InitStrue;
	USART_InitTypeDef USART_InitStrue;
	NVIC_InitTypeDef NVIC_InitStrue;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_10MHz;
  GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_10MHz;
  GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	USART_InitStrue.USART_BaudRate=bound;
	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStrue.USART_Parity=USART_Parity_No;
	USART_InitStrue.USART_StopBits=USART_StopBits_1;
	USART_InitStrue.USART_WordLength=USART_WordLength_8b;
	
	USART_Init(USART1,&USART_InitStrue);
	
	USART_Cmd(USART1,ENABLE);//使能串口1
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//开启接收中断
	
	NVIC_InitStrue.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStrue);
	
	
}

void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 端口配置, 推挽输出
 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOE,GPIO_Pin_5); 						 //PE.5 输出高 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    		 //LED1-->PE.5 端口配置, 推挽输出
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOA,GPIO_Pin_4); 	
}
void sendchars(unsigned char *UARTbuff)
{  
	
	
	
	unsigned char i = 0;
//	
//	i=strlen((const char*)UARTbuff);		//此次发送数据的长度
//	for(j=0;j<i;j++)							//循环发送数据
//	{
//	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
//		USART_SendData(USART1,UARTbuff[j]); 
	
while(UARTbuff[i]!= '\0')
{   

  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
	USART_SendData(USART1,UARTbuff[i]);
USART_ClearFlag(USART1,USART_FLAG_TC);
   i++;      
}

}


void USART1_IRQHandler(void)
{

	 if(USART_GetITStatus(USART1,USART_IT_RXNE))
 {
    

	
      USART_ClearITPendingBit(USART1, USART_IT_RXNE);     
       UARTbuff[count]= USART_ReceiveData(USART1); 
      count++;
			if(count>7)
         {
         count=0;
        USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
	       k=1;
           }
    } 
	 
	 
  }

 
 int main(void)
 {	
	 char a[15];
	 int  i=0;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
	My_USART1_Init(38400);
	delayms(1000); 	
	GPIO_SetBits(GPIOA,GPIO_Pin_4); 	
	sendchars(AT1);
  delayms(20);
	sendchars(AT2);
	delayms(20);
	sendchars(AT3);
	delayms(5000); 	 
  GPIO_ResetBits(GPIOA,GPIO_Pin_4); 	
 	My_USART1_Init(115200);
	 while( UARTbuff[i++]!='\0')//判断不是空的话就让它为空
       UARTbuff[i-1]='\0';
	 while(1)
{
	
	
if(k==1)
   {
    
    k=0;
   USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//开启接收中断
    sprintf(a,"%s",UARTbuff);
		 sendchars(UARTbuff);
	if(strstr((const char*)a,"oonled"))
	{  GPIO_ResetBits(GPIOE,GPIO_Pin_5); 
	   GPIO_ResetBits(GPIOB,GPIO_Pin_5);	
		 while( UARTbuff[i++]!='\0')//判断不是空的话就让它为空
       UARTbuff[i-1]='\0';
	}		
  if(strstr((const char*)a,"offled"))
	{  GPIO_SetBits(GPIOB,GPIO_Pin_5);	 
	   GPIO_SetBits(GPIOE,GPIO_Pin_5);	
		 while( UARTbuff[i++]!='\0')//判断不是空的话就让它为空
       UARTbuff[i-1]='\0';
	}
}
	 
 }
 }

