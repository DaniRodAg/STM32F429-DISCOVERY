/*
Daniel Rodriguez Agraz
Driver de retardo por software

*/
#include "system_stm32f4xx.h"
#include "stm32f4xx.h"	
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "system_stm32f4xx.h"

//Defines
#define CLK_CNSTNT ((SystemCoreClock/1000)-1)
#define HSE_BY_MORAN
//Variables
extern volatile uint32_t msTicks;


//Funciones
void _Delayh_ini(void);
void _Delay_ms(__IO uint32_t mSec);
void _Delay_us(__IO uint32_t uSec);
void Delayh_ms (uint32_t dlyTicks);









//Fin del codigo
