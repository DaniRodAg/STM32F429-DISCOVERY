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
#include "math.h"
#include "stdio.h"
#include "stdlib.h"


/** @addtogroup Template
  * @{
  */ 


/* Private define ------------------------------------------------------------*/
#define VRES 318
#define HRES 238
#define NCUADROS 3
#define NCIRCLE  4
/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef	GPIO_InitStructure;	//Inicialización de Terminales/Velocidad/Modo
typedef struct{
	uint16_t radio;
	uint16_t x;
	uint16_t y;
	int16_t xsig;
	int16_t ysig;
	uint16_t color;
}circle;
circle circulo[NCIRCLE];

typedef struct{
	uint16_t lado;
	uint16_t x;
	uint16_t y;
	uint16_t color;
	uint32_t area;
}square;
square cuadrado[NCUADROS];
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t n,i,j;
uint16_t dxmax,dymax, xmax,ymax;
uint32_t Narea;
float deriv, v, dir;
const float PI = 3.14159265;
/* Private function prototypes -----------------------------------------------*/
void PutPixel(int16_t x, int16_t y);
/* Private functions ---------------------------------------------------------*/
void PutPixel(int16_t x, int16_t y)
{ 
  if(x < 1 || x > 238 || y < 1 || y > 318)
  {
    return;  
  }
  LCD_DrawLine(x, y, 1, LCD_DIR_HORIZONTAL);
}

float ABS(float X)
{
	if(X<0) return -X;
	else if(X==0) return 0;
	else return X;
}

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
	LCD_SetFont(&Font12x12);
	
	srand(1);
	LCD_DrawUniLine(0,VRES,HRES,VRES);
	LCD_DrawUniLine(HRES,0,HRES,VRES);
	LCD_DrawUniLine(0,0,0,VRES);
	LCD_DrawUniLine(0,0,HRES,0);
	
	while(1)
	{
		dxmax=VRES;
		dymax=HRES;
		xmax=VRES;
		ymax=HRES;
		
		for(n=0;n<NCUADROS;n++)
		{
			
			LCD_SetTextColor(rand()%65536);
			cuadrado[n].lado = (HRES/20)+rand()%(HRES/8);
			cuadrado[n].x = 5+rand()%(HRES/(rand()%2)-2*cuadrado[n].lado-5);
			cuadrado[n].y = 5+rand()%(VRES/(rand()%2)-2*cuadrado[n].lado-5);	
			
			Narea += cuadrado[n].area;
			//Si el área de los cuadrados es mayor a la mitad, termina el ciclo
			if(Narea>(HRES*VRES/2))
			{
				break;
			}
			
			LCD_DrawRect(cuadrado[n].x,cuadrado[n].y,cuadrado[n].lado,cuadrado[n].lado);
		}
		for(n=0;n<NCIRCLE;n++)
		{
			circulo[n].color=(65536/2)+rand()%65536;
			LCD_SetTextColor(circulo[n].color);
			circulo[n].radio = 10 + rand()%24;
			circulo[n].x = circulo[n].radio+5+(rand()%(HRES-2*circulo[n].radio-5));
			circulo[n].y = circulo[n].radio+5+(rand()%(VRES-2*circulo[n].radio-5));	
			circulo[n].xsig = rand()%6-3;
			circulo[n].ysig = rand()%6-3;
			if((circulo[n].xsig==0)&&(circulo[n].ysig==0)) circulo[n].ysig = 1; 
			LCD_DrawCircle(circulo[n].x,circulo[n].y,circulo[n].radio);
		}
		
		while(1)
		{
			for(n=0;n<NCIRCLE;n++)
			{
				LCD_SetTextColor(LCD_COLOR_BLACK);
				LCD_DrawCircle(circulo[n].x,circulo[n].y,circulo[n].radio);
				LCD_SetTextColor(circulo[n].color);
				
				circulo[n].x += circulo[n].xsig;
				circulo[n].y += circulo[n].ysig;
				for(i=0;i<NCUADROS;i++)
				{
					if(((circulo[n].x+circulo[n].radio)>=HRES)||((circulo[n].x-circulo[n].radio)<=2)||
						 ((((circulo[n].x+circulo[n].radio)>=cuadrado[i].x)&&((circulo[n].x-circulo[n].radio)<=cuadrado[i].x+cuadrado[i].lado))&&
						 (((circulo[n].y+circulo[n].radio)>=cuadrado[i].y)&&((circulo[n].y-circulo[n].radio)<=cuadrado[i].y+cuadrado[i].lado))))
					{
						circulo[n].xsig = -circulo[n].xsig;
						circulo[n].x += circulo[n].xsig;
					}
					if(((circulo[n].y+circulo[n].radio)>=VRES)||((circulo[n].y-circulo[n].radio)<=2)||
						 ((((circulo[n].x+circulo[n].radio)>=cuadrado[i].x)&&((circulo[n].x-circulo[n].radio)<=cuadrado[i].x+cuadrado[i].lado))&&
						 (((circulo[n].y+circulo[n].radio)>=cuadrado[i].y)&&((circulo[n].y-circulo[n].radio)<=cuadrado[i].y+cuadrado[i].lado))))
					{
						circulo[n].ysig = -circulo[n].ysig;
						circulo[n].y += circulo[n].ysig;
					}
				}
				LCD_DrawCircle(circulo[n].x,circulo[n].y,circulo[n].radio);
				
			}
			Delayh_ms(20);
		}
		
		
		
		
		
		
		
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
