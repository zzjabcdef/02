#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H
#include "stm32f4xx.h" 

//�ж������Ƿ���յ����ݲ����д���
void BlueRecvProcessing(void);
//������������
void BLE_Config(u32 baud);

#define MaxPwdSize 16     //������󳤶�
extern __IO char pwd_buf[MaxPwdSize];
extern __IO char card_id[50][20];         //�����洢50�ſ��ţ��������Ϊ20λ
extern __IO u8 card_cnt; //�ſ�¼�������
#endif

