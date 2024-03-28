#include "timer.h"
#include "key.h" 	 
#include "usart.h"

extern vu16 USART2_RX_STA;
u8 key_num=0;
//ͨ�ö�ʱ��7�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ42M ��ʱ��Ϊ84M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
	 
void TIM7_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	//1.��TIM7��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);   
	
	//2.����TIM7��ʱʱ��  	
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_Period 		= arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ�����������ʱ��ֻ֧�ֵ�������
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	//3.����NVIC���貢��ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;							//�ж�ͨ�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;				//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;							//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							    //�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
	
	//4.ѡ��ʱ�����ж�Դ
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
	
	//5.�򿪶�ʱ��
	TIM_Cmd(TIM7, ENABLE);
}


//��ʱ��7�жϷ������		    
void TIM7_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 			   
		USART2_RX_STA|=1<<15;	//��ǽ������
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //���TIM7�����жϱ�־    
		TIM_Cmd(TIM7, DISABLE);  //�ر�TIM7 
	}	    
}














