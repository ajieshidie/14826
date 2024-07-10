#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "timer2.h"     //������Ҫ��ͷ�ļ�
#include "main.h"
#include "delay.h"      //������Ҫ��ͷ�ļ�
#include "usart1.h"     //������Ҫ��ͷ�ļ�
#include "usart2.h"     //������Ҫ��ͷ�ļ�
#include "timer1.h"     //������Ҫ��ͷ�ļ�
#include "timer3.h"     //������Ҫ��ͷ�ļ�
#include "timer4.h"     //������Ҫ��ͷ�ļ�
#include "wifi.h"	    //������Ҫ��ͷ�ļ�
#include "mqtt.h"       //������Ҫ��ͷ�ļ�
#include "dht11.h"
#include "timer5.h"     //������Ҫ��ͷ�ļ�
#include "math.h"
#include "led.h"
#include "OLED.h"
#include "AD.h"
#include "beep.h"
#include "relay.h"  
#include "DS18B20.h"
#include "max30102.h"


/*
  ѹ��     PA0
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
	//��һ����ʾ��ʪ��
	/*OLED_ShowCHinese(1,5,0);
	OLED_ShowCHinese(1,7,1);
	OLED_ShowCHinese(1,9,2);
	
	
	//�ڶ�����ʾ�¶�
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
	

	
	//��������ʾ�¶�
	OLED_ShowCHinese(3,1,1);
	OLED_ShowCHinese(3,3,2);
	OLED_ShowCHinese(3,5,3);
	OLED_ShowNum(3,7,humiH,1);
	OLED_ShowNum(3,8,humiL,1);
	
	//��������ʾ����
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
	
	usart1_init(115200);              //����1���ܳ�ʼ����������9600
	u1_printf("CONNECT���ĳɹ�\r\n"); 
	Usart2_Init(115200);            //����2���ܳ�ʼ����������115200	
	tim4_init(500,7200);            //TIM4��ʼ������ʱʱ�� 500*7200*1000/72000000 = 50ms
	delay_init();                   //��ʱ���ܳ�ʼ�� 
	wifi_reset_io_init();            //��ʼ��WiFi�ĸ�λIO
	AliIoT_Parameter_Init();	    //��ʼ�����Ӱ�����IoTƽ̨MQTT�������Ĳ���	
	DHT11_Init();
	AD_Init();
	LED_Init();
	BEEP_Init();
	OLED_Init();
	relay_Init();
	DS18B20_Init();
	
	while(1)                        //��ѭ��
	{ 			
		pressure =AD_Value[0]*1000/4095;
		lsens =100-(adc_1(10)*100/4095-100);
        gas=100-AD_Value[2]*1000/4095;
		Air_Quality=AD_Value[3]*1000/4095;
		
		DHT11_Read_Data(&humiH,&humiL,&tempH,&tempL);

		TEMP_Value_Conversion();
		
		oeld_show();


		
		/*--------------------------------------------------------------------*/
		/*   Connect_flag=1ͬ����������������,���ǿ��Է������ݺͽ���������    */
		/*--------------------------------------------------------------------*/
		if(Connect_flag==1){    
							
			/*-------------------------------------------------------------*/
			/*                     �����ͻ���������                      */
			/*-------------------------------------------------------------*/
				if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr){                //if�����Ļ���˵�����ͻ�������������
				//3������ɽ���if
				//��1�֣�0x10 ���ӱ���
				//��2�֣�0x82 ���ı��ģ���ConnectPack_flag��λ����ʾ���ӱ��ĳɹ�
				//��3�֣�SubcribePack_flag��λ��˵�����ӺͶ��ľ��ɹ����������Ŀɷ�
				if((MQTT_TxDataOutPtr[2]==0x10)||((MQTT_TxDataOutPtr[2]==0x82)&&(ConnectPack_flag==1))||(SubcribePack_flag==1)){    
					u1_printf("��������:0x%x\r\n",MQTT_TxDataOutPtr[2]);  //������ʾ��Ϣ
					MQTT_TxData(MQTT_TxDataOutPtr);                       //��������
					MQTT_TxDataOutPtr += TBUFF_UNIT;                      //ָ������
					if(MQTT_TxDataOutPtr==MQTT_TxDataEndPtr)              //���ָ�뵽������β����
						MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];            //ָ���λ����������ͷ
				} 				
			}//�����ͻ��������ݵ�else if��֧��β
			
			/*-------------------------------------------------------------*/
			/*                     ������ջ���������                      */
			/*-------------------------------------------------------------*/
			if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr){  //if�����Ļ���˵�����ջ�������������														
				u1_printf("���յ�����:");
				/*-----------------------------------------------------*/
				/*                    ����CONNACK����                  */
				/*-----------------------------------------------------*/				
				//if�жϣ������һ���ֽ���0x20����ʾ�յ�����CONNACK����
				//��������Ҫ�жϵ�4���ֽڣ�����CONNECT�����Ƿ�ɹ�
				if(MQTT_RxDataOutPtr[2]==0x20){             			
				    switch(MQTT_RxDataOutPtr[5]){					
						case 0x00 : u1_printf("CONNECT���ĳɹ�\r\n");                            //���������Ϣ	
								    ConnectPack_flag = 1;                                        //CONNECT���ĳɹ������ı��Ŀɷ�
									break;                                                       //������֧case 0x00                                              
						case 0x01 : u1_printf("�����Ѿܾ�����֧�ֵ�Э��汾��׼������\r\n");     //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������
									break;                                                       //������֧case 0x01   
						case 0x02 : u1_printf("�����Ѿܾ������ϸ�Ŀͻ��˱�ʶ����׼������\r\n"); //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������
									break;                                                       //������֧case 0x02 
						case 0x03 : u1_printf("�����Ѿܾ�������˲����ã�׼������\r\n");         //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������
									break;                                                       //������֧case 0x03
						case 0x04 : u1_printf("�����Ѿܾ�����Ч���û��������룬׼������\r\n");   //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������						
									break;                                                       //������֧case 0x04
						case 0x05 : u1_printf("�����Ѿܾ���δ��Ȩ��׼������\r\n");               //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������						
									break;                                                       //������֧case 0x05 		
						default   : u1_printf("�����Ѿܾ���δ֪״̬��׼������\r\n");             //���������Ϣ 
									Connect_flag = 0;                                            //Connect_flag���㣬��������					
									break;                                                       //������֧case default 								
					}				
				}			
				//if�жϣ���һ���ֽ���0x90����ʾ�յ�����SUBACK����
				//��������Ҫ�ж϶��Ļظ��������ǲ��ǳɹ�
				else if(MQTT_RxDataOutPtr[2]==0x90){ 
						switch(MQTT_RxDataOutPtr[6]){					
						case 0x00 :
						case 0x01 : u1_printf("���ĳɹ�\r\n");            //���������Ϣ
							        SubcribePack_flag = 1;                //SubcribePack_flag��1����ʾ���ı��ĳɹ����������Ŀɷ���
									Ping_flag = 0;                        //Ping_flag����
											TIM3_ENABLE_30S();                    //����30s��PING��ʱ��
								set_temp_humid();	//�������ݴ���	
										TIM2_ENABLE_6S();	 //�жϿ���״̬����������������  ��ʱ��
									break;                                //������֧                                             
						default   : u1_printf("����ʧ�ܣ�׼������\r\n");  //���������Ϣ 
									Connect_flag = 0;                     //Connect_flag���㣬��������
									break;                                //������֧ 								
					}					
				}
				//if�жϣ���һ���ֽ���0xD0����ʾ�յ�����PINGRESP����
				else if(MQTT_RxDataOutPtr[2]==0xD0){ 
					u1_printf("PING���Ļظ�\r\n"); 		  //���������Ϣ 
					if(Ping_flag==1){                     //���Ping_flag=1����ʾ��һ�η���
						 Ping_flag = 0;    				  //Ҫ���Ping_flag��־
					}else if(Ping_flag>1){ 				  //���Ping_flag>1����ʾ�Ƕ�η����ˣ�������2s����Ŀ��ٷ���
						Ping_flag = 0;     				  //Ҫ���Ping_flag��־
						TIM3_ENABLE_30S(); 				  //PING��ʱ���ػ�30s��ʱ��
						
					}				
				}	
				//if�жϣ������һ���ֽ���0x30����ʾ�յ����Ƿ�������������������
				//����Ҫ��ȡ��������
				else if((MQTT_RxDataOutPtr[2]==0x30)){ 
					u1_printf("�������ȼ�0����\r\n"); 		   //���������Ϣ 
					MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);  //����ȼ�0��������
				}				
								  
				MQTT_RxDataOutPtr +=RBUFF_UNIT;                     //ָ������
				if(MQTT_RxDataOutPtr==MQTT_RxDataEndPtr)            //���ָ�뵽������β����
					MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];          //ָ���λ����������ͷ                        
			}//������ջ��������ݵ�else if��֧��β
			
			/*-------------------------------------------------------------*/
			/*                     ���������������                      */
			/*-------------------------------------------------------------*/
			if(MQTT_CMDOutPtr != MQTT_CMDInPtr){                             //if�����Ļ���˵�����������������			       
				u1_printf("����:%s\r\n",&MQTT_CMDOutPtr[2]);                 //���������Ϣ
				if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"LED\":1}")){      //��ƽ̨�·��������LED��
        LED_OFF;

        }else if(strstr((char *)MQTT_CMDOutPtr+2,"\"params\":{\"LED\":0}")){          //��ƽ̨�·��������LED��
        LED_ON;
				}
				MQTT_CMDOutPtr += CBUFF_UNIT;                             	 //ָ������
				if(MQTT_CMDOutPtr==MQTT_CMDEndPtr)           	             //���ָ�뵽������β����
					MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	             //ָ���λ����������ͷ				
			}//��������������ݵ�else if��֧��β	
		}//Connect_flag=1��if��֧�Ľ�β
		
		/*--------------------------------------------------------------------*/
		/*      Connect_flag=0ͬ�������Ͽ�������,����Ҫ�������ӷ�����         */
		/*--------------------------------------------------------------------*/
		else{ 
			u1_printf("��Ҫ���ӷ�����\r\n");                 //���������Ϣ
			TIM_Cmd(TIM4,DISABLE);                           //�ر�TIM4 
			TIM_Cmd(TIM3,DISABLE); 			//�ر�TIM3 
		
			WiFi_RxCounter=0;                                //WiFi������������������                        
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);          //���WiFi���ջ����� 
			if(WiFi_Connect_IoTServer()==0){   			     //���WiFi�����Ʒ�������������0����ʾ��ȷ������if
				u1_printf("����TCP���ӳɹ�\r\n");            //���������Ϣ
				Connect_flag = 1;                            //Connect_flag��1����ʾ���ӳɹ�	
				WiFi_RxCounter=0;                            //WiFi������������������                        
				memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);      //���WiFi���ջ����� 
				MQTT_Buff_Init();                            //��ʼ�����ͻ�����                    
			}				
		}
		
	
	}
}


void set_temp_humid(void)
{
	char temp[256];  
	
	sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Humidity\":%d,\"temperature\":%d,\"Pressure\":%d,\"LightLux\":%d,\"gas\":%d,\"BodyTemp\":%.2f,\"Air_Quality\":%.d},\"version\":\"1.0.0\"}",(humiH*10+humiL),(tempH*10+tempL),pressure,lsens,gas,temp_data,Air_Quality);  //�����ظ���ʪ������
	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������	
} 
