#include "time.h"


void _Delayh_ini(void)
{
	//Habilitacion del arbol de reloj
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	
	//Verificar configuracion de reloj
	SystemCoreClockUpdate();
	if (SysTick_Config(SystemCoreClock/1000))
	{
		while(1);
	}
}


//Funcion de retardo milisegundos
void _Delay_ms(__IO uint32_t mSec)
{
	mSec*= (uint32_t)CLK_CNSTNT; 
	for(; mSec != 0; mSec--);
}


void Delayh_ms (uint32_t dlyTicks)
{
	uint32_t curTicks;
	
	curTicks = msTicks;
	while ((msTicks - curTicks) < dlyTicks);
}





















//Fin del codigo
