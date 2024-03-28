#include "stm32f4xx.h"
#include "bluetooth.h"
#include "delay.h"
#include "usart.h"
#include "key.h"
#include "oled.h"
#include "as608.h"
#include <string.h>

#define  BLE_BUFFERSIZE  256						       //���ջ������Ĵ�С
static __IO char ble_buffer[BLE_BUFFERSIZE];   //��Ϊ����ģ��Ľ��ջ�����
static __IO int  ble_cnt=0;										 //��Ϊ����ģ��Ľ��ռ�����
static __IO uint8_t ble_recvflag = 0;			  	 //��Ϊ����ģ��Ľ��ձ�־λ

__IO char pwd_buf[5][MaxPwdSize];  //��Ϊ����Ľ��ջ�����  Ĭ������123456
__IO int  pwd_count = 0;				     	   //��Ϊ����Ľ��ռ�����
extern __IO u8 PwdAlarmLimit;            //�����ж�����������ٴ�������ʱ����
__IO char card_id[50][20]={{0}};         //�����洢50�ſ��ţ��������Ϊ50λ
__IO u8 card_cnt = 0;                    //�ſ�¼�������
extern unsigned char BMP1[];


//������������
void BLE_Config(u32 baud)
{
	USART3_Config(baud);
	
	Usart_SendString( USART3, "AT+NAMEWGLWGL\r\n");   //�޸���������
	delay_ms(10);
//	USART3_SendString("AT+BAUD4\r\n"); //�������������� 9600
//	delay_ms(10);
	Usart_SendString( USART3, "AT+RESET\r\n"); 				//����������Ч
	delay_ms(10);
}

//�޸�����
//void ModPwd(void)
//{
//	//��ս��ջ�����
//	memset((char *)pwd_buf,0,MaxPwdSize);
//	//������ռ�����
//	pwd_count = 0;
//	OLED_Clear();
//	OLED_ShowCHinese(0,2,99);	   //��������Ҫ�޸ĵ�����
//	OLED_ShowCHinese(16,2,104);
//	OLED_ShowCHinese(32,2,20);
//	OLED_ShowCHinese(48,2,105);
//	OLED_ShowCHinese(64,2,106);
//	OLED_ShowCHinese(80,2,107);
//	OLED_ShowCHinese(96,2,108);
//	OLED_ShowCHinese(112,2,109);
//	OLED_ShowCHinese(0,4,103);
//	OLED_ShowCHinese(16,4,81);
//	OLED_ShowString(0,6,"#",16);
//	OLED_ShowCHinese(8,6,110); //���
//	OLED_ShowCHinese(24,6,111); 
//	OLED_ShowString(40,6,"*",111);
//	OLED_ShowCHinese(56,6,97); //ȷ��
//	OLED_ShowCHinese(72,6,98);
//	u8 ClearFlag = 1;
//	while(1)
//	{
//		get_key(); //��ȡ����״̬
//		if(KeyState !=' ' && KeyState != '*' && KeyState != '#') //���µļ���Ч ������Ҫ������ִ����Ӧ�Ĳ��� 
//		{
//			if(ClearFlag) //����
//			{
//				OLED_Clear();
//				ClearFlag=0;
//			}
//			//������
//			if(pwd_count < MaxPwdSize)
//			{
//				pwd_buf[pwd_count++]=KeyState;
//				pwd_buf[pwd_count]='\0';
//				OLED_ShowString(8,2,(u8 *)pwd_buf,16);
//			}
//			// ��������Ϻ󽫰���״̬����Ϊ' '�������ظ�����ͬһ�������¼�
//			KeyState = ' ';
//		}
//		else if(KeyState == '*')  //*��ʾȷ�ϼ�
//		{
//			OLED_Clear();
//			OLED_ShowCHinese(16,3,103);
//			OLED_ShowCHinese(32,3,81);
//			OLED_ShowCHinese(48,3,107);
//			OLED_ShowCHinese(64,3,108);
//			OLED_ShowCHinese(80,3,95);
//			OLED_ShowCHinese(96,3,96);
//			delay_ms(500); //���ܳ���798
//			delay_ms(500); //���ܳ���798
//			OLED_Clear();
//			OLED_ShowCHinese(0,3,0);	       //С
//			OLED_ShowCHinese(18,3,1);	       //��
//			OLED_DrawBMP(46,1,81,7,BMP1);    //��ʾlogoͼƬ
//			OLED_ShowCHinese(92,3,2);	       //��
//			OLED_ShowCHinese(110,3,3);	     //��
//			// ��������Ϻ󽫰���״̬����Ϊ' '�������ظ�����ͬһ�������¼�
//			KeyState = ' ';
//			break;
//		}
//		else if(KeyState == '#')  //#��ʾ�����
//		{
//			OLED_Clear();
//			//��ս��ջ�����
//			memset((char *)pwd_buf,0,MaxPwdSize);
//			//������ռ�����
//			pwd_count = 0;
//			// ��������Ϻ󽫰���״̬����Ϊ' '�������ظ�����ͬһ�������¼�
//			KeyState = ' ';
//		}	
//	}		
//}





//�ж������Ƿ���յ����ݲ����д���
void BlueRecvProcessing(void)
{
	if( 1 == ble_recvflag )
	{
		
		//������ջ�����
		if(strstr((char *)ble_buffer,"OpenDoor#")!=NULL)
		{
			OLED_Clear(); 	//����
			OLED_ShowCHinese(32,2,2);	   //��˳�����Ѵ� 
			OLED_ShowCHinese(48,2,87);
			OLED_ShowCHinese(64,2,88);
			OLED_ShowCHinese(80,2,89);
			OLED_ShowCHinese(32,4,90);	   //��˳�򣺻�ӭ���� 
			OLED_ShowCHinese(48,4,91);
			OLED_ShowCHinese(64,4,92);
			OLED_ShowCHinese(80,4,93);
			GPIO_SetBits(GPIOF, GPIO_Pin_15); //�ſ�
			GPIO_SetBits(GPIOF, GPIO_Pin_8); //��������
			delay_ms(200); //���ܳ���798
			GPIO_ResetBits(GPIOF, GPIO_Pin_8); //��������
			for(int i = 0 ; i<=8 ; i++)
			{
				delay_ms(500); //���ܳ���798
			}
			GPIO_ResetBits(GPIOF, GPIO_Pin_15); //�Ź�
			PwdAlarmLimit=0; //��λ����ֹ���󱨾�
			OLED_Clear(); 	//����
			OLED_ShowCHinese(0,3,0);	       //С
			OLED_ShowCHinese(18,3,1);	       //��
			OLED_DrawBMP(46,1,81,7,BMP1);    //��ʾlogoͼƬ
			OLED_ShowCHinese(92,3,2);	       //��
			OLED_ShowCHinese(110,3,3);	     //��
		}
		else if(strstr((char *)ble_buffer,"ModPwd#")!=NULL)
		{
			ModPwd(); //�޸����뺯��
			PwdAlarmLimit=0; //��λ����ֹ���󱨾�
		}
		else if(strstr((char *)ble_buffer,"Add_FR#")!=NULL)
		{ 
			//¼ָ��
			Add_FR();
			PwdAlarmLimit=0; //��λ����ֹ���󱨾�
			OLED_Clear(); 	//����
			OLED_ShowCHinese(0,3,0);	       //С
			OLED_ShowCHinese(18,3,1);	       //��
			OLED_DrawBMP(46,1,81,7,BMP1);    //��ʾlogoͼƬ
			OLED_ShowCHinese(92,3,2);	       //��
			OLED_ShowCHinese(110,3,3);	     //��
			
		}
		else if(strstr((char *)ble_buffer,"Del_FR#")!=NULL)
		{ 
			//ɾ��ָ��
			Del_FR();
			PwdAlarmLimit=0; //��λ����ֹ���󱨾�
			OLED_Clear(); 	//����
			OLED_ShowCHinese(0,3,0);	       //С
			OLED_ShowCHinese(18,3,1);	       //��
			OLED_DrawBMP(46,1,81,7,BMP1);    //��ʾlogoͼƬ
			OLED_ShowCHinese(92,3,2);	       //��
			OLED_ShowCHinese(110,3,3);	     //��
			
		}
		else
		{
			PwdAlarmLimit++;
			if(PwdAlarmLimit==8) //����������������10�ξͱ���
			{
				for(int i=0;i<25;i++)
				{
					GPIO_SetBits(GPIOF, GPIO_Pin_8); //��������
					delay_ms(50); //���ܳ���798
					GPIO_ResetBits(GPIOF, GPIO_Pin_8); //��������
					delay_ms(50); //���ܳ���798
				}
				PwdAlarmLimit=0; //��λ����ֹ���󱨾�
			}
		}
			
		
		//������ձ�־λ
		ble_recvflag = 0;
		
		//��ս��ջ�����
		memset((char *)ble_buffer,0,BLE_BUFFERSIZE);
		
		//������ռ�����
		ble_cnt = 0;
		
	}
}

//�жϷ����� ������������
void USART3_IRQHandler(void)
{
	
	//�ж��ж��Ƿ񴥷�
	if( USART_GetITStatus(USART3, USART_IT_RXNE) == SET )
	{
		//�жϽ��ջ������Ƿ�����
		if(ble_cnt < BLE_BUFFERSIZE)
		{
			ble_buffer[ble_cnt++] = USART_ReceiveData(USART3); //һ��ֻ�ܽ���1���ֽ�
			
			//˵�����ݽ������
			if(ble_buffer[ble_cnt-1] == '#')
			{
				ble_recvflag = 1;
			}
		}
		
	}
}



