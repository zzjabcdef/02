#include "delay.h"


void delay(unsigned int i)
{
	while(i--);
}

//΢�뼶����ʱ
void delay_nnus(uint32_t delay_us)
{    
  volatile unsigned int num;
  volatile unsigned int t;
 
  
  for (num = 0; num < delay_us; num++)
  {
    t = 11;
    while (t != 0)
    {
      t--;
    }
  }
}

//���뼶����ʱ
void delay_nnms(uint16_t delay_ms)
{    
  volatile unsigned int num;
  for (num = 0; num < delay_ms; num++)
  {
    delay_nnus(1000);
  }
}


//��ʱ΢��  ��������ʱ
void RTOSdelay_us(u32 nus)
{
	int cnt  = 0;  //���ڴ洢�������ܴ���
	int load = 0; //���ڼ�¼Systick���Զ����ؼĴ�����ֵ
	int told = 0; //���ڼ�¼Systick�ĵ�ǰ��ֵ�Ĵ����ĳ�ֵ
	int tnew = 0; //���ڼ�¼Systick�ĵ�ǰ��ֵ�Ĵ�������ֵ
	int sum  = 0; //��¼Systick�ļ�������
	
	//1.������ʱʱ���Ӧ�ļ�������  Systick��ʱ��Դ��168MHZ ����1us����168�� 
	cnt = nus * 168;
	
	//2.��¼Systick���Զ����ؼĴ�����ֵ
	load = SysTick->LOAD;
	
	//3.��¼Systick�ĵ�ǰ��ֵ�Ĵ����ĳ�ֵ
	told = SysTick->VAL;
	
	//4.ѭ����¼��ǰ��ֵ�Ĵ����ļ�����������Ҫ��ʱ�ļ����������бȽϼ���
	while(1)
	{
		//5.��ȡSystick�ĵ�ǰ��ֵ�Ĵ�����ֵ
		tnew = SysTick->VAL;
		
		//6.�ж��Ƿ����һ������
		if(told != tnew)
		{
			if(told < tnew)
					sum += load - tnew + told;
			else
					sum += told - tnew;
			
			told = tnew;
			
			if(sum >= cnt) //˵��ʱ�䵽��
				break;
		}
	}
}

//��ʱ����  ��������ʱ
void RTOSdelay_ms(u32 nms)
{
	int i = 0;
	for(i=0;i<nms;i++)
		RTOSdelay_us(1000);
}


//�����˵δ�ʱ������һ����ʱ
void Delay_us(uint32_t xus)
{
	SysTick->CTRL = 0; 					// �رն�ʱ��
	SysTick->LOAD = xus*21 - 1; // ��������ֵ
	SysTick->VAL 	= 0; 					// �������ֵ
	SysTick->CTRL = 1; 					// ������ʱ��  21MHZ  
	while ((SysTick->CTRL & 0x00010000)==0);// �ȴ�ʱ�䵽��
	SysTick->CTRL = 0; 					// �رն�ʱ��
}

/**
  * @brief  ���뼶��ʱ
  * @param  xms ��ʱʱ������Χ��0~4294967295
  * @retval ��
  */
void Delay_ms(uint32_t xms)
{
	for(int i = 0; i < xms * 2; i++)
	{
		Delay_us(500);
	}
}
 
/**
  * @brief  �뼶��ʱ
  * @param  xs ��ʱʱ������Χ��0~4294967295
  * @retval ��
  */
void Delay_s(uint32_t xs)
{
	while(xs--)
	{
		Delay_ms(1000);
	}
} 

void delay_us(uint32_t xus)
{
	SysTick->CTRL = 0; 					// �رն�ʱ��
	SysTick->LOAD = xus*21 - 1; // ��������ֵ
	SysTick->VAL 	= 0; 					// �������ֵ
	SysTick->CTRL = 1; 					// ������ʱ��  21MHZ  
	while ((SysTick->CTRL & 0x00010000)==0);// �ȴ�ʱ�䵽��
	SysTick->CTRL = 0; 					// �رն�ʱ��
}

/**
  * @brief  ���뼶��ʱ
  * @param  xms ��ʱʱ������Χ��0~4294967295
  * @retval ��
  */
void delay_ms(uint32_t xms)
{
	for(int i = 0; i < xms * 2; i++)
	{
		Delay_us(500);
	}
}
 
/**
  * @brief  �뼶��ʱ
  * @param  xs ��ʱʱ������Χ��0~4294967295
  * @retval ��
  */
void delay_s(uint32_t xs)
{
	while(xs--)
	{
		Delay_ms(1000);
	}
} 



















