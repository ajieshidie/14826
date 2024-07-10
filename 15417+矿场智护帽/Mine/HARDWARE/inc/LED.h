/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*              ʵ��LED���ܵ�ͷ�ļ�                */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __LED_H
#define __LED_H
#define LED0 PBout(0)


#define LED_OUT(x)   GPIO_WriteBit(GPIOB, GPIO_Pin_0, (BitAction)x)  //����PB0 �ĵ�ƽ�����Ե���Ϩ��LED3

#define LED_IN_STA   GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_0)  //PB0 ����LED3,��ȡ��ƽ״̬�������ж�LED3�ǵ�������Ϩ��

#define LED_ON       GPIO_ResetBits(GPIOB, GPIO_Pin_0)        //������������PB0��ƽ������LED3
#define LED_OFF      GPIO_SetBits(GPIOB, GPIO_Pin_0)          //������������PB0��ƽ��Ϩ��LED3



void LED_Init(void);               //��ʼ��	


#endif
