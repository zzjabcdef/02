#include "beep.h"
#include "stm32f4xx.h"
//��������ʼ��
void Beep_Config(void)
{
	/* Private typedef -----------------------------------------------------------*/
	GPIO_InitTypeDef  GPIO_InitStructure;  //��������ṹ��

  /* GPIOG Peripheral clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); //������ʱ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;  //�����Ÿ�ֵ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //��������ٶ�
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  //��ʹ����������
	GPIO_Init(GPIOF, &GPIO_InitStructure);  //��ʼ��
	
//	GPIO_SetBits(GPIOF, GPIO_Pin_8); //Ĭ�ϵ͵�ƽ����
}


