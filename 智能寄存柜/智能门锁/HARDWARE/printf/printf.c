#include "printf.h"


#pragma import(__use_no_semihosting_swi)


struct __FILE { int handle; /* Add whatever you need here */ };

FILE __stdout;
FILE __stdin;

//�û���Ҫ�ض���UART
int fputc(int ch, FILE *f) 
{
	USART_SendData(USART1,ch);
	while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET ); //�ȴ��������ݼĴ���Ϊ��  ��ʾ�������		
	
	return ch;
}


void _sys_exit(int return_code) 
{

}


