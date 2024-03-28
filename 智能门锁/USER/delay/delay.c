#include "delay.h"


void delay(unsigned int i)
{
	while(i--);
}

//微秒级的延时
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

//毫秒级的延时
void delay_nnms(uint16_t delay_ms)
{    
  volatile unsigned int num;
  for (num = 0; num < delay_ms; num++)
  {
    delay_nnus(1000);
  }
}


//延时微秒  非阻塞延时
void RTOSdelay_us(u32 nus)
{
	int cnt  = 0;  //用于存储计数的总次数
	int load = 0; //用于记录Systick的自动重载寄存器的值
	int told = 0; //用于记录Systick的当前数值寄存器的初值
	int tnew = 0; //用于记录Systick的当前数值寄存器的数值
	int sum  = 0; //记录Systick的计数次数
	
	//1.计算延时时间对应的计数次数  Systick的时钟源是168MHZ 所以1us计数168次 
	cnt = nus * 168;
	
	//2.记录Systick的自动重载寄存器的值
	load = SysTick->LOAD;
	
	//3.记录Systick的当前数值寄存器的初值
	told = SysTick->VAL;
	
	//4.循环记录当前数值寄存器的计数次数并和要延时的计数总数进行比较即可
	while(1)
	{
		//5.获取Systick的当前数值寄存器的值
		tnew = SysTick->VAL;
		
		//6.判断是否可以一轮数完
		if(told != tnew)
		{
			if(told < tnew)
					sum += load - tnew + told;
			else
					sum += told - tnew;
			
			told = tnew;
			
			if(sum >= cnt) //说明时间到达
				break;
		}
	}
}

//延时毫秒  非阻塞延时
void RTOSdelay_ms(u32 nms)
{
	int i = 0;
	for(i=0;i<nms;i++)
		RTOSdelay_us(1000);
}


//若用了滴答定时器别用一下延时
void Delay_us(uint32_t xus)
{
	SysTick->CTRL = 0; 					// 关闭定时器
	SysTick->LOAD = xus*21 - 1; // 设置重载值
	SysTick->VAL 	= 0; 					// 清除计数值
	SysTick->CTRL = 1; 					// 开启定时器  21MHZ  
	while ((SysTick->CTRL & 0x00010000)==0);// 等待时间到达
	SysTick->CTRL = 0; 					// 关闭定时器
}

/**
  * @brief  毫秒级延时
  * @param  xms 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_ms(uint32_t xms)
{
	for(int i = 0; i < xms * 2; i++)
	{
		Delay_us(500);
	}
}
 
/**
  * @brief  秒级延时
  * @param  xs 延时时长，范围：0~4294967295
  * @retval 无
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
	SysTick->CTRL = 0; 					// 关闭定时器
	SysTick->LOAD = xus*21 - 1; // 设置重载值
	SysTick->VAL 	= 0; 					// 清除计数值
	SysTick->CTRL = 1; 					// 开启定时器  21MHZ  
	while ((SysTick->CTRL & 0x00010000)==0);// 等待时间到达
	SysTick->CTRL = 0; 					// 关闭定时器
}

/**
  * @brief  毫秒级延时
  * @param  xms 延时时长，范围：0~4294967295
  * @retval 无
  */
void delay_ms(uint32_t xms)
{
	for(int i = 0; i < xms * 2; i++)
	{
		Delay_us(500);
	}
}
 
/**
  * @brief  秒级延时
  * @param  xs 延时时长，范围：0~4294967295
  * @retval 无
  */
void delay_s(uint32_t xs)
{
	while(xs--)
	{
		Delay_ms(1000);
	}
} 



















