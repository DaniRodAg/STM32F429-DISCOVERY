/**
  **************************
  * @file    main.c 
  * @author  MC José Miguel Morán Loza
  * @version V1.0.0
  * @date    20-September-2013
  * @brief   Main program body
  **************************

  */

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "stm32f4xx.h"	//Archivo encabezado para el acceso a los 
												//dispositivos Periféricos del CMSIS Cortex-M4

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#include "stm32f4xx_exti.h"		//HEADER SYSTICK
#include "stm32f4xx_syscfg.h"	//GEADER SYSTICK
#include "misc.h"							
#include "stm32f4xx_it.h"

#include "Delay_H.h"
//#include "Delay.h"
#include "math.h"

#include "stm32f429i_discovery_ioe.h"			//HEADER TOUCH

EXTI_InitTypeDef EXTI_InitStructure;
extern NVIC_InitTypeDef NVIC_InitStructure;
 

/** @addtogroup Template
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/

GPIO_InitTypeDef	GPIO_InitStructure;	//Inicialización de Terminales/Velocidad/Modo

typedef struct{				//TYPEDEF GUARDAR VARIABLES DE CIRCULO

	uint16_t xpos;
	uint16_t ypos;
	uint16_t w;
	uint16_t h;
	
}	circle;

circle button[12];
	
/* Private define ------------------------------------------------------------*/

#define PI 3.141592
#define SQUARES 4

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

//uint16_t SQHET,SQWDT;

uint16_t n;

uint16_t unidad=0;
uint16_t decena=0;
uint16_t centena=0;

static TP_STATE* TP_State;

/* Private function prototypes -----------------------------------------------*/

void Delay(__IO uint32_t nCount);
void piki(void);
	
/* Private functions ---------------------------------------------------------*/

void PutPixel(int16_t x, int16_t y);

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
	
	  /* Habilitación de reloj del periférico GPIOG */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	/* Habilitación de reloj del periférico GPIOA */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	 

  /* Configurar PG13 y PG14 como Push Pull*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	/* Configurar PA0*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//RELOJ DE SYSCFG
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	//HABILITA INTERRUPTCION DEL PUERTO
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource1);
	
	//CONFIGURA LA LINEA EXTI DEL BOTON
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd=ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	
	//NVIC
	NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//INICIALIZAR LCD
	LCD_Init();
	
	//HABILITAR CAPA 1
	LTDC_LayerCmd(LTDC_Layer1, ENABLE);
	
	//HABILITAR CAPA 2
	LTDC_LayerCmd(LTDC_Layer2, ENABLE);
	
	//RECARGA CONFIGURACION LTDC
	LTDC_ReloadConfig(LTDC_IMReload);
	
	//HABILITAR LCD
	LTDC_Cmd(ENABLE);
	
	//CONFIGURAR DE LCD
	LCD_LayerInit();
	//HABILITACION DEL DISPLAY
	LCD_DisplayOn();
	//SET FOREGROUND LAYER
	LCD_SetLayer(LCD_FOREGROUND_LAYER);
	//CLEAR THE LCD
	LCD_Clear(LCD_COLOR_BLACK);
	//SET THE LCD BLACK COLOR
	LCD_SetBackColor(LCD_COLOR_BLACK);
	//SET TEXT COLOR
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_SetFont(&Font16x24);

	uint16_t distancia,distanciax,distanciay;
	uint16_t xc=135;
	uint16_t yc=120;
	uint16_t r=25;
	uint8_t j,p=0,ycord=0,buttonpress=0;
	uint8_t xcord[3] = {65, 120, 175};
	uint16_t charposi=35,prechar,character;
	
	/*CONFIGURACION TOUCH, Y MENSAJE DE ERROR*/
	
	while(IOE_Config() != IOE_OK)	
	{
		LCD_Clear(LCD_COLOR_RED);
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_DisplayStringLine(LCD_LINE_6,(uint8_t*)"    IOE NO ESTA BIEN");
		LCD_DisplayStringLine(LCD_LINE_7,(uint8_t*)"    REINICIE LA TARJETA");
		LCD_DisplayStringLine(LCD_LINE_8,(uint8_t*)"    E INTENTE DE NUEVO");
	}
	
	LCD_Clear(LCD_COLOR_BLACK);

	LCD_DisplayChar((LCD_LINE_5-8), 59, 0x31);
	LCD_DisplayChar((LCD_LINE_7-1), 59, 0x34);
	LCD_DisplayChar((LCD_LINE_9+7), 59, 0x37);
	LCD_DisplayChar((LCD_LINE_11+13), 59, 0x43);
	
	LCD_DisplayChar((LCD_LINE_5-8), 113, 0x32);
	LCD_DisplayChar((LCD_LINE_7-1), 113, 0x35);
	LCD_DisplayChar((LCD_LINE_9+7), 113, 0x38);
	LCD_DisplayChar((LCD_LINE_11+13), 113, 0x30);
	
	LCD_DisplayChar((LCD_LINE_5-8), 169, 0x33);
	LCD_DisplayChar((LCD_LINE_7-1), 169, 0x36);
	LCD_DisplayChar((LCD_LINE_9+7), 169, 0x39);
	LCD_DisplayChar((LCD_LINE_11+13), 169, 0x58);
	
	LCD_DrawUniLine(30,75,56,75);
	LCD_DrawUniLine(61,75,87,75);
	LCD_DrawUniLine(92,75,118,75);
	LCD_DrawUniLine(123,75,149,75);
	LCD_DrawUniLine(154,75,180,75);
	LCD_DrawUniLine(185,75,211,75);
	
	for(j=0;j!=12;j++)
	{
		if(p>2)
		{
			p=0;
		}
		if(ycord>2)
		{
			yc+=55;
			ycord=0;
		}
		
		button[j].xpos=xcord[p];
		button[j].ypos=yc;
		LCD_DrawCircle(button[j].xpos,button[j].ypos,r);
		p++;
		ycord++;
	}

while(1)
	{
		
		 
		TP_State=IOE_TP_GetState();
		
		for (j = 0; j < 12; j++) {
        distanciax = TP_State->X - button[j].xpos;
        distanciay = TP_State->Y - button[j].ypos;
        distancia = (distanciax * distanciax) + (distanciay * distanciay);
				
			
        if (TP_State->TouchDetected && distancia <= (r * r)) {
            // Muestra el número o identificador del botón en la pantalla
          
					prechar=charposi;
					character=j+0x31;
					
					if(character==0x3B)
					{
						character=0x30;
					}
					
					if(character==0x3A)
					{
						LCD_SetTextColor(LCD_COLOR_BLACK);
						LCD_DrawFullRect(0, 0, 74, 240);
						LCD_SetTextColor(LCD_COLOR_WHITE);
						charposi=35;
						buttonpress=2;
					}
					
					if(charposi<=190&&buttonpress==0)
					{
            LCD_DisplayChar(LCD_LINE_2 + 2, charposi, character);
						charposi+=31;
						buttonpress=0;
					}
					
					while(TP_State->TouchDetected)
					{
						TP_State=IOE_TP_GetState();
					}
					buttonpress=0;
        }	
					
			}	//FOR
		
	}	//WHILE
	
} //main
		


static void PutPixel(int16_t x, int16_t y)
{ 
  //if(x < 0 || x > 239 || y < 0 || y > 319)	//Sin limite
	if(x < 1 || x > 238 || y < 1 || y > 318)	//Con limite de 1 pixel a los bordes
  {
    return;  
  }
  LCD_DrawLine(x, y, 1, LCD_DIR_HORIZONTAL);
}


void Delay(__IO uint32_t nCount)
{
  
	for(; nCount != 0; nCount--);
}


static void piki(void)
{
	unidad-=0x30;
	decena-=0x30;
	centena-=0x30;
	
	unidad++;
	
	if(unidad>9)
	{
		unidad=0;
		decena++;
	}
	
	if(decena>9)
	{
		decena=0;
		centena++;
	}
	
	if(centena>9)
	{
		centena=0;
	}
	
	unidad+=0x30;
	decena+=0x30;
	centena+=0x30;
	
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


/******** (C) COPYRIGHT 2011 Moran Technologies **END OF FILE*/
