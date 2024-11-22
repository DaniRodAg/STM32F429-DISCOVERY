
#include "Delay.h"
uint32_t nCount;

void Delay_S(uint32_t mili)
{
  //uint32_t nCount;
	//mili=1000*mili;
	nCount=(1000*mili)-276;
	for(; nCount != 0; nCount--);
}
