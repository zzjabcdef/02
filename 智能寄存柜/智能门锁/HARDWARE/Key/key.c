/*********************************************************************************
**************************STM32F407核心应用板*************************************
**********************************************************************************
* 文件名称: keyboard.c                                                    		 *
* 文件简述：4*4按键键盘输出		                                                 *
*    																			 *
*  												                              	 *
**********************************************************************************
*********************************************************************************/	
#include "key.h"

char KeyState = ' ';

__IO u8 keyflag1,keyflag2,keyflag3,keyflag4,keyflag5,keyflag6,keyflag7,keyflag8,
keyflag9,keyflag10,keyflag11,keyflag12,keyflag13,keyflag14,keyflag15,keyflag16; // 全局变量,默认初始值为0，判断按键是否改变按键状态

void Key_Init(void)
{
	//定义结构体
	GPIO_InitTypeDef GPIO_InitStruct;

	//打开时钟
	RCC_AHB1PeriphClockCmd(KEYBOARD_GPIO_CLK, ENABLE);

	//配置和初始化引脚 行引脚
	GPIO_InitStruct.GPIO_Pin =  KEYBOARD_GPIO_LINE;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; //必须使用上拉电阻GPIO_PuPd_UP 使电平默认为高电平1 
	GPIO_Init(KEYBOARD_GPIO_PORT, &GPIO_InitStruct);

	//配置和初始化引脚 列引脚
	GPIO_InitStruct.GPIO_Pin = KEYBOARD_GPIO_ROW;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; //不使用上下拉电阻GPIO_PuPd_NOPULL
	GPIO_Init(KEYBOARD_GPIO_PORT, &GPIO_InitStruct);
}

void get_key(void)
{
	GPIO_ResetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW1);  // 将第1列置低  目的是使按键短路
	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW2);    // 将第2列置高
	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW3);    // 将第3列置高
	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW4);    // 将第4列置高

	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 0 && keyflag1 == 0) //检测按键1
	{
		Delay_ms(10); //按键消抖
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 0 && keyflag1 == 0) //检测按键1
		{
			KeyState = '1';
			keyflag1=1;  //不再允许按键赋值,直到松开按键
		}
	}
	else if(GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 1)
	{
		keyflag1=0; //允许按键赋值
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 0 && keyflag4 == 0) //检测按键4
	{
		Delay_ms(10); //按键消抖
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 0 && keyflag4 == 0) //检测按键4
		{
			KeyState = '4';
			keyflag4 = 1; //不再允许按键赋值,直到松开按键
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 1)
	{
		keyflag4 = 0; //允许按键赋值
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 0 && keyflag7 == 0) //检测按键7
	{
		Delay_ms(10); //按键消抖
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 0 && keyflag7 == 0) //检测按键7
		{
		KeyState = '7';
		keyflag7 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 1)
	{
		keyflag7 = 0;
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE4) == 0 && keyflag11 == 0) //检测按键*
	{
		Delay_ms(10); //按键消抖
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE4) == 0 && keyflag11 == 0) //检测按键*
		{
			KeyState = '*';
			keyflag11 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE4) == 1)
	{
		keyflag11 = 0;
	}

	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW1);   // 将第1列置高
	GPIO_ResetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW2); // 将第2列置低  目的是使按键短路
	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW3);   // 将第3列置高
	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW4);   // 将第4列置高

	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 0 && keyflag2 == 0) //检测按键2
	{
		Delay_ms(10); //按键消抖
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 0 && keyflag2 == 0) //检测按键2
		{
			KeyState = '2';
			keyflag2 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 1)
	{
		keyflag2 = 0;
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 0 && keyflag5 == 0) //检测按键5
	{
		Delay_ms(10); //按键消抖
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 0 && keyflag5 == 0) //检测按键5
		{
			KeyState = '5';
			keyflag5 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 1)
	{
		keyflag5 = 0;
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 0 && keyflag8 == 0) //检测按键8
	{
		Delay_ms(10); //按键消抖
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 0 && keyflag8 == 0) //检测按键8
		{
			KeyState = '8';
			keyflag8 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 1)
	{
		keyflag8 = 0;
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE4) == 0 && keyflag10 == 0) //检测按键0
	{
		Delay_ms(10); //按键消抖
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE4) == 0 && keyflag10 == 0) //检测按键0
		{
			KeyState = '0';
			keyflag10 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE4) == 1)
	{
		keyflag10 = 0;
	}

	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW1);    // 将第1列置高
	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW2);    // 将第2列置高
	GPIO_ResetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW3);  // 将第3列置低   目的是使按键短路
	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW4);    // 将第4列置高

	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 0 && keyflag3 == 0) //检测按键3
	{
		Delay_ms(10); //按键消抖
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 0 && keyflag3 == 0) //检测按键3
		{
			KeyState = '3';
			keyflag3 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 1)
	{
		keyflag3 = 0;
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 0 && keyflag6 == 0) //检测按键6
	{
		Delay_ms(10); //按键消抖
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 0 && keyflag6 == 0) //检测按键6
		{
			KeyState = '6';
			keyflag6 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 1)
	{
		keyflag6 = 0;
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 0 && keyflag9 == 0) //检测按键9
	{
		Delay_ms(10); //按键消抖
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 0 && keyflag9 == 0) //检测按键9
		{
			KeyState = '9';
			keyflag9 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 1)
	{
		keyflag9 = 0;
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE4) == 0 && keyflag12 == 0) //检测按键#
	{
		Delay_ms(10); //按键消抖
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE4) == 0 && keyflag12 == 0) //检测按键#
		{
			KeyState = '#';
			keyflag12 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE4) == 1)
	{
		keyflag12 = 0;
	}

	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW1);      // 将第1列置高
	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW2);      // 将第2列置高
	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW3);      // 将第3列置高   
	GPIO_ResetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW4);    // 将第4列置低   目的是使按键短路

	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 0 && keyflag13 == 0) //检测按键A
	{
		Delay_ms(10); //按键消抖
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 0 && keyflag13 == 0) //检测按键A
		{
			KeyState = 'A';
			keyflag13 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 1)
	{
		keyflag13 = 0;
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 0 && keyflag14 == 0) //检测按键B
	{
		Delay_ms(10); //按键消抖
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 0 && keyflag14 == 0) //检测按键B
		{
			KeyState = 'B';
			keyflag14 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 1)
	{
		keyflag14 = 0;
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 0 && keyflag15 == 0) //检测按键C
	{
		Delay_ms(10); //按键消抖
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 0 && keyflag15 == 0) //检测按键C
		{
			KeyState = 'C';
			keyflag15 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 1)
	{
		keyflag15 = 0;
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE4) == 0 && keyflag16 == 0) //检测按键D
	{
		Delay_ms(10); //按键消抖
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE4) == 0 && keyflag16 == 0) //检测按键D
		{
			KeyState = 'D';
			keyflag16 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE4) == 1)
	{
		keyflag16 = 0;
	}
	
}
