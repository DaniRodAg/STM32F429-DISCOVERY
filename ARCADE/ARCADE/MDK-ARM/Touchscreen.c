#include "main.h"

void Touch_init(void)
{
	uint8_t status = 0;
  status = BSP_TS_Init(240,320);
	while(status != TS_OK)
  {
		BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
		BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 148, (uint8_t*)"ERROR", LEFT_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 132, (uint8_t*)"No se puede", LEFT_MODE);
		BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 116, (uint8_t*)"inicializar", LEFT_MODE);
		BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 100, (uint8_t*)"el touchscreen", LEFT_MODE);
  }
}


void WaitForPressedState(uint8_t Pressed) 
{
  TS_StateTypeDef  State;
  
  do 
  {
    BSP_TS_GetState(&State);
    HAL_Delay(10);
    if (State.TouchDetected == Pressed) 
    {
      uint16_t TimeStart = HAL_GetTick();
      do
			{
        BSP_TS_GetState(&State);
        HAL_Delay(10);
        if (State.TouchDetected != Pressed) 
        {
          break;
        } 
				else if ((HAL_GetTick() - 100) > TimeStart) 
        {
          return;
        }
      } while (1);
    }
  } while (1);
}


	
















