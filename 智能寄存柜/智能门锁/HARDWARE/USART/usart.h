#ifndef __USART_H
#define __USART_H
#include "stm32f4xx.h"
#include "stdio.h"
 
#define USART2_MAX_RECV_LEN		400					//�����ջ����ֽ���
#define USART2_MAX_SEND_LEN		400					//����ͻ����ֽ���
#define USART2_RX_EN 			1					//0,������;1,����.

extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		//���ջ���,���USART2_MAX_RECV_LEN�ֽ�
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
extern vu16 USART2_RX_STA;   						//��������״̬

void u2_printf(char* fmt,...);

//USART2�ĳ�ʼ��
void USART2_Config(u32 baud);

//USART3�ĳ�ʼ��
void USART3_Config(u32 baud);

//����һ���ֽ�
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);

//�����ַ����ĺ���
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);

//����һ��16λ��
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f);

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f);

#endif
