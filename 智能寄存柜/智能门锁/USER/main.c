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
__IO char PwdIn_Buf[5] = {0}; //存储开门密码
__IO int PwdIn_cnt = 0;        //开门密码计数器
__IO u8 PwdAlarmLimit = 0;     //用来判断连续输入多少错误密码时报警
__IO u8 status=1;
//__IO u8 card_pydebuf[2];
//__IO u8 card_numberbuf[5];  //最后一个字节是校验字节

extern __IO u32 box[5];
__IO int box_cnt=0;

#define MaxPwdSize 4     //密码最大长度
__IO char pwd_buf[5][MaxPwdSize];  //作为密码的接收缓冲区  
__IO int  pwd_count = 0;				     	   //作为密码的接收计数器
extern __IO u8 PwdAlarmLimit;            //用来判断连续输入多少错误密码时报警



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
	GPIO_SetBits(GPIOF, GPIO_PIN); //门开
	GPIO_SetBits(GPIOF, GPIO_Pin_8); //蜂鸣器响
	delay_ms(200); //不能超过798
	GPIO_ResetBits(GPIOF, GPIO_Pin_8); //蜂鸣器关
	for(int j = 0 ; j<=8 ; j++)
	{
		delay_ms(500); //不能超过798
	}
	GPIO_ResetBits(GPIOF, GPIO_PIN); //门关
	PwdAlarmLimit=0; //复位，防止错误报警
	return;
}


//取件
void PasswordInput(void)
{
		OLED_Clear();
		OLED_ShowCHinese(0,2,133);	   //请输入密码 
		OLED_ShowCHinese(16,2,134);
		OLED_ShowCHinese(32,2,135);
		OLED_ShowCHinese(48,2,136);
		OLED_ShowCHinese(64,2,137);
		u8 ClearFlag = 0; //清屏标志
		//清空接收缓冲区
		memset((char *)PwdIn_Buf,0,16);
		//清除接收计数器
		PwdIn_cnt = 0;

		// 处理按键完毕后将按键状态重置为' '，避免重复处理同一个按键事件
		KeyState = ' ';
		while(1)
		{
			get_key(); //获取按键状态
			if(KeyState !=' ' && KeyState != '*' && KeyState != '#'&& KeyState != 'D') //按下的键有效 根据需要在这里执行相应的操作 
			{
				if(ClearFlag) //清屏
				{
					OLED_Clear();
					ClearFlag=0;
				}
				//处理按键
				if(PwdIn_cnt < 5)
				{
					PwdIn_Buf[PwdIn_cnt++]=KeyState;
//					PwdIn_Buf[PwdIn_cnt]='\0';
					OLED_ShowString(80,2,(u8 *)PwdIn_Buf,16);
				}
				// 处理按键完毕后将按键状态重置为' '，避免重复处理同一个按键事件
				KeyState = ' ';
			}
			else if(KeyState == '*')  //*表示确认键
			{
//				for(int i = 0; i < 5;i++)
//				{
//					OLED_Clear();
//					OLED_ShowString(8,2,(u8 *)pwd_buf[i] ,16);
//					delay_ms(500); //不能超过798
//					delay_ms(500); //不能超过798
//					delay_ms(500); //不能超过798
//				}
				int f=0;
				for(int i = 0; i < 5;i++)
				{
					OLED_Clear();
					if(memcmp( (char *)PwdIn_Buf, (char *)( pwd_buf[i] ),4 ) == 0)
					{
						OLED_ShowCHinese(32,2,128);	   //按顺序：门已打开 
						OLED_ShowCHinese(48,2,87);
						OLED_ShowCHinese(64,2,88);
						OLED_ShowCHinese(80,2,89);
						OLED_ShowCHinese(32,4,90);	   //按顺序：欢迎光临 
						OLED_ShowCHinese(48,4,91);
						OLED_ShowCHinese(64,4,92);
						OLED_ShowCHinese(80,4,93);
//						GPIO_SetBits(GPIOF, GPIO_Pin_15); //门开
//						GPIO_SetBits(GPIOF, GPIO_Pin_8); //蜂鸣器响
//						delay_ms(200); //不能超过798
//						GPIO_ResetBits(GPIOF, GPIO_Pin_8); //蜂鸣器关
//						for(int j = 0 ; j<=8 ; j++)
//						{
//							delay_ms(500); //不能超过798
//						}
//						GPIO_ResetBits(GPIOF, GPIO_Pin_11); //门关
//						PwdAlarmLimit=0; //复位，防止错误报警
						DOOR_Open(i);//门开
						OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //智
								OLED_ShowCHinese(18,3,1);	       //能
								OLED_DrawBMP(46,1,81,7,BMP1);    //显示logo图片
								OLED_ShowCHinese(92,3,2);	       //寄
								OLED_ShowCHinese(110,3,3);	     //存
						box[i]=0;
						f=1;
						break;
					}
				
				// 处理按键完毕后将按键状态重置为' '，避免重复处理同一个按键事件
				KeyState = ' ';
				
			}
				if(!f)
				{
					//密码错误
					OLED_ShowCHinese(32,3,103);	       
					OLED_ShowCHinese(48,3,81);	       
					OLED_ShowCHinese(64,3,9);	      
					OLED_ShowCHinese(80,3,10);	 
					PwdAlarmLimit++;
					if(PwdAlarmLimit==5) //错误密码连续超过5次就报警
					{
						GPIO_ResetBits(GPIOF,GPIO_Pin_9);
						for(int i=0;i<25;i++)
						{
							GPIO_SetBits(GPIOF, GPIO_Pin_8); //蜂鸣器响
							delay_ms(50); //不能超过798
							GPIO_ResetBits(GPIOF, GPIO_Pin_8); //蜂鸣器关
							delay_ms(50); //不能超过798
						}
						GPIO_SetBits(GPIOF,GPIO_Pin_9);
						PwdAlarmLimit=0; //复位，防止错误报警
					}
					for(int i=0;i<4;i++)
					{
						delay_ms(500); //不能超过798
					}
					OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //智
								OLED_ShowCHinese(18,3,1);	       //能
								OLED_DrawBMP(46,1,81,7,BMP1);    //显示logo图片
								OLED_ShowCHinese(92,3,2);	       //寄
								OLED_ShowCHinese(110,3,3);	     //存
				
					return;
				}
					
				break;
			}
			else if(KeyState == 'D')  //#表示清除键
			{
				OLED_Clear();
				//清空接收缓冲区
				memset((char *)PwdIn_cnt,0,16);
				//清除接收计数器
				PwdIn_cnt = 0;
				// 处理按键完毕后将按键状态重置为' '，避免重复处理同一个按键事件
				KeyState = ' ';
				OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //智
								OLED_ShowCHinese(18,3,1);	       //能
								OLED_DrawBMP(46,1,81,7,BMP1);    //显示logo图片
								OLED_ShowCHinese(92,3,2);	       //寄
								OLED_ShowCHinese(110,3,3);	     //存
				
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

//存件
void Password_Create(void)
{
	char PwdIn_Buf[5]={0};
	int	PwdIn_cnt = 0;
	
	OLED_Clear();
	OLED_ShowCHinese(0,2,133);	   //请输入密码 
	OLED_ShowCHinese(16,2,134);
	OLED_ShowCHinese(32,2,135);
	OLED_ShowCHinese(48,2,136);
	OLED_ShowCHinese(64,2,137);
	u8 ClearFlag = 0; //清屏标志
	
	// 处理按键完毕后将按键状态重置为' '，避免重复处理同一个按键事件
	KeyState = ' ';
	while(1)
	{
		get_key(); //获取按键状态
		if(KeyState !=' ' && KeyState != '*' && KeyState != '#'&&KeyState!='D') //按下的键有效 根据需要在这里执行相应的操作 
		{
			if(ClearFlag) //清屏
			{
				OLED_Clear();
				ClearFlag=0;
			}
			//处理按键
			if(PwdIn_cnt < 4)
			{
				PwdIn_Buf[PwdIn_cnt++]=KeyState;
//				PwdIn_Buf[PwdIn_cnt]='\0';
				OLED_ShowString(80,2,(u8 *)PwdIn_Buf,16);
			}
			// 处理按键完毕后将按键状态重置为' '，避免重复处理同一个按键事件
			KeyState = ' ';
		}
		else if(KeyState == '*'&&PwdIn_cnt==4)  //*表示确认键
		{
			for(int i = 0; i < 5; i++)
			{
				if(box[i]==0)
				{
					box[i]=1;
					OLED_Clear();
//					box_cnt = i;
					OLED_ShowCHinese(32,2,128);	   //按顺序：门已打开 
					OLED_ShowCHinese(48,2,87);
					OLED_ShowCHinese(64,2,88);
					OLED_ShowCHinese(80,2,89);
					OLED_ShowCHinese(32,4,90);	   //按顺序：欢迎光临 
					OLED_ShowCHinese(48,4,91);
					OLED_ShowCHinese(64,4,92);
					OLED_ShowCHinese(80,4,93);
//					GPIO_SetBits(GPIOF, GPIO_Pin_11); //门开
//					GPIO_SetBits(GPIOF, GPIO_Pin_8); //蜂鸣器响
//					delay_ms(200); //不能超过798
//					GPIO_ResetBits(GPIOF, GPIO_Pin_8); //蜂鸣器关
//					for(int j = 0 ; j<=8 ; j++)
//					{
//						delay_ms(500); //不能超过798
//					}
//					GPIO_ResetBits(GPIOF, GPIO_Pin_11); //门关
//					PwdAlarmLimit=0; //复位，防止错误报警
					DOOR_Open(i);
					OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //智
								OLED_ShowCHinese(18,3,1);	       //能
								OLED_DrawBMP(46,1,81,7,BMP1);    //显示logo图片
								OLED_ShowCHinese(92,3,2);	       //寄
								OLED_ShowCHinese(110,3,3);	     //存
					
					strcpy((char *)pwd_buf[i],PwdIn_Buf);
					
					return;
				}
			}
			OLED_Clear();
			// OLED输出：柜子已满

			OLED_ShowCHinese(32,2,129);	   
			OLED_ShowCHinese(48,2,130);
			OLED_ShowCHinese(64,2,131);
			OLED_ShowCHinese(80,2,132);
			
			
			
			for(int j = 0 ; j<=8 ; j++)
			{
				delay_ms(500); //不能超过798
			}
			OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //智
								OLED_ShowCHinese(18,3,1);	       //能
								OLED_DrawBMP(46,1,81,7,BMP1);    //显示logo图片
								OLED_ShowCHinese(92,3,2);	       //寄
								OLED_ShowCHinese(110,3,3);	     //存
			return;
		}
		else if(KeyState == 'D')  //#表示清除键
		{
				OLED_Clear();
				//清空接收缓冲区
				memset((char *)PwdIn_cnt,0,16);
				//清除接收计数器
				PwdIn_cnt = 0;
				// 处理按键完毕后将按键状态重置为' '，避免重复处理同一个按键事件
				KeyState = ' ';
				OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //智
								OLED_ShowCHinese(18,3,1);	       //能
								OLED_DrawBMP(46,1,81,7,BMP1);    //显示logo图片
								OLED_ShowCHinese(92,3,2);	       //寄
								OLED_ShowCHinese(110,3,3);	     //存
				
				ClearFlag=1;
		}	
		else if(KeyState == '*'&&PwdIn_cnt!=4)
		{
			OLED_Clear();
			//密码错误
			OLED_ShowCHinese(32,3,103);	       
			OLED_ShowCHinese(48,3,81);	       
			OLED_ShowCHinese(64,3,9);	      
			OLED_ShowCHinese(80,3,10);	 
			return;
		
		}
	}
	
	
}

//管理员
void Administrator(void)
{
	u8 num[6]={0};
	int num_cnt = 0 ;
	//请输入密码
	OLED_Clear();
//	OLED_ShowCHinese(0,2,133);	  
//	OLED_ShowCHinese(16,2,134);
//	OLED_ShowCHinese(32,2,135);
//	OLED_ShowCHinese(48,2,136);
//	OLED_ShowCHinese(64,2,137);
	
	u8 ClearFlag = 0; //清屏标志
	// 处理按键完毕后将按键状态重置为' '，避免重复处理同一个按键事件
	KeyState = ' ';
	while(1)
	{
		get_key(); //获取按键状态
		if(KeyState !=' ' && KeyState != '*' && KeyState != '#'&&KeyState!='D') //按下的键有效 根据需要在这里执行相应的操作 
		{
			if(ClearFlag) //清屏
			{
				OLED_Clear();
				ClearFlag=0;
			}
			//处理按键
			if(num_cnt < 6)
			{
				num[num_cnt++]=KeyState;
//				PwdIn_Buf[PwdIn_cnt]='\0';
				OLED_ShowString(10,2,(u8 *)num,16);
			}
			// 处理按键完毕后将按键状态重置为' '，避免重复处理同一个按键事件
			KeyState = ' ';
		}
		else if(KeyState == '*')  //*表示确认键
			{
				int f=0;
				for(int i = 0; i < 5;i++)
				{
					OLED_Clear();
					if(memcmp( (char *)num, (char *)"123456",6 ) == 0)
					{
						u8 n[2]={0};//储存要打开的柜门编号
						while(1)
						{
							KeyState = ' ';
							get_key(); //获取按键状态
							if(KeyState !=' ' && KeyState != '*' && KeyState != '#'&&KeyState!='D') //按下的键有效 根据需要在这里执行相应的操作 
							{
								if(ClearFlag) //清屏
								{
									OLED_Clear();
									ClearFlag=0;
								}
								//处理按键
								
								n[0] = KeyState;
								OLED_ShowString(10,2,n,16);
								
								// 处理按键完毕后将按键状态重置为' '，避免重复处理同一个按键事件
								KeyState = ' ';
							}
							else if(KeyState == '*')  //*表示确认键
							{
								f=1;
								OLED_Clear();
								OLED_ShowCHinese(32,2,128);	   //按顺序：门已打开 
								OLED_ShowCHinese(48,2,87);
								OLED_ShowCHinese(64,2,88);
								OLED_ShowCHinese(80,2,89);
								OLED_ShowCHinese(32,4,90);	   //按顺序：欢迎光临 
								OLED_ShowCHinese(48,4,91);
								OLED_ShowCHinese(64,4,92);
								OLED_ShowCHinese(80,4,93);
								
								DOOR_Open(atoi(n));//门开
								
								OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //智
								OLED_ShowCHinese(18,3,1);	       //能
								OLED_DrawBMP(46,1,81,7,BMP1);    //显示logo图片
								OLED_ShowCHinese(92,3,2);	       //寄
								OLED_ShowCHinese(110,3,3);	     //存
					
								box[atoi(n)]=0;
								return;
							}
							else if(KeyState == 'D')  //*表示退出键
							{
								OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //智
								OLED_ShowCHinese(18,3,1);	       //能
								OLED_DrawBMP(46,1,81,7,BMP1);    //显示logo图片
								OLED_ShowCHinese(92,3,2);	       //寄
								OLED_ShowCHinese(110,3,3);	     //存
								return;
							}
						
						}
							
					
						break;
					}
				
				// 处理按键完毕后将按键状态重置为' '，避免重复处理同一个按键事件
				KeyState = ' ';
				
			}
				if(!f)
				{
					//密码错误
					OLED_ShowCHinese(32,3,103);	       
					OLED_ShowCHinese(48,3,81);	       
					OLED_ShowCHinese(64,3,9);	      
					OLED_ShowCHinese(80,3,10);	 
					PwdAlarmLimit++;
					if(PwdAlarmLimit==5) //错误密码连续超过5次就报警
					{
						GPIO_ResetBits(GPIOF,GPIO_Pin_9);
						for(int i=0;i<25;i++)
						{
							GPIO_SetBits(GPIOF, GPIO_Pin_8); //蜂鸣器响
							delay_ms(50); //不能超过798
							GPIO_ResetBits(GPIOF, GPIO_Pin_8); //蜂鸣器关
							delay_ms(50); //不能超过798
						}
						GPIO_SetBits(GPIOF,GPIO_Pin_9);
						PwdAlarmLimit=0; //复位，防止错误报警
					}
					for(int i=0;i<4;i++)
					{
						delay_ms(500); //不能超过798
					}
					OLED_Clear();
								OLED_ShowCHinese(0,3,0);	       //智
								OLED_ShowCHinese(18,3,1);	       //能
								OLED_DrawBMP(46,1,81,7,BMP1);    //显示logo图片
								OLED_ShowCHinese(92,3,2);	       //寄
								OLED_ShowCHinese(110,3,3);	     //存
				
					return;
				}
					
				break;
			}
			else if(KeyState == 'D')  //#表示清除键
		{
				OLED_Clear();
				//清空接收缓冲区
				memset((char *)PwdIn_cnt,0,16);
				//清除接收计数器
				PwdIn_cnt = 0;
				// 处理按键完毕后将按键状态重置为' '，避免重复处理同一个按键事件
				KeyState = ' ';
			
								OLED_ShowCHinese(0,3,0);	       //智
								OLED_ShowCHinese(18,3,1);	       //能
								OLED_DrawBMP(46,1,81,7,BMP1);    //显示logo图片
								OLED_ShowCHinese(92,3,2);	       //寄
								OLED_ShowCHinese(110,3,3);	     //存
				
				ClearFlag=1;
		}	
		else if(KeyState == '*'&&PwdIn_cnt!=4)
		{
			OLED_Clear();
			//密码错误
			OLED_ShowCHinese(32,3,103);	       
			OLED_ShowCHinese(48,3,81);	       
			OLED_ShowCHinese(64,3,9);	      
			OLED_ShowCHinese(80,3,10);	 
			return;
		
		}
		
		
		
	}
	

}



//程序入口
int main()
{
	//1.硬件初始化
	uart1_init(115200);
	LED_Config();
	Relay_Config(); //继电器初始化
//	Beep_Config();  //蜂鸣器初始化
	Key_Init(); //键盘初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	//USART2的初始化
	USART2_Config(57600);
	OLED_Init();		//初始化OLED  
	OLED_Clear();  //清屏

	OLED_ShowCHinese(0,3,0);	       
	OLED_ShowCHinese(18,3,1);	       
	OLED_DrawBMP(46,1,81,7,BMP1);    //显示图片
	OLED_ShowCHinese(92,3,2);	       //门
	OLED_ShowCHinese(110,3,3);	     //锁
	KeyState = ' ';
  while(1)
	{
		get_key(); //获取按键状态
		if(KeyState == 'A')
		{
			KeyState = ' ';
			//录指纹 
			Add_FR();
		}
		else if(KeyState == 'B')
		{
			KeyState = ' ';
			//创建密码
			Password_Create();
		
		}
		else if(KeyState == 'C')
		{
			KeyState = ' ';
			//刷指纹
			press_FR(); 
		}
		else if(KeyState == 'D')
		{
			KeyState = ' ';
			//读取输入密码并判断是否正确
			PasswordInput();
		}
		else if(KeyState == '1')
		{
			//进入管理员模式
			
			Administrator();
			
		}


	}
}








