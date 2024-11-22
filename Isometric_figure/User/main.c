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



/** @addtogroup Template
  * @{
  */ 


/* Private define ------------------------------------------------------------*/
#define VRES 320
#define HRES 240
#define X0 HRES/2
#define Y0 VRES/2
#define N     4
#define NODES 20*N
#define SIDE 10
#define STEP 10
/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef	GPIO_InitStructure;	//Inicialización de Terminales/Velocidad/Modo
typedef struct{
	int16_t x;
	int16_t y;
}point;
point edge[NODES],prev;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int16_t i,j,x, y,z,px,py,xoff,scale;
float xf,yf,zf,sf,escala,deriv,deg;
const float PI = 3.14159265;
/* Private function prototypes -----------------------------------------------*/
void PutPixel(int16_t x, int16_t y);

/* Private functions ---------------------------------------------------------*/
void PutPixel(int16_t x, int16_t y)
{ 
  if(x < 0 || x > 239 || y < 0 || y > 319)
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
	LCD_SetFont(&Font16x24);
	
	
	xoff = 40;
	scale = 4;
	escala = 5;
  while (1)
  {

		
		for(j=0;j<NODES;j++)
		{
			edge[j].x = 10+j*SIDE/N;
			edge[j].y = X0/2+j*SIDE/N;
		}
		//for(deg=0;deg<(2*PI);deg+=0.2)
		//{
		LCD_Clear(LCD_COLOR_BLACK);
		LCD_DrawUniLine(10,Y0,HRES-10,Y0);
		LCD_DrawUniLine(X0,10,X0,VRES-10);
		LCD_DrawUniLine(X0,Y0,HRES-10,HRES+30);
		for(i=0;i<NODES;i++)
		{
			x = i*SIDE/N;
			xf = (escala/200)*(float)(x-xoff);
			
			for(j=0;j<NODES;j++)
			{
				y = j*SIDE/2;
				yf = (escala/200)*(float)(y-100);
				
				zf = (xf*xf)-(yf*yf);
				z = zf*scale;
				
			  px = 10+y/2+x;//(y+Y0)/2-z*sin(deg)+xf*cos(deg);
				py = Y0/2+y/2+z;//*sin(deg)+z*cos(deg);
				/*
				px = 10+x+y/2;
				py = Y0/2+z+y/2;				
				*/
				if((px-prev.x)!=0) deriv = ABS((prev.y-py)/(prev.x-px));
				if(deriv>=1) LCD_SetTextColor(LCD_COLOR_BLUE);
				else LCD_SetTextColor(LCD_COLOR_RED);

				PutPixel(py,px);
				if(x>0) LCD_DrawUniLine(py,px,edge[j].y,edge[j].x);
				if(y>0) LCD_DrawUniLine(py,px,prev.y,prev.x);

				
				prev.x = px;
				prev.y = py;
				edge[j].x = px;
				edge[j].y = py;


			}
			
		}
		Delayh_ms(50);
		while(1);
	//}
	
	
	
	
	
	
	
	}
}

    







/*
					sf = sqrt(xf*xf+yf*yf);
					zf = (deg/2)/(1+sf)*cos(sf);
					//zf = (xf*xf)/10+(yf*yf)/10;
					z = zf*75;
					px = X0+x+y/2;//-x*sin(deg)+y*cos(deg);
					py = Y0+y/2+z;//+y*sin(deg)+x*cos(deg);
					if((px-prev.x)!=0) deriv = ABS((prev.y-py)/(prev.x-px));
					//else if(deriv>1.2) LCD_SetTextColor(LCD_COLOR_NARANJA);
					//else if(deriv>1.1) LCD_SetTextColor(LCD_COLOR_YELLOW);
					if(deriv>=1) LCD_SetTextColor(LCD_COLOR_BLUE);
					else LCD_SetTextColor(LCD_COLOR_RED);
					//Delayh_ms(2);
					PutPixel(py,px);


*/




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
