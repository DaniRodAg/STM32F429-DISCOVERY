/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "crc.h"
#include "dma2d.h"
#include "i2c.h"
#include "ltdc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define NBLOQUES 40
#define MAXBALLS 15
#define NBARRAS  1
#define RADIO    5
#define BACKCOLOR LCD_COLOR_BLACK
#define OUTLINING LCD_COLOR_WHITE

uint8_t NCIRCLE=1;

static TS_StateTypeDef TS_State;

GPIO_InitTypeDef	GPIO_InitStructure;	//Inicializaci?n de Terminales/Velocidad/Modo
typedef struct{
	uint16_t x;
	uint16_t y;
	uint8_t  dead;
	int8_t   xsig;
	int8_t   ysig;
	uint32_t color;
}circle;
circle circulo[MAXBALLS];

typedef struct{
	uint8_t  dead;
	uint16_t width;
	uint16_t height;
	uint16_t x;
	uint16_t y;
	uint32_t column;
	uint32_t Points;
}blocks;
blocks bloque[NBLOQUES];

typedef struct{
	uint8_t longitud;
	uint8_t x;
	uint8_t y;
	uint32_t color;
}bar;
bar barras[1];
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define Avg_Slope (float)2.5
#define ADC_RES   4096
#define Vref			(uint8_t)3
#define V25				760
#define VRES  290
#define HRES  240
#define RSEED 3
#define DEFAULTLENGHT 60
#define DEFAULTHEIGHT 15
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern uint8_t Linea;
//uint8_t RSEED; 
char texto[40];
uint8_t lifes =3, prevlifes, velocidad=1, nivel=1;
uint8_t balls_alive=1, bricks_alive=0;
float TimeStart=0;
uint16_t dxmax,dymax, xmax,ymax;
uint16_t X,Y,Z, contador;
uint32_t score=0;
float temperatura;
float deriv, v, dir;
const float PI = 3.14159265;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void Initbar(void);
void Drawbar(uint32_t barcol);
void InitBalls(uint8_t n);
void DrawBlocks(void);
void Check4Coll(void);
void Start_screen(void);
void Menu_screen(void);
void Lossing_Screen(void);
void Winning_screen(void);
void BSP_LCD_printf(char BSP_LCD_printline, char *text, Text_AlignModeTypdef mode);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	uint8_t j;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CRC_Init();
  MX_DMA2D_Init();
  MX_FMC_Init();
  MX_I2C3_Init();
  MX_LTDC_Init();
  MX_SPI5_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
	BSP_SDRAM_Init();
	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(LCD_BACKGROUND_LAYER,LCD_FRAME_BUFFER);
  BSP_LCD_LayerDefaultInit(LCD_FOREGROUND_LAYER,LCD_FRAME_BUFFER);
	BSP_LCD_SelectLayer(LCD_FOREGROUND_LAYER);
	BSP_LCD_DisplayOn();
	BSP_LCD_Clear(BACKCOLOR);
	BSP_LCD_SetBackColor(BACKCOLOR);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	Touch_init();
	//Start_screen();
	contador=0;
	srand(RSEED);
	
	BSP_LCD_DrawLine(0,VRES,HRES,VRES);
	BSP_LCD_DrawLine(HRES,0,HRES,VRES);
	BSP_LCD_DrawLine(0,0,0,VRES);
	BSP_LCD_DrawLine(0,0,HRES,0);
	
	while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//Menu_screen();
		BSP_LCD_Clear(BACKCOLOR);
		BSP_LCD_SetBackColor(BACKCOLOR);
		Menu_screen();
		dxmax=VRES;
		dymax=HRES;
		xmax=VRES;
		ymax=HRES;
		
		velocidad=1;
		nivel=1;
		lifes=3;
		balls_alive=1;
		NCIRCLE = 1;
		DrawBlocks();
		Initbar();
		Drawbar(barras[0].color);
		InitBalls(0);
		WaitForPressedState(1);
		

		BSP_LCD_SetFont(&Times24);
		BSP_LCD_SetTextColor(OUTLINING);		
		while(1)
		{
			BSP_LCD_SetTextColor(OUTLINING);
			sprintf(texto,"SCORE:%02d",score);
			BSP_LCD_printf(0,texto,LEFT_MODE);
			Check4Coll();
			
			if(bricks_alive==0) 
			{	
				Winning_screen();
				velocidad*=3;
				balls_alive=1;
				NCIRCLE = 1;
				DrawBlocks();
				Initbar();
				Drawbar(barras[0].color);
				InitBalls(0);
				WaitForPressedState(1);
				nivel++;
			}
			if(lifes==0) 
			{
				Lossing_Screen();
				break;
			}
			
		}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}



/* USER CODE BEGIN 4 */

void DrawBlocks(void)
{
	uint8_t column=1, len=1;
	bricks_alive=0;
	for(uint8_t n=0;n<NBLOQUES;n++)
	{
		bricks_alive++;
		bloque[n].height = 20;
		bloque[n].dead = 0;
		bloque[n].column = column;
		switch(column)
		{
			case 1: BSP_LCD_SetTextColor(LCD_COLOR_LIGHTRED);
						break;
			case 2: BSP_LCD_SetTextColor(LCD_COLOR_RED);
						break;
			case 3: BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
						break;
			case 4: BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
						break;
			case 5: BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
						break;
			case 6: BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
						break;
			case 7: BSP_LCD_SetTextColor(LCD_COLOR_DARKMAGENTA);
						break;
			default: BSP_LCD_SetTextColor(LCD_COLOR_LIGHTCYAN);
						break;
		}
		

		if((HRES-len)<(HRES/4))
		{
			bloque[n].width  = HRES-len;
			bloque[n].x = len;
			bloque[n].y = VRES-column*bloque[n].height;
			column++;
			len=1;
		}
		else
		{
			bloque[n].width  = (HRES/8)+rand()%(HRES/8);
			bloque[n].x = len;
			len += bloque[n].width;
			bloque[n].y = VRES-(column*bloque[n].height);
		}
		
		bloque[n].Points = ((12-column)*(bloque[n].width*bloque[n].height))/10;
		BSP_LCD_FillRect(bloque[n].x,bloque[n].y,bloque[n].width,bloque[n].height);
		BSP_LCD_SetTextColor(OUTLINING);
		BSP_LCD_DrawRect(bloque[n].x,bloque[n].y,bloque[n].width-1,bloque[n].height-1);
	}
	
}

void Initbar(void)
{
	for(uint8_t i=0; i<NBARRAS;i++)
	{
		barras[i].color = LCD_COLOR_DARKGREEN;
		barras[i].x = HRES/2-DEFAULTLENGHT/2;
		barras[i].y = 0;
		barras[i].longitud = DEFAULTLENGHT;
	}
}


void Drawbar(uint32_t barcol)
{
	for(uint8_t i=0; i<NBARRAS;i++)
	{
		BSP_LCD_SetTextColor(barcol);
		BSP_LCD_FillRect(barras[i].x,barras[i].y,barras[i].longitud-2,DEFAULTHEIGHT-2);
	}
}


void BarTouch(void)
{
	BSP_TS_GetState(&TS_State);
	if(TS_State.TouchDetected)
	{
		if((TS_State.Y<=325-barras[0].y)
			&&(TS_State.Y>=300-barras[0].y-DEFAULTHEIGHT)
			&&(TS_State.X<=barras[0].x+barras[0].longitud)
			&&(TS_State.X>=barras[0].x))
		{
			Drawbar(BACKCOLOR);
			if(TS_State.X>=HRES-barras[0].longitud/2)	barras[0].x = HRES-5-barras[0].longitud;
			else if(TS_State.X<=barras[0].longitud/2)	barras[0].x = 2;
			else barras[0].x = TS_State.X-barras[0].longitud/2;
			Drawbar(barras[0].color);
		}

	}
}



void InitBalls(uint8_t n)
{
	circulo[0].dead=0;
	for(;n<NCIRCLE;n++)
	{
		circulo[n+1].dead=1;
		circulo[n].color=LCD_COLOR_DARKGREEN;
		BSP_LCD_SetTextColor(OUTLINING);
		circulo[n].x = HRES/2;
		circulo[n].y = VRES/4;
		circulo[n].xsig = rand() % 2 == 0 ? 1 : -1;
		circulo[n].ysig = 1;
		BSP_LCD_FillCircle(circulo[n].x,circulo[n].y,RADIO);
	}
}

void Check4Coll(void)
{
	uint8_t n,i;
	
	for(n=0;n<NCIRCLE;n++)
	{

		
		if (!circulo[n].dead)
		{
			for(i=0;i<NBLOQUES;i++)
			{
				//Si es mayor que el límite superior en x, retrocede
				if((circulo[n].x+RADIO)>=HRES-3) circulo[n].xsig = -1;

				//Si es menor que el límite inferior en x avanza
				else if((circulo[n].x-RADIO)<=3) circulo[n].xsig = 1;
		
				//Si es mayor que el límite superior en y, retrocede
				if((circulo[n].y+RADIO)>=VRES-3) circulo[n].ysig = -1;
				
				//Si es menor que el límite inferior en y 
				else if((circulo[n].y-RADIO)<=3)
				{
					
					if((balls_alive>1)&&(!circulo[n].dead))
					{
						circulo[n].dead=1;
						balls_alive--;
						break;
					}
					else 
					{
						lifes--;
						BSP_LCD_SetTextColor(BACKCOLOR);
						BSP_LCD_FillCircle(circulo[n].x,circulo[n].y,RADIO);
						InitBalls(0);
						
						Drawbar(BACKCOLOR);
						Initbar();
						Drawbar(barras[0].color);
						
						WaitForPressedState(1);
						break;
					}
				}
				
				//Revisa si el bloque está vivo
				if(!bloque[i].dead)
				{
					if((circulo[n].y>=bloque[i].y)&&(circulo[n].y<=(bloque[i].y+bloque[i].height)))
					{
						if(((circulo[n].x-RADIO)<=(bloque[i].x+bloque[i].width))&&((circulo[n].x-RADIO)>=bloque[i].x))
						{
							circulo[n].xsig = -circulo[n].xsig;
							bloque[i].dead=1;
						}
						else if(((circulo[n].x+RADIO)<=(bloque[i].x+bloque[i].width))&&((circulo[n].x+RADIO)>=bloque[i].x))
						{
							circulo[n].xsig = -circulo[n].xsig;
							bloque[i].dead=1;
						}
					}
					else if((circulo[n].x>=bloque[i].x)&&(circulo[n].x<=(bloque[i].x+bloque[i].width)))
					{
						if(((circulo[n].y-RADIO)<=(bloque[i].y+bloque[i].height))&&((circulo[n].y-RADIO)>=bloque[i].y))
						{
							circulo[n].ysig = -circulo[n].ysig;
							bloque[i].dead=1;
						}
						else if(((circulo[n].y+RADIO)<=(bloque[i].y+bloque[i].height))&&((circulo[n].y+RADIO)>=bloque[i].y))
						{
							circulo[n].ysig = -circulo[n].ysig;
							bloque[i].dead=1;

						}
					}
					

					if(bloque[i].dead)
					{
						bricks_alive--;
					  BSP_LCD_SetTextColor(BACKCOLOR);
						score+=bloque[i].Points;
						BSP_LCD_FillRect(bloque[i].x,bloque[i].y,bloque[i].width,bloque[i].height);	
						
						if(bricks_alive==(NBLOQUES-((NCIRCLE*NBLOQUES)/6)))
						{
							balls_alive+=2;
							NCIRCLE+=2;
							InitBalls(NCIRCLE-2);
							circulo[NCIRCLE-2].dead = 0;
							circulo[NCIRCLE-1].dead = 0;
						}
						
					}
					
				}
				

				if((circulo[n].y+RADIO>=barras[0].y)&&(circulo[n].y+RADIO<=(barras[0].y+DEFAULTHEIGHT)))
				{
					if(((circulo[n].x-RADIO)<=(barras[0].x+barras[0].longitud))&&((circulo[n].x-RADIO)>=barras[0].x))
					{
						circulo[n].xsig = -circulo[n].xsig;
					}
					else if(((circulo[n].x+RADIO)<=(barras[0].x+barras[0].longitud))&&((circulo[n].x+RADIO)>=barras[0].x))
					{
						circulo[n].xsig = -circulo[n].xsig;
					}
				}
				else if((circulo[n].x+RADIO>=barras[0].x)&&(circulo[n].x+RADIO<=(barras[0].x+bloque[i].width)))
				{
					if(((circulo[n].y-RADIO)<=(barras[0].y+DEFAULTHEIGHT))&&((circulo[n].y-RADIO)>=barras[0].y))
					{
						circulo[n].ysig = 1;
					}
				}
							
			}
			
			TimeStart = HAL_GetTick();
			BarTouch();
			BSP_LCD_SetTextColor(BACKCOLOR);
			BSP_LCD_FillCircle(circulo[n].x,circulo[n].y,RADIO);		
			circulo[n].y += circulo[n].ysig;
			circulo[n].x += circulo[n].xsig;
			
			if (circulo[n].dead) BSP_LCD_SetTextColor(BACKCOLOR);
			else BSP_LCD_SetTextColor(OUTLINING);
			
			BSP_LCD_FillCircle(circulo[n].x,circulo[n].y,RADIO);
			while((HAL_GetTick()-TimeStart)<=(9/(balls_alive*velocidad)))	BarTouch();
			
		}

	}
	
}
void Start_screen(void)
{
	BSP_LCD_SetTextColor(BACKCOLOR);
	BSP_LCD_SetFont(&Arcade_font48);
	BSP_LCD_printf(2,"EMBEDDED",CENTER_MODE);
	BSP_LCD_printf(3,"ARCADE",CENTER_MODE);
	HAL_Delay(500);
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_printf(19,"BY DANIEL",CENTER_MODE);
	HAL_Delay(1000);
}


void Menu_screen(void)
{
	TS_StateTypeDef  State;
	BSP_LCD_Clear(BACKCOLOR);
	BSP_LCD_SetBackColor(BACKCOLOR);
	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_SetFont(&Comicsans_font40);
	BSP_LCD_printf(4,"START",CENTER_MODE);
	WaitForPressedState(1);
	do
	{
		BSP_TS_GetState(&State);
		HAL_Delay(10);
		if((State.X>64)&&(State.X<176)&&(State.Y>140)&&(State.Y<180)) {break;}
		else {continue;}
	}while(1);
	WaitForPressedState(0);
	BSP_LCD_Clear(BACKCOLOR);
}

void BSP_LCD_printf(char BSP_LCD_printline, char *text, Text_AlignModeTypdef mode)
{
	BSP_LCD_DisplayStringAt(0,(BSP_LCD_printline*(BSP_LCD_GetFont()->Height))+2,(uint8_t*)text, mode);
}





void Winning_screen(void)
{
	BSP_LCD_Clear(LCD_COLOR_BLACK);
	HAL_Delay(100);	
	BSP_LCD_SetTextColor(OUTLINING);
	BSP_LCD_SetFont(&Arcade_font16);
	BSP_LCD_printf(2,"YOU",CENTER_MODE);
	BSP_LCD_printf(3,"WON",CENTER_MODE);
	HAL_Delay(1000);
}

void Lossing_Screen(void)
{
	BSP_LCD_Clear(LCD_COLOR_BLACK);
	HAL_Delay(100);	
	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_SetFont(&Arcade_font16);
	BSP_LCD_printf(2,"YOU",CENTER_MODE);
	BSP_LCD_printf(3,"LOSE :(",CENTER_MODE);
	HAL_Delay(1000);	
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
