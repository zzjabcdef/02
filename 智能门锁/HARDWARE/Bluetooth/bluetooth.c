#include "stm32f4xx.h"
#include "bluetooth.h"
#include "delay.h"
#include "usart.h"
#include "key.h"
#include "oled.h"
#include "as608.h"
#include <string.h>

#define  BLE_BUFFERSIZE  256						       //接收缓冲区的大小
static __IO char ble_buffer[BLE_BUFFERSIZE];   //作为蓝牙模块的接收缓冲区
static __IO int  ble_cnt=0;										 //作为蓝牙模块的接收计数器
static __IO uint8_t ble_recvflag = 0;			  	 //作为蓝牙模块的接收标志位

__IO char pwd_buf[5][MaxPwdSize];  //作为密码的接收缓冲区  默认密码123456
__IO int  pwd_count = 0;				     	   //作为密码的接收计数器
extern __IO u8 PwdAlarmLimit;            //用来判断连续输入多少错误密码时报警
__IO char card_id[50][20]={{0}};         //用来存储50张卡号，长度最大为50位
__IO u8 card_cnt = 0;                    //门卡录入计数器
extern unsigned char BMP1[];


//配置蓝牙参数
void BLE_Config(u32 baud)
{
	USART3_Config(baud);
	
	Usart_SendString( USART3, "AT+NAMEWGLWGL\r\n");   //修改蓝牙名称
	delay_ms(10);
//	USART3_SendString("AT+BAUD4\r\n"); //设置蓝牙波特率 9600
//	delay_ms(10);
	Usart_SendString( USART3, "AT+RESET\r\n"); 				//参数重启生效
	delay_ms(10);
}

//修改密码
//void ModPwd(void)
//{
//	//清空接收缓冲区
//	memset((char *)pwd_buf,0,MaxPwdSize);
//	//清除接收计数器
//	pwd_count = 0;
//	OLED_Clear();
//	OLED_ShowCHinese(0,2,99);	   //请输入您要修改的密码
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
//	OLED_ShowCHinese(8,6,110); //清空
//	OLED_ShowCHinese(24,6,111); 
//	OLED_ShowString(40,6,"*",111);
//	OLED_ShowCHinese(56,6,97); //确认
//	OLED_ShowCHinese(72,6,98);
//	u8 ClearFlag = 1;
//	while(1)
//	{
//		get_key(); //获取按键状态
//		if(KeyState !=' ' && KeyState != '*' && KeyState != '#') //按下的键有效 根据需要在这里执行相应的操作 
//		{
//			if(ClearFlag) //清屏
//			{
//				OLED_Clear();
//				ClearFlag=0;
//			}
//			//处理按键
//			if(pwd_count < MaxPwdSize)
//			{
//				pwd_buf[pwd_count++]=KeyState;
//				pwd_buf[pwd_count]='\0';
//				OLED_ShowString(8,2,(u8 *)pwd_buf,16);
//			}
//			// 处理按键完毕后将按键状态重置为' '，避免重复处理同一个按键事件
//			KeyState = ' ';
//		}
//		else if(KeyState == '*')  //*表示确认键
//		{
//			OLED_Clear();
//			OLED_ShowCHinese(16,3,103);
//			OLED_ShowCHinese(32,3,81);
//			OLED_ShowCHinese(48,3,107);
//			OLED_ShowCHinese(64,3,108);
//			OLED_ShowCHinese(80,3,95);
//			OLED_ShowCHinese(96,3,96);
//			delay_ms(500); //不能超过798
//			delay_ms(500); //不能超过798
//			OLED_Clear();
//			OLED_ShowCHinese(0,3,0);	       //小
//			OLED_ShowCHinese(18,3,1);	       //亮
//			OLED_DrawBMP(46,1,81,7,BMP1);    //显示logo图片
//			OLED_ShowCHinese(92,3,2);	       //门
//			OLED_ShowCHinese(110,3,3);	     //锁
//			// 处理按键完毕后将按键状态重置为' '，避免重复处理同一个按键事件
//			KeyState = ' ';
//			break;
//		}
//		else if(KeyState == '#')  //#表示清除键
//		{
//			OLED_Clear();
//			//清空接收缓冲区
//			memset((char *)pwd_buf,0,MaxPwdSize);
//			//清除接收计数器
//			pwd_count = 0;
//			// 处理按键完毕后将按键状态重置为' '，避免重复处理同一个按键事件
//			KeyState = ' ';
//		}	
//	}		
//}





//判断蓝牙是否接收到数据并进行处理
void BlueRecvProcessing(void)
{
	if( 1 == ble_recvflag )
	{
		
		//处理接收缓冲区
		if(strstr((char *)ble_buffer,"OpenDoor#")!=NULL)
		{
			OLED_Clear(); 	//清屏
			OLED_ShowCHinese(32,2,2);	   //按顺序：门已打开 
			OLED_ShowCHinese(48,2,87);
			OLED_ShowCHinese(64,2,88);
			OLED_ShowCHinese(80,2,89);
			OLED_ShowCHinese(32,4,90);	   //按顺序：欢迎光临 
			OLED_ShowCHinese(48,4,91);
			OLED_ShowCHinese(64,4,92);
			OLED_ShowCHinese(80,4,93);
			GPIO_SetBits(GPIOF, GPIO_Pin_15); //门开
			GPIO_SetBits(GPIOF, GPIO_Pin_8); //蜂鸣器响
			delay_ms(200); //不能超过798
			GPIO_ResetBits(GPIOF, GPIO_Pin_8); //蜂鸣器关
			for(int i = 0 ; i<=8 ; i++)
			{
				delay_ms(500); //不能超过798
			}
			GPIO_ResetBits(GPIOF, GPIO_Pin_15); //门关
			PwdAlarmLimit=0; //复位，防止错误报警
			OLED_Clear(); 	//清屏
			OLED_ShowCHinese(0,3,0);	       //小
			OLED_ShowCHinese(18,3,1);	       //亮
			OLED_DrawBMP(46,1,81,7,BMP1);    //显示logo图片
			OLED_ShowCHinese(92,3,2);	       //门
			OLED_ShowCHinese(110,3,3);	     //锁
		}
		else if(strstr((char *)ble_buffer,"ModPwd#")!=NULL)
		{
			ModPwd(); //修改密码函数
			PwdAlarmLimit=0; //复位，防止错误报警
		}
		else if(strstr((char *)ble_buffer,"Add_FR#")!=NULL)
		{ 
			//录指纹
			Add_FR();
			PwdAlarmLimit=0; //复位，防止错误报警
			OLED_Clear(); 	//清屏
			OLED_ShowCHinese(0,3,0);	       //小
			OLED_ShowCHinese(18,3,1);	       //亮
			OLED_DrawBMP(46,1,81,7,BMP1);    //显示logo图片
			OLED_ShowCHinese(92,3,2);	       //门
			OLED_ShowCHinese(110,3,3);	     //锁
			
		}
		else if(strstr((char *)ble_buffer,"Del_FR#")!=NULL)
		{ 
			//删除指纹
			Del_FR();
			PwdAlarmLimit=0; //复位，防止错误报警
			OLED_Clear(); 	//清屏
			OLED_ShowCHinese(0,3,0);	       //小
			OLED_ShowCHinese(18,3,1);	       //亮
			OLED_DrawBMP(46,1,81,7,BMP1);    //显示logo图片
			OLED_ShowCHinese(92,3,2);	       //门
			OLED_ShowCHinese(110,3,3);	     //锁
			
		}
		else
		{
			PwdAlarmLimit++;
			if(PwdAlarmLimit==8) //错误密码连续超过10次就报警
			{
				for(int i=0;i<25;i++)
				{
					GPIO_SetBits(GPIOF, GPIO_Pin_8); //蜂鸣器响
					delay_ms(50); //不能超过798
					GPIO_ResetBits(GPIOF, GPIO_Pin_8); //蜂鸣器关
					delay_ms(50); //不能超过798
				}
				PwdAlarmLimit=0; //复位，防止错误报警
			}
		}
			
		
		//清除接收标志位
		ble_recvflag = 0;
		
		//清空接收缓冲区
		memset((char *)ble_buffer,0,BLE_BUFFERSIZE);
		
		//清除接收计数器
		ble_cnt = 0;
		
	}
}

//中断服务函数 接收蓝牙数据
void USART3_IRQHandler(void)
{
	
	//判断中断是否触发
	if( USART_GetITStatus(USART3, USART_IT_RXNE) == SET )
	{
		//判断接收缓冲区是否满了
		if(ble_cnt < BLE_BUFFERSIZE)
		{
			ble_buffer[ble_cnt++] = USART_ReceiveData(USART3); //一次只能接收1个字节
			
			//说明数据接收完成
			if(ble_buffer[ble_cnt-1] == '#')
			{
				ble_recvflag = 1;
			}
		}
		
	}
}



