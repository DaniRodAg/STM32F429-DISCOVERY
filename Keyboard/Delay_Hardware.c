#include "Delay_H.h"
#include "stm32f4xx_syscfg.h"

extern volatile uint32_t msTicks;
//void Delay_H(uint32_t dlyTicks);
//uint32_t dlyTicks;

void SYSTICK_Cal(void)
{
	
	SystemCoreClockUpdate();
	
	if(SysTick_Config(SystemCoreClock/1000))
	{
		while(1);
	}
	
}

void Delay_H(uint32_t dlyTicks)
{
	uint32_t Curticks;
	Curticks=msTicks;
	while((msTicks-Curticks)<dlyTicks);
}
