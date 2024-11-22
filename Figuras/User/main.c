/**
  ******************************************************************************
  * @file    main.c 
  * @author  MC José Miguel Morán Loza
  * @version V1.0.0
  * @date    20-September-2013
  * @brief   Main program body
  ******************************************************************************

  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx.h"	//Archivo encabezado para el acceso a los 
												//dispositivos Periféricos del CMSIS Cortex-M4
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "system_stm32f4xx.h"
#include "time.h"




/** @addtogroup Template
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef	GPIO_InitStructure;	//Inicialización de Terminales/Velocidad/Modo

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/



/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< 	En este punto los ajustes del reloj del microcontrolador están 
				configurados, a través de la función SystemInit() la cual se llama
				desde el archivo startup (startup_stm32f429_439xx.s) antes de saltar
				a la aplicación principal.
				Para reconfigurar los ajustes predeterminados de la función SystemInit,
				refierase al documento system_stm32f4xx.c
  */  
  
  /* Agregue el código de su aplicación aquí */
	
	//Inicializacion delay por software
	_Delayh_ini();
	
	  /* Habilitación de reloj del periférico GPIOG */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	

  /* Configurar PG13 y PG14 como Push Pull*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
	
  /* Configurar PA0 como entrada*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);	

  /*LCD*/
	//Habilitacion del LCD
	LCD_Init();
	
	/* Configturar LCD: Configurar 2 capas w/ blending and  CLUT cargado para capa 1*/
	
	// Habilitar capa 1
	LTDC_LayerCmd(LTDC_Layer1, ENABLE);
	// Habilitar capa 
	LTDC_LayerCmd(LTDC_Layer2, ENABLE);
	
	//Recarga configuracion LTDC
	LTDC_ReloadConfig(LTDC_IMReload);
	
	//Habilita el LCD
	LTDC_Cmd(ENABLE);
	
	//CONFIGURACION DEL LDC
	LCD_LayerInit();
	//HABILITACION DEL DISPLAY DEL LCD
	LCD_DisplayOn();
	//SET FOREGROUND  LAYER
	LCD_SetLayer(LCD_FOREGROUND_LAYER);
	//CLEAR LCD
	LCD_Clear(LCD_COLOR_BLACK);
	//Set the LCD BACK COLOR
	LCD_SetBackColor(LCD_COLOR_BLACK);
	//Set LCD Text Color
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_SetFont(&Font16x24);
	
	LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"1er Prueba LCD");
	LCD_DisplayStringLine(LCD_LINE_3, (uint8_t *)"En ARM 32F104");
	LCD_DisplayStringLine(LCD_LINE_5, (uint8_t *)"By: Daniel");
	LCD_DisplayStringLine(LCD_LINE_6, (uint8_t *)"Rodriguez Agraz");
	
	
	
	
	
  while (1)
  {
		GPIOG->BSRRL=GPIO_Pin_13;	//LED Verde ON
		Delayh_ms(1);
		GPIOG->BSRRH=GPIO_Pin_13;	//LED Verde ON
		Delayh_ms(1);
  }
	
	
	
}
    












#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reportar del nombre del archivo fuente y el número de línea
  *			donde se "declara o mantiene" algún parámetro con error.
  * @param  file: Apunta al nombre del archivo fuente
  * @param  line: Número de líne que contiene el error.
  * @retval Ninguno
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* El usuario podrá agregar su propia implementación para reportar el nombre
  	 del archivo y el número de línea,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) 
	*/

  /* lazo infinito */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT 2011 Moran Technologies *****END OF FILE****/
