/***********************************************************************************
* @file    main.c 
* @author  
* @version V1
* @date    
* @brief   
***********************************************************************************/

#include "stm32f4xx.h"
#include "Relay.h"
#include "oled.h"
#include "beep.h"
#include "key.h"
#include "timer.h"
#include "as608.h"
#include "delay.h"
#include "usart.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "bmp.h"
#include "led.h"
__IO char PwdIn_Buf[5] = {0}; //�洢��������
__IO int PwdIn_cnt = 0;        //�������������
__IO u8 PwdAlarmLimit = 0;     //�����ж�����������ٴ�������ʱ����
__IO u8 status=1;
//__IO u8 card_pydebuf[2];
//__IO u8 card_numberbuf[5];  //���һ���ֽ���У���ֽ�

extern __IO u32 box[5];
__IO int box_cnt=0;

#define MaxPwdSize 4     //������󳤶�
__IO char pwd_buf[5][MaxPwdSize];  //��Ϊ����Ľ��ջ�����  
__IO int  pwd_count = 0;				     	   //��Ϊ����Ľ��ռ�����
extern __IO u8 PwdAlarmLimit;            //�����ж�����������ٴ�������ʱ����



void DOOR_Open(int i)
{
	uint16_t GPIO_PIN;
	if(i==0)
	{
		GPIO_PIN = GPIO_Pin_11;
	}
	else if(i==1)
	{
		GPIO_PIN = GPIO_Pin_12;
	}
	else if(i==2)
	{
		GPIO_PIN = GPIO_Pin_13;
	}
	else if(i==3)
	{
		GPIO_PIN = GPIO_Pin_14;
	}
	else if(i==4)
	{
		GPIO_PIN = GPIO_Pin_15;
	}
	GPIO_SetBits(GPIOF, GPIO_PIN); //�ſ�
	GPIO_SetBits(GPIOF, GPIO_Pin_8); //��������
	delay_ms(200); //���ܳ���798
	GPIO_ResetBits(GPIOF, GPIO_Pin_8); //��������
	for(int j = 0 ; j<=8 ; j++)
	{
		delay_ms(500); //���ܳ���798
	}
	GPIO_ResetBits(GPIOF, GPIO_PIN); //�Ź�
	PwdAlarmLimit=0; //��λ����ֹ���󱨾�
	return;
}


//ȡ��
void PasswordInput(void)
{
		OLED_Clear();
		OLED_ShowCHinese(0,2,133);	   //���������� 
		OLED_ShowCHinese(16,2,134);
		OLED_ShowCHinese(32,2,135);
		OLED_ShowCHinese(48,2,136);
		OLED_ShowCHinese(64,2,137);
		u8 ClearFlag = 0; //������־
		//��ս��ջ�����
		memset((char *)PwdIn_Buf,0,16);
		//������ռ�����
		PwdIn_cnt = 0;

		// ��������Ϻ󽫰���״̬����Ϊ' '�������ظ�����ͬһ�������¼�
		KeyState = ' ';
		while(1)
		{
			get_key(); //��ȡ����״̬
			if(KeyState !=' ' && KeyState != '*' && KeyState != '#'&& KeyState != 'D') //���µļ���Ч ������Ҫ������ִ����Ӧ�Ĳ��� 
			{
				if(ClearFlag) //����
				{
					OLED_Clear();
					ClearFlag=0;
				}
				//������
				if(PwdIn_cnt < 5)
				{
					PwdIn_Buf[PwdIn_cnt++]=KeyState;
//					PwdIn_Buf[PwdIn_cnt]='\0';
					OLED_ShowString(80,2,(u8 *)PwdIn_Buf,16);
				}
				// ��������Ϻ󽫰���״̬����Ϊ' '�������ظ�����ͬһ�������¼�
				KeyState = ' ';
			}
			else if(KeyState == '*')  //*��ʾȷ�ϼ�
			{
//				for(int i = 0; i < 5;i++)
//				{
//					OLED_Clear();
//					OLED_ShowString(8,2,(u8 *)pwd_buf[i] ,16);
//					delay_ms(500); //���ܳ���798
//					delay_ms(500); //���ܳ���798
//					delay_ms(500); //���ܳ���798
//				}
				int f=0;
				for(int i = 0; i < 5;i++)
				{
					OLED_Clear();
					if(memcmp( (char *)PwdIn_Buf, (char *)( pwd_buf[i] ),4 ) == 0)
					{
						OLED_ShowCHinese(32,2,128);	   //��˳�����Ѵ� 
						OLED_ShowCHinese(48,2,87);
						OLED_ShowCHinese(64,2,88);
						OLED_ShowCHinese(80,2,89);
						OLED_ShowCHinese(32,4,90);	   //��˳�򣺻�ӭ���� 
						OLED_ShowCHinese(48,4,91);
						OLED_ShowCHinese(64,4,92);
						OLED_ShowCHinese(80,4,93);
//						GPIO_SetBits(GPIOF, GPIO_Pin_15); //�ſ�
//						GPIO_SetBits(GPIOF, GPIO_Pin_8); //��������
//						delay_ms(200); //���ܳ���798
//						GPIO_ResetBits(GPIOF, GPIO_Pin_8); //��������
//						for(int j = 0 ; j<=8 ; j++)
//						{
//							delay_ms(500); //���ܳ���798
//						}
//						GPIO_ResetBits(GPIOF, GPIO_Pin_11); //�Ź�
//						PwdAlarmLimit=0; //��λ����ֹ���󱨾�
						DOOR_Open(i);//�ſ�
						OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //��
								OLED_ShowCHinese(18,3,1);	       //��
								OLED_DrawBMP(46,1,81,7,BMP1);    //��ʾlogoͼƬ
								OLED_ShowCHinese(92,3,2);	       //��
								OLED_ShowCHinese(110,3,3);	     //��
						box[i]=0;
						f=1;
						break;
					}
				
				// ��������Ϻ󽫰���״̬����Ϊ' '�������ظ�����ͬһ�������¼�
				KeyState = ' ';
				
			}
				if(!f)
				{
					//�������
					OLED_ShowCHinese(32,3,103);	       
					OLED_ShowCHinese(48,3,81);	       
					OLED_ShowCHinese(64,3,9);	      
					OLED_ShowCHinese(80,3,10);	 
					PwdAlarmLimit++;
					if(PwdAlarmLimit==5) //����������������5�ξͱ���
					{
						GPIO_ResetBits(GPIOF,GPIO_Pin_9);
						for(int i=0;i<25;i++)
						{
							GPIO_SetBits(GPIOF, GPIO_Pin_8); //��������
							delay_ms(50); //���ܳ���798
							GPIO_ResetBits(GPIOF, GPIO_Pin_8); //��������
							delay_ms(50); //���ܳ���798
						}
						GPIO_SetBits(GPIOF,GPIO_Pin_9);
						PwdAlarmLimit=0; //��λ����ֹ���󱨾�
					}
					for(int i=0;i<4;i++)
					{
						delay_ms(500); //���ܳ���798
					}
					OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //��
								OLED_ShowCHinese(18,3,1);	       //��
								OLED_DrawBMP(46,1,81,7,BMP1);    //��ʾlogoͼƬ
								OLED_ShowCHinese(92,3,2);	       //��
								OLED_ShowCHinese(110,3,3);	     //��
				
					return;
				}
					
				break;
			}
			else if(KeyState == 'D')  //#��ʾ�����
			{
				OLED_Clear();
				//��ս��ջ�����
				memset((char *)PwdIn_cnt,0,16);
				//������ռ�����
				PwdIn_cnt = 0;
				// ��������Ϻ󽫰���״̬����Ϊ' '�������ظ�����ͬһ�������¼�
				KeyState = ' ';
				OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //��
								OLED_ShowCHinese(18,3,1);	       //��
								OLED_DrawBMP(46,1,81,7,BMP1);    //��ʾlogoͼƬ
								OLED_ShowCHinese(92,3,2);	       //��
								OLED_ShowCHinese(110,3,3);	     //��
				
				ClearFlag=1;
				return;
			}	
			else if(KeyState == '#')
			{
				KeyState = ' ';
				
				PwdIn_Buf[--PwdIn_cnt]=' ';
				
				OLED_ShowString(8,2,(u8 *)PwdIn_Buf,16);
			
			}
		}

}

//���
void Password_Create(void)
{
	char PwdIn_Buf[5]={0};
	int	PwdIn_cnt = 0;
	
	OLED_Clear();
	OLED_ShowCHinese(0,2,133);	   //���������� 
	OLED_ShowCHinese(16,2,134);
	OLED_ShowCHinese(32,2,135);
	OLED_ShowCHinese(48,2,136);
	OLED_ShowCHinese(64,2,137);
	u8 ClearFlag = 0; //������־
	
	// ��������Ϻ󽫰���״̬����Ϊ' '�������ظ�����ͬһ�������¼�
	KeyState = ' ';
	while(1)
	{
		get_key(); //��ȡ����״̬
		if(KeyState !=' ' && KeyState != '*' && KeyState != '#'&&KeyState!='D') //���µļ���Ч ������Ҫ������ִ����Ӧ�Ĳ��� 
		{
			if(ClearFlag) //����
			{
				OLED_Clear();
				ClearFlag=0;
			}
			//������
			if(PwdIn_cnt < 4)
			{
				PwdIn_Buf[PwdIn_cnt++]=KeyState;
//				PwdIn_Buf[PwdIn_cnt]='\0';
				OLED_ShowString(80,2,(u8 *)PwdIn_Buf,16);
			}
			// ��������Ϻ󽫰���״̬����Ϊ' '�������ظ�����ͬһ�������¼�
			KeyState = ' ';
		}
		else if(KeyState == '*'&&PwdIn_cnt==4)  //*��ʾȷ�ϼ�
		{
			for(int i = 0; i < 5; i++)
			{
				if(box[i]==0)
				{
					box[i]=1;
					OLED_Clear();
//					box_cnt = i;
					OLED_ShowCHinese(32,2,128);	   //��˳�����Ѵ� 
					OLED_ShowCHinese(48,2,87);
					OLED_ShowCHinese(64,2,88);
					OLED_ShowCHinese(80,2,89);
					OLED_ShowCHinese(32,4,90);	   //��˳�򣺻�ӭ���� 
					OLED_ShowCHinese(48,4,91);
					OLED_ShowCHinese(64,4,92);
					OLED_ShowCHinese(80,4,93);
//					GPIO_SetBits(GPIOF, GPIO_Pin_11); //�ſ�
//					GPIO_SetBits(GPIOF, GPIO_Pin_8); //��������
//					delay_ms(200); //���ܳ���798
//					GPIO_ResetBits(GPIOF, GPIO_Pin_8); //��������
//					for(int j = 0 ; j<=8 ; j++)
//					{
//						delay_ms(500); //���ܳ���798
//					}
//					GPIO_ResetBits(GPIOF, GPIO_Pin_11); //�Ź�
//					PwdAlarmLimit=0; //��λ����ֹ���󱨾�
					DOOR_Open(i);
					OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //��
								OLED_ShowCHinese(18,3,1);	       //��
								OLED_DrawBMP(46,1,81,7,BMP1);    //��ʾlogoͼƬ
								OLED_ShowCHinese(92,3,2);	       //��
								OLED_ShowCHinese(110,3,3);	     //��
					
					strcpy((char *)pwd_buf[i],PwdIn_Buf);
					
					return;
				}
			}
			OLED_Clear();
			// OLED�������������

			OLED_ShowCHinese(32,2,129);	   
			OLED_ShowCHinese(48,2,130);
			OLED_ShowCHinese(64,2,131);
			OLED_ShowCHinese(80,2,132);
			
			
			
			for(int j = 0 ; j<=8 ; j++)
			{
				delay_ms(500); //���ܳ���798
			}
			OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //��
								OLED_ShowCHinese(18,3,1);	       //��
								OLED_DrawBMP(46,1,81,7,BMP1);    //��ʾlogoͼƬ
								OLED_ShowCHinese(92,3,2);	       //��
								OLED_ShowCHinese(110,3,3);	     //��
			return;
		}
		else if(KeyState == 'D')  //#��ʾ�����
		{
				OLED_Clear();
				//��ս��ջ�����
				memset((char *)PwdIn_cnt,0,16);
				//������ռ�����
				PwdIn_cnt = 0;
				// ��������Ϻ󽫰���״̬����Ϊ' '�������ظ�����ͬһ�������¼�
				KeyState = ' ';
				OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //��
								OLED_ShowCHinese(18,3,1);	       //��
								OLED_DrawBMP(46,1,81,7,BMP1);    //��ʾlogoͼƬ
								OLED_ShowCHinese(92,3,2);	       //��
								OLED_ShowCHinese(110,3,3);	     //��
				
				ClearFlag=1;
		}	
		else if(KeyState == '*'&&PwdIn_cnt!=4)
		{
			OLED_Clear();
			//�������
			OLED_ShowCHinese(32,3,103);	       
			OLED_ShowCHinese(48,3,81);	       
			OLED_ShowCHinese(64,3,9);	      
			OLED_ShowCHinese(80,3,10);	 
			return;
		
		}
	}
	
	
}

//����Ա
void Administrator(void)
{
	u8 num[6]={0};
	int num_cnt = 0 ;
	//����������
	OLED_Clear();
//	OLED_ShowCHinese(0,2,133);	  
//	OLED_ShowCHinese(16,2,134);
//	OLED_ShowCHinese(32,2,135);
//	OLED_ShowCHinese(48,2,136);
//	OLED_ShowCHinese(64,2,137);
	
	u8 ClearFlag = 0; //������־
	// ��������Ϻ󽫰���״̬����Ϊ' '�������ظ�����ͬһ�������¼�
	KeyState = ' ';
	while(1)
	{
		get_key(); //��ȡ����״̬
		if(KeyState !=' ' && KeyState != '*' && KeyState != '#'&&KeyState!='D') //���µļ���Ч ������Ҫ������ִ����Ӧ�Ĳ��� 
		{
			if(ClearFlag) //����
			{
				OLED_Clear();
				ClearFlag=0;
			}
			//������
			if(num_cnt < 6)
			{
				num[num_cnt++]=KeyState;
//				PwdIn_Buf[PwdIn_cnt]='\0';
				OLED_ShowString(10,2,(u8 *)num,16);
			}
			// ��������Ϻ󽫰���״̬����Ϊ' '�������ظ�����ͬһ�������¼�
			KeyState = ' ';
		}
		else if(KeyState == '*')  //*��ʾȷ�ϼ�
			{
				int f=0;
				for(int i = 0; i < 5;i++)
				{
					OLED_Clear();
					if(memcmp( (char *)num, (char *)"123456",6 ) == 0)
					{
						u8 n[2]={0};//����Ҫ�򿪵Ĺ��ű��
						while(1)
						{
							KeyState = ' ';
							get_key(); //��ȡ����״̬
							if(KeyState !=' ' && KeyState != '*' && KeyState != '#'&&KeyState!='D') //���µļ���Ч ������Ҫ������ִ����Ӧ�Ĳ��� 
							{
								if(ClearFlag) //����
								{
									OLED_Clear();
									ClearFlag=0;
								}
								//������
								
								n[0] = KeyState;
								OLED_ShowString(10,2,n,16);
								
								// ��������Ϻ󽫰���״̬����Ϊ' '�������ظ�����ͬһ�������¼�
								KeyState = ' ';
							}
							else if(KeyState == '*')  //*��ʾȷ�ϼ�
							{
								f=1;
								OLED_Clear();
								OLED_ShowCHinese(32,2,128);	   //��˳�����Ѵ� 
								OLED_ShowCHinese(48,2,87);
								OLED_ShowCHinese(64,2,88);
								OLED_ShowCHinese(80,2,89);
								OLED_ShowCHinese(32,4,90);	   //��˳�򣺻�ӭ���� 
								OLED_ShowCHinese(48,4,91);
								OLED_ShowCHinese(64,4,92);
								OLED_ShowCHinese(80,4,93);
								
								DOOR_Open(atoi(n));//�ſ�
								
								OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //��
								OLED_ShowCHinese(18,3,1);	       //��
								OLED_DrawBMP(46,1,81,7,BMP1);    //��ʾlogoͼƬ
								OLED_ShowCHinese(92,3,2);	       //��
								OLED_ShowCHinese(110,3,3);	     //��
					
								box[atoi(n)]=0;
								return;
							}
							else if(KeyState == 'D')  //*��ʾ�˳���
							{
								OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //��
								OLED_ShowCHinese(18,3,1);	       //��
								OLED_DrawBMP(46,1,81,7,BMP1);    //��ʾlogoͼƬ
								OLED_ShowCHinese(92,3,2);	       //��
								OLED_ShowCHinese(110,3,3);	     //��
								return;
							}
						
						}
							
					
						break;
					}
				
				// ��������Ϻ󽫰���״̬����Ϊ' '�������ظ�����ͬһ�������¼�
				KeyState = ' ';
				
			}
				if(!f)
				{
					//�������
					OLED_ShowCHinese(32,3,103);	       
					OLED_ShowCHinese(48,3,81);	       
					OLED_ShowCHinese(64,3,9);	      
					OLED_ShowCHinese(80,3,10);	 
					PwdAlarmLimit++;
					if(PwdAlarmLimit==5) //����������������5�ξͱ���
					{
						GPIO_ResetBits(GPIOF,GPIO_Pin_9);
						for(int i=0;i<25;i++)
						{
							GPIO_SetBits(GPIOF, GPIO_Pin_8); //��������
							delay_ms(50); //���ܳ���798
							GPIO_ResetBits(GPIOF, GPIO_Pin_8); //��������
							delay_ms(50); //���ܳ���798
						}
						GPIO_SetBits(GPIOF,GPIO_Pin_9);
						PwdAlarmLimit=0; //��λ����ֹ���󱨾�
					}
					for(int i=0;i<4;i++)
					{
						delay_ms(500); //���ܳ���798
					}
					OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //��
								OLED_ShowCHinese(18,3,1);	       //��
								OLED_DrawBMP(46,1,81,7,BMP1);    //��ʾlogoͼƬ
								OLED_ShowCHinese(92,3,2);	       //��
								OLED_ShowCHinese(110,3,3);	     //��
				
					return;
				}
					
				break;
			}
			else if(KeyState == 'D')  //#��ʾ�����
		{
				OLED_Clear();
				//��ս��ջ�����
				memset((char *)PwdIn_cnt,0,16);
				//������ռ�����
				PwdIn_cnt = 0;
				// ��������Ϻ󽫰���״̬����Ϊ' '�������ظ�����ͬһ�������¼�
				KeyState = ' ';
			
								OLED_ShowCHinese(0,3,0);	       //��
								OLED_ShowCHinese(18,3,1);	       //��
								OLED_DrawBMP(46,1,81,7,BMP1);    //��ʾlogoͼƬ
								OLED_ShowCHinese(92,3,2);	       //��
								OLED_ShowCHinese(110,3,3);	     //��
				
				ClearFlag=1;
		}	
		else if(KeyState == '*'&&PwdIn_cnt!=4)
		{
			OLED_Clear();
			//�������
			OLED_ShowCHinese(32,3,103);	       
			OLED_ShowCHinese(48,3,81);	       
			OLED_ShowCHinese(64,3,9);	      
			OLED_ShowCHinese(80,3,10);	 
			return;
		
		}
		
		
		
	}
	

}



//�������
int main()
{
	//1.Ӳ����ʼ��
	uart1_init(115200);
	LED_Config();
	Relay_Config(); //�̵�����ʼ��
//	Beep_Config();  //��������ʼ��
	Key_Init(); //���̳�ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	//USART2�ĳ�ʼ��
	USART2_Config(57600);
	OLED_Init();		//��ʼ��OLED  
	OLED_Clear();  //����

	OLED_ShowCHinese(0,3,0);	       
	OLED_ShowCHinese(18,3,1);	       
	OLED_DrawBMP(46,1,81,7,BMP1);    //��ʾͼƬ
	OLED_ShowCHinese(92,3,2);	       //��
	OLED_ShowCHinese(110,3,3);	     //��
	KeyState = ' ';
  while(1)
	{
		get_key(); //��ȡ����״̬
		if(KeyState == 'A')
		{
			KeyState = ' ';
			//¼ָ�� 
			Add_FR();
		}
		else if(KeyState == 'B')
		{
			KeyState = ' ';
			//��������
			Password_Create();
		
		}
		else if(KeyState == 'C')
		{
			KeyState = ' ';
			//ˢָ��
			press_FR(); 
		}
		else if(KeyState == 'D')
		{
			KeyState = ' ';
			//��ȡ�������벢�ж��Ƿ���ȷ
			PasswordInput();
		}
		else if(KeyState == '1')
		{
			//�������Աģʽ
			
			Administrator();
			
		}


	}
}








