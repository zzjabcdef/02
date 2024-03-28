#include "printf.h"


#pragma import(__use_no_semihosting_swi)


struct __FILE { int handle; /* Add whatever you need here */ };

FILE __stdout;
FILE __stdin;

//用户需要重定向到UART
int fputc(int ch, FILE *f) 
{
	USART_SendData(USART1,ch);
	while( USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET ); //等待发送数据寄存器为空  表示发送完成		
	
	return ch;
}


void _sys_exit(int return_code) 
{

}


