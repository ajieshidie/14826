#include "stm32f10x.h"  //包含需要的头文件
#include "timer2.h"     //包含需要的头文件
#include "main.h"
#include "delay.h"      //包含需要的头文件
#include "usart1.h"     //包含需要的头文件
#include "usart2.h"     //包含需要的头文件
#include "timer1.h"     //包含需要的头文件
#include "timer3.h"     //包含需要的头文件
#include "timer4.h"     //包含需要的头文件
#include "wifi.h"	    //包含需要的头文件
#include "mqtt.h"       //包含需要的头文件
#include "dht11.h"
#include "timer5.h"     //包含需要的头文件
#include "math.h"
#include "led.h"
#include "OLED.h"
#include "AD.h"
#include "beep.h"
#include "relay.h"  
#include "DS18B20.h"
#include "max30102.h"


/*
  压力     PA0
  lsens    PA1
  gas(MQ4) PA6
  air      PA7
  DHT11    PA8 
  DS18B20  PB12
   
  
*/
u8 humiH,humiL,tempH,tempL,NH3,nh3,lsens,pressure,gas,Air_Quality;
char ucbuf[17];
float temp_data=0.0;


void TEMP_Value_Conversion()
{
	temp_data=DS18B20_Get_Temp()/10.0;
	sprintf(ucbuf,"%4.2f",temp_data);
    OLED_ShowString(2,8,ucbuf);
}
void oeld_show(void)
{
	//第一行显示温湿度
	/*OLED_ShowCHinese(1,5,0);
	OLED_ShowCHinese(1,7,1);
	OLED_ShowCHinese(1,9,2);
	
	
	//第二行显示温度
	OLED_ShowCHinese(2,1,0);
	OLED_ShowCHinese(2,3,2);
	OLED_ShowCHinese(2,5,3);
	OLED_ShowNum(2,7,tempH,1);
	OLED_ShowNum(2,8,tempL,1);*/
	
	OLED_ShowString(1,1,"Pressure:");
	OLED_ShowNum(1,10,pressure,4);
	
//	OLED_ShowString(2,1,"Gas:");
//	OLED_ShowNum(2,5,gas,3);
	
	OLED_ShowString(2,1,"Temp:");
//	OLED_ShowNum(2,6,temper/10%100,2);
//	OLED_ShowString(2,8,".");
//	OLED_ShowNum(2,9,temper%10,2);
	

	
	//第三行显示温度
	OLED_ShowCHinese(3,1,1);
	OLED_ShowCHinese(3,3,2);
	OLED_ShowCHinese(3,5,3);
	OLED_ShowNum(3,7,humiH,1);
	OLED_ShowNum(3,8,humiL,1);
	
	//第四行显示光照
	OLED_ShowString(4,1,"lsens:");
	OLED_ShowNum(4,8,lsens,3);
}

//uint16_t adc_0(int t)
//{
//	int i=0,value;
//	for(i = 0;i<t ;i++)
//	{
//		value += AD_Value[0];
//		delay_ms(5);	
//	}
//	return value/t;
//	
//}
uint16_t adc_1(int t)
{
	int i=0,value;
	for(i = 0;i<t ;i++)
	{
		value += AD_Value[1];
		delay_ms(5);	
	}
	return value/t;
	
}

int main(void) 
{		
	
	usart1_init(115200);              //串口1功能初始化，波特率9600
	u1_printf("CONNECT报文成功\r\n"); 
	Usart2_Init(115200);            //串口2功能初始化，波特率115200	
	tim4_init(500,7200);            //TIM4初始化，定时时间 500*7200*1000/72000000 = 50ms
	delay_init();                   //延时功能初始化 
	wifi_reset_io_init();            //初始化WiFi的复位IO
	AliIoT_Parameter_Init();	    //初始化连接阿里云IoT平台MQTT服务器的参数	
	DHT11_Init();
	AD_Init();
	LED_Init();
	BEEP_Init();
	OLED_Init();
	relay_Init();
	DS18B20_Init();
	
	while(1)                        //主循环
	{ 			
		pressure =AD_Value[0]*1000/4095;
		lsens =100-(adc_1(10)*100/4095-100);
        gas=100-AD_Value[2]*1000/4095;
		Air_Quality=AD_Value[3]*1000/4095;
		
		DHT11_Read_Data(&humiH,&humiL,&tempH,&tempL);

		TEMP_Value_Conversion();
		
		oeld_show();


		
		/*--------------------------------------------------------------------*/
		/*   Connect_flag=1同服务器建立了连接,我们可以发布数据和接收推送了    */
		/*--------------------------------------------------------------------*/
		if(Connect_flag==1){    
							
			/*-------------------------------------------------------------*/
			/*                     处理发送缓冲区数据                      */
			/*-------------------------------------------------------------*/
				if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr){                //if成立的话，说明发送缓冲区有数据了
				//3种情况可进入if
				//第1种：0x10 连接报文
				//第2种：0x82 订阅报文，且ConnectPack_flag置位，表示连接报文成功
				//第3种：SubcribePack_flag置位，说明连接和订阅均成功，其他报文可发
				if((MQTT_TxDataOutPtr[2]==0x10)||((MQTT_TxDataOutPtr[2]==0x82)&&(ConnectPack_flag==1))||(SubcribePack_flag==1)){    
					u1_printf("发送数据:0x%x\r\n",MQTT_TxDataOutPtr[2]);  //串口提示信息
					MQTT_TxData(MQTT_TxDataOutPtr);                       //发送数据
					MQTT_TxDataOutPtr += TBUFF_UNIT;                      //指针下移
					if(MQTT_TxDataOutPtr==MQTT_TxDataEndPtr)              //如果指针到缓冲区尾部了
						MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];            //指针归位到缓冲区开头
				} 				
			}//处理发送缓冲区数据的else if分支结尾
			
			/*-------------------------------------------------------------*/
			/*                     处理接收缓冲区数据                      */
			/*-------------------------------------------------------------*/
			if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr){  //if成立的话，说明接收缓冲区有数据了														
				u1_printf("接收到数据:");
				/*-----------------------------------------------------*/
				/*                    处理CONNACK报文                  */
				/*-----------------------------------------------------*/				
				//if判断，如果第一个字节是0x20，表示收到的是CONNACK报文
				//接着我们要判断第4个字节，看看CONNECT报文是否成功
				if(MQTT_RxDataOutPtr[2]==0x20){             			
				    switch(MQTT_RxDataOutPtr[5]){					
						case 0x00 : u1_printf("CONNECT报文成功\r\n");                            //串口输出信息	
								    ConnectPack_flag = 1;                                        //CONNECT报文成功，订阅报文可发
									break;                                                       //跳出分支case 0x00                                              
						case 0x01 : u1_printf("连接已拒绝，不支持的协议版本，准备重启\r\n");     //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接
									break;                                                       //跳出分支case 0x01   
						case 0x02 : u1_printf("连接已拒绝，不合格的客户端标识符，准备重启\r\n"); //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接
									break;                                                       //跳出分支case 0x02 
						case 0x03 : u1_printf("连接已拒绝，服务端不可用，准备重启\r\n");         //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接
									break;                                                       //跳出分支case 0x03
						case 0x04 : u1_printf("连接已拒绝，无效的用户名或密码，准备重启\r\n");   //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接						
									break;                                                       //跳出分支case 0x04
						case 0x05 : u1_printf("连接已拒绝，未授权，准备重启\r\n");               //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接						
									break;                                                       //跳出分支case 0x05 		
						default   : u1_printf("连接已拒绝，未知状态，准备重启\r\n");             //串口输出信息 
									Connect_flag = 0;                                            //Connect_flag置零，重启连接					
									break;                                                       //跳出分支case default 								
					}				
				}			
				//if判断，第一个字节是0x90，表示收到的是SUBACK报文
				//接着我们要判断订阅回复，看看是不是成功
				else if(MQTT_RxDataOutPtr[2]==0x90){ 
						switch(MQTT_RxDataOutPtr[6]){					
						case 0x00 :
						case 0x01 : u1_printf("订阅成功\r\n");            //串口输出信息
							        SubcribePack_flag = 1;                //SubcribePack_flag置1，表示订阅报文成功，其他报文可发送
									Ping_flag = 0;                        //Ping_flag清零
											TIM3_ENABLE_30S();                    //启动30s的PING定时器
								set_temp_humid();	//启动数据传输	
										TIM2_ENABLE_6S();	 //判断开关状态，并发布给服务器  定时器
									break;                                //跳出分支                                             
						default   : u1_printf("订阅失败，准备重启\r\n");  //串口输出信息 
									Connect_flag = 0;                     //Connect_flag置零，重启连接
									break;                                //跳出分支 								
					}					
				}
				//if判断，第一个字节是0xD0，表示收到的是PINGRESP报文
				else if(MQTT_RxDataOutPtr[2]==0xD0){ 
					u1_printf("PING报文回复\r\n"); 		  //串口输出信息 
					if(Ping_flag==1){                     //如果Ping_flag=1，表示第一次发送
						 Ping_flag = 0;    				  //要清除Ping_flag标志
					}else if(Ping_flag>1){ 				  //如果Ping_flag>1，表示是多次发送了，而且是2s间隔的快速发送
						Ping_flag = 0;     				  //要清除Ping_flag标志
						TIM3_ENABLE_30S(); 				  //PING定时器重回30s的时间
						
					}				
				}	
				//if判断，如果第一个字节是0x30，表示收到的是服务器发来的推送数据
				//我们要提取控制命令
				else if((MQTT_RxDataOutPtr[2]==0x30)){ 
					u1_printf("服务器等级0推送\r\n"); 		   //串口输出信息 
					MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);  //处理等级0推送数据
				}				
								  
				MQTT_RxDataOutPtr +=RBUFF_UNIT;                     //指针下移
				if(MQTT_RxDataOutPtr==MQTT_RxDataEndPtr)            //如果指针到缓冲区尾部了
					MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];          //指针归位到缓冲区开头                        
			}//处理接收缓冲区数据的else if分支结尾
			
			/*-------------------------------------------------------------*/
			/*                     处理命令缓冲区数据                      */
			/*-------------------------------------------------------------*/
			if(MQTT_CMDOutPtr != MQTT_CMDInPtr){                             //if成立的话，说明命令缓冲区有数据了			       
				u1_printf("命令:%s\r\n",&MQTT_CMDOutPtr[2]);                 //串口输出信息
				if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"LED\":1}")){      //云平台下发命令控制LED关
        LED_OFF;

        }else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"LED\":0}")){          //云平台下发命令控制LED开
        LED_ON;
				}
				MQTT_CMDOutPtr += CBUFF_UNIT;                             	 //指针下移
				if(MQTT_CMDOutPtr==MQTT_CMDEndPtr)           	             //如果指针到缓冲区尾部了
					MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	             //指针归位到缓冲区开头				
			}//处理命令缓冲区数据的else if分支结尾	
		}//Connect_flag=1的if分支的结尾
		
		/*--------------------------------------------------------------------*/
		/*      Connect_flag=0同服务器断开了连接,我们要重启连接服务器         */
		/*--------------------------------------------------------------------*/
		else{ 
			u1_printf("需要连接服务器\r\n");                 //串口输出信息
			TIM_Cmd(TIM4,DISABLE);                           //关闭TIM4 
			TIM_Cmd(TIM3,DISABLE); 			//关闭TIM3 
		
			WiFi_RxCounter=0;                                //WiFi接收数据量变量清零                        
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);          //清空WiFi接收缓冲区 
			if(WiFi_Connect_IoTServer()==0){   			     //如果WiFi连接云服务器函数返回0，表示正确，进入if
				u1_printf("建立TCP连接成功\r\n");            //串口输出信息
				Connect_flag = 1;                            //Connect_flag置1，表示连接成功	
				WiFi_RxCounter=0;                            //WiFi接收数据量变量清零                        
				memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);      //清空WiFi接收缓冲区 
				MQTT_Buff_Init();                            //初始化发送缓冲区                    
			}				
		}
		
	
	}
}


void set_temp_humid(void)
{
	char temp[256];  
	
	sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Humidity\":%d,\"temperature\":%d,\"Pressure\":%d,\"LightLux\":%d,\"gas\":%d,\"BodyTemp\":%.2f,\"Air_Quality\":%.d},\"version\":\"1.0.0\"}",(humiH*10+humiL),(tempH*10+tempL),pressure,lsens,gas,temp_data,Air_Quality);  //构建回复温湿度数据
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器	
} 
