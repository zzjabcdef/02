#ifndef _PRINTF_H
#define _PRINTF_H

#include "stm32f4xx.h"
#include <stdio.h>
#include "usart1.h"


int fputc(int ch, FILE *f) ;
void _sys_exit(int return_code) ;

#endif

