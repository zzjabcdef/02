#ifndef __DELAY_H
#define __DELAY_H 			   

#include "stm32f4xx.h"
typedef uint32_t  u32;
typedef uint16_t  u16;
typedef uint8_t   u8;


void delay(unsigned int i);
void delay_nnus(uint32_t delay_us);
void delay_nnms(uint16_t delay_ms);
void RTOSdelay_ms(u32 nms);
void RTOSdelay_us(u32 nus);
void Delay_us(uint32_t xus);
void Delay_ms(uint32_t xms);
void Delay_s(uint32_t xs);
void delay_us(uint32_t xus);
void delay_ms(uint32_t xms);
void delay_s(uint32_t xs);

#endif





























