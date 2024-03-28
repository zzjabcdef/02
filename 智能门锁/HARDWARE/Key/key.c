/*********************************************************************************
**************************STM32F407����Ӧ�ð�*************************************
**********************************************************************************
* �ļ�����: keyboard.c                                                    		 *
* �ļ�������4*4�����������		                                                 *
*    																			 *
*  												                              	 *
**********************************************************************************
*********************************************************************************/	
#include "key.h"

char KeyState = ' ';

__IO u8 keyflag1,keyflag2,keyflag3,keyflag4,keyflag5,keyflag6,keyflag7,keyflag8,
keyflag9,keyflag10,keyflag11,keyflag12,keyflag13,keyflag14,keyflag15,keyflag16; // ȫ�ֱ���,Ĭ�ϳ�ʼֵΪ0���жϰ����Ƿ�ı䰴��״̬

void Key_Init(void)
{
	//����ṹ��
	GPIO_InitTypeDef GPIO_InitStruct;

	//��ʱ��
	RCC_AHB1PeriphClockCmd(KEYBOARD_GPIO_CLK, ENABLE);

	//���úͳ�ʼ������ ������
	GPIO_InitStruct.GPIO_Pin =  KEYBOARD_GPIO_LINE;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; //����ʹ����������GPIO_PuPd_UP ʹ��ƽĬ��Ϊ�ߵ�ƽ1 
	GPIO_Init(KEYBOARD_GPIO_PORT, &GPIO_InitStruct);

	//���úͳ�ʼ������ ������
	GPIO_InitStruct.GPIO_Pin = KEYBOARD_GPIO_ROW;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; //��ʹ������������GPIO_PuPd_NOPULL
	GPIO_Init(KEYBOARD_GPIO_PORT, &GPIO_InitStruct);
}

void get_key(void)
{
	GPIO_ResetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW1);  // ����1���õ�  Ŀ����ʹ������·
	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW2);    // ����2���ø�
	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW3);    // ����3���ø�
	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW4);    // ����4���ø�

	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 0 && keyflag1 == 0) //��ⰴ��1
	{
		Delay_ms(10); //��������
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 0 && keyflag1 == 0) //��ⰴ��1
		{
			KeyState = '1';
			keyflag1=1;  //������������ֵ,ֱ���ɿ�����
		}
	}
	else if(GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 1)
	{
		keyflag1=0; //��������ֵ
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 0 && keyflag4 == 0) //��ⰴ��4
	{
		Delay_ms(10); //��������
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 0 && keyflag4 == 0) //��ⰴ��4
		{
			KeyState = '4';
			keyflag4 = 1; //������������ֵ,ֱ���ɿ�����
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 1)
	{
		keyflag4 = 0; //��������ֵ
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 0 && keyflag7 == 0) //��ⰴ��7
	{
		Delay_ms(10); //��������
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 0 && keyflag7 == 0) //��ⰴ��7
		{
		KeyState = '7';
		keyflag7 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 1)
	{
		keyflag7 = 0;
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE4) == 0 && keyflag11 == 0) //��ⰴ��*
	{
		Delay_ms(10); //��������
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE4) == 0 && keyflag11 == 0) //��ⰴ��*
		{
			KeyState = '*';
			keyflag11 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE4) == 1)
	{
		keyflag11 = 0;
	}

	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW1);   // ����1���ø�
	GPIO_ResetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW2); // ����2���õ�  Ŀ����ʹ������·
	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW3);   // ����3���ø�
	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW4);   // ����4���ø�

	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 0 && keyflag2 == 0) //��ⰴ��2
	{
		Delay_ms(10); //��������
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 0 && keyflag2 == 0) //��ⰴ��2
		{
			KeyState = '2';
			keyflag2 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 1)
	{
		keyflag2 = 0;
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 0 && keyflag5 == 0) //��ⰴ��5
	{
		Delay_ms(10); //��������
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 0 && keyflag5 == 0) //��ⰴ��5
		{
			KeyState = '5';
			keyflag5 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 1)
	{
		keyflag5 = 0;
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 0 && keyflag8 == 0) //��ⰴ��8
	{
		Delay_ms(10); //��������
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 0 && keyflag8 == 0) //��ⰴ��8
		{
			KeyState = '8';
			keyflag8 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 1)
	{
		keyflag8 = 0;
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE4) == 0 && keyflag10 == 0) //��ⰴ��0
	{
		Delay_ms(10); //��������
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE4) == 0 && keyflag10 == 0) //��ⰴ��0
		{
			KeyState = '0';
			keyflag10 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE4) == 1)
	{
		keyflag10 = 0;
	}

	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW1);    // ����1���ø�
	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW2);    // ����2���ø�
	GPIO_ResetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW3);  // ����3���õ�   Ŀ����ʹ������·
	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW4);    // ����4���ø�

	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 0 && keyflag3 == 0) //��ⰴ��3
	{
		Delay_ms(10); //��������
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 0 && keyflag3 == 0) //��ⰴ��3
		{
			KeyState = '3';
			keyflag3 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 1)
	{
		keyflag3 = 0;
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 0 && keyflag6 == 0) //��ⰴ��6
	{
		Delay_ms(10); //��������
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 0 && keyflag6 == 0) //��ⰴ��6
		{
			KeyState = '6';
			keyflag6 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 1)
	{
		keyflag6 = 0;
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 0 && keyflag9 == 0) //��ⰴ��9
	{
		Delay_ms(10); //��������
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 0 && keyflag9 == 0) //��ⰴ��9
		{
			KeyState = '9';
			keyflag9 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 1)
	{
		keyflag9 = 0;
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE4) == 0 && keyflag12 == 0) //��ⰴ��#
	{
		Delay_ms(10); //��������
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE4) == 0 && keyflag12 == 0) //��ⰴ��#
		{
			KeyState = '#';
			keyflag12 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE4) == 1)
	{
		keyflag12 = 0;
	}

	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW1);      // ����1���ø�
	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW2);      // ����2���ø�
	GPIO_SetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW3);      // ����3���ø�   
	GPIO_ResetBits(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_ROW4);    // ����4���õ�   Ŀ����ʹ������·

	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 0 && keyflag13 == 0) //��ⰴ��A
	{
		Delay_ms(10); //��������
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 0 && keyflag13 == 0) //��ⰴ��A
		{
			KeyState = 'A';
			keyflag13 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE1) == 1)
	{
		keyflag13 = 0;
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 0 && keyflag14 == 0) //��ⰴ��B
	{
		Delay_ms(10); //��������
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 0 && keyflag14 == 0) //��ⰴ��B
		{
			KeyState = 'B';
			keyflag14 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE2) == 1)
	{
		keyflag14 = 0;
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 0 && keyflag15 == 0) //��ⰴ��C
	{
		Delay_ms(10); //��������
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 0 && keyflag15 == 0) //��ⰴ��C
		{
			KeyState = 'C';
			keyflag15 = 1;
		}
	}
	else if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE3) == 1)
	{
		keyflag15 = 0;
	}
	
	if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE4) == 0 && keyflag16 == 0) //��ⰴ��D
	{
		Delay_ms(10); //��������
		if (GPIO_ReadInputDataBit(KEYBOARD_GPIO_PORT, KEYBOARD_GPIO_LINE4) == 0 && keyflag16 == 0) //��ⰴ��D
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
