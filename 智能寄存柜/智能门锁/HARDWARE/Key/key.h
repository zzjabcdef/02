#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "stm32f4xx.h"
#include "printf.h"
#include "delay.h"

#define   KEYBOARD_GPIO_PORT          GPIOF
#define   KEYBOARD_GPIO_CLK           RCC_AHB1Periph_GPIOF


//line    ÐÐ
#define   KEYBOARD_GPIO_LINE1           GPIO_Pin_0
#define   KEYBOARD_GPIO_LINE2           GPIO_Pin_1
#define   KEYBOARD_GPIO_LINE3           GPIO_Pin_2
#define   KEYBOARD_GPIO_LINE4           GPIO_Pin_3
#define	  KEYBOARD_GPIO_LINE			(KEYBOARD_GPIO_LINE1|KEYBOARD_GPIO_LINE2|KEYBOARD_GPIO_LINE3|KEYBOARD_GPIO_LINE4)


//row      ÁÐ
#define   KEYBOARD_GPIO_ROW1           GPIO_Pin_4
#define   KEYBOARD_GPIO_ROW2           GPIO_Pin_5
#define   KEYBOARD_GPIO_ROW3           GPIO_Pin_6
#define   KEYBOARD_GPIO_ROW4           GPIO_Pin_7
#define   KEYBOARD_GPIO_ROW			   (KEYBOARD_GPIO_ROW1|KEYBOARD_GPIO_ROW2|KEYBOARD_GPIO_ROW3|KEYBOARD_GPIO_ROW4)


void Key_Init(void);
void get_key(void);

extern char KeyState;

#endif
