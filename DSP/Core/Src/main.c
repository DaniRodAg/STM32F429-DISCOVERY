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
#include "dac.h"
#include "dma.h"
#include "dma2d.h"
#include "i2c.h"
#include "ltdc.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <math.h>
//#include "LOWPASSFILTER.h"
//#include "LOWPASSFILTER2ND.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_ts.h"
#include "stm32f429i_discovery_sdram.h"
#include "../Components/ili9341/ili9341.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define Avg_Slope (float)2.5
#define ADC_RES   4096
#define BUFFER_SIZE 255
#define M_PI 3.1415925
#define ORDER

typedef struct {
    float yn[ORDER+1];
    float xn[ORDER+1];
    float G_i;     // Ganancia del filtro
} Filtro;

Filtro LP_Filter = {
        .yn = {1.0000,   5.9174,   -14.5905,  19.1878,   -14.1944,   5.6005,    -0.9207,
        		0,0,0,0},
		.xn = {-1.42883992197319e-12,	-8.57303953183914e-12,	-2.14325988295979e-11,	-2.85767984394638e-11,	-2.14325988295979e-11,	-8.57303953183914e-12,	-1.42883992197319e-12,
				0,0,0,0},
		.G_i = 0.7
    };

Filtro HP_Filter = {
        .yn = {1.0000,   -5.9670,  14.8354,  -19.6719,   14.6730,   -5.8370,    0.9675,
        		0,0,0,0},
		.xn = {-0.9833,   5.8997,   -14.7492,  19.6657,   -14.7492,   5.8997,    -0.9833,
				0,0,0,0},
		.G_i = 0.6
    };

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */


extern uint8_t Linea;
uint16_t X,Y,Z, contador;
char texto[40];
uint32_t input_val = 0;
uint32_t dac_data[BUFFER_SIZE];
__IO uint32_t adc_data[BUFFER_SIZE];
__IO uint32_t low_pass_data[BUFFER_SIZE];


__IO float adcf_data[BUFFER_SIZE];
__IO float dacf_data[BUFFER_SIZE];
__IO float prev_dacf_data[BUFFER_SIZE];
__IO float prev_adcf_data[BUFFER_SIZE];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Low_Pass_Filter(void);
void High_Pass_Filter(void);
void print_Sine_Wave(void);
void BSP_LCD_printf(char line, const char *text);
void Generate_Sine_Wave(uint32_t *buffer, uint32_t buff_size, uint32_t amplitude);
void norm(void);
void Low_Pass_Filter(void);
void dacf2dac(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* Callback que se llama al completarse la conversión del ADC */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	//HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, &adc_data, BUFFER_SIZE, DAC_ALIGN_12B_R);
    if (hadc->Instance == ADC1) {
    	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
        //Procesa los datos del buffer
    	norm();
    	Low_Pass_Filter();
    	//High_Pass_Filter();
    	//Graficar los datos del ADC en la pantalla LCD
    	//print_Sine_Wave();
    }
}

void norm(void){
	for(int i = 0;i < BUFFER_SIZE-1; i++){
		adcf_data[i] = ((float)adc_data[i]-2048)/2048;
	}
}

void dacf2dac(void){
	for (int i =0; i<BUFFER_SIZE-1;i++){
		dac_data[i] = (uint32_t) (dacf_data[i]+1)*2048;
		//dac_data[i] = (uint32_t) (dac_data[i])*2048;
	}
}


void Low_Pass_Filter(void){
	for (int n = 0; n < BUFFER_SIZE; n++)
	{
		/*ORDEN 2*/
		if(n==0){
			dacf_data[n] = (float)(
							  LP_Filter.xn[0]*adcf_data[n]
							  +LP_Filter.xn[1]*prev_adcf_data[BUFFER_SIZE-1]
							  +LP_Filter.xn[2]*prev_adcf_data[BUFFER_SIZE-2]
							  +LP_Filter.xn[3]*prev_adcf_data[BUFFER_SIZE-3]
							  +LP_Filter.xn[4]*prev_adcf_data[BUFFER_SIZE-4]
							  +LP_Filter.xn[5]*prev_adcf_data[BUFFER_SIZE-5]
							  +LP_Filter.xn[6]*prev_adcf_data[BUFFER_SIZE-6]
							  +LP_Filter.yn[1]*dacf_data[BUFFER_SIZE-1]
							  +LP_Filter.yn[2]*prev_dacf_data[BUFFER_SIZE-2]
							  +LP_Filter.yn[3]*prev_dacf_data[BUFFER_SIZE-3]
							  +LP_Filter.yn[4]*prev_dacf_data[BUFFER_SIZE-4]
							  +LP_Filter.yn[5]*prev_dacf_data[BUFFER_SIZE-5]
							  +LP_Filter.yn[6]*prev_dacf_data[BUFFER_SIZE-6]
						  )*LP_Filter.G_i ;
		}
		else if(n==1){
			dacf_data[n] = (
							  LP_Filter.xn[0]*adcf_data[n]
							  +LP_Filter.xn[1]*adcf_data[n-1]
							  +LP_Filter.xn[2]*prev_adcf_data[BUFFER_SIZE-2]
							  +LP_Filter.xn[3]*prev_adcf_data[BUFFER_SIZE-3]
							  +LP_Filter.xn[4]*prev_adcf_data[BUFFER_SIZE-4]
							  +LP_Filter.xn[5]*prev_adcf_data[BUFFER_SIZE-5]
							  +LP_Filter.xn[6]*prev_adcf_data[BUFFER_SIZE-6]

							  +LP_Filter.yn[1]*dacf_data[n-1]
							  +LP_Filter.yn[2]*prev_dacf_data[BUFFER_SIZE-2]
							  +LP_Filter.yn[3]*prev_dacf_data[BUFFER_SIZE-3]
							  +LP_Filter.yn[4]*prev_dacf_data[BUFFER_SIZE-4]
							  +LP_Filter.yn[5]*prev_dacf_data[BUFFER_SIZE-5]
							  +LP_Filter.yn[6]*prev_dacf_data[BUFFER_SIZE-6]
						  )*LP_Filter.G_i ;
		}
		else if(n==2){
			dacf_data[n] = (
							  LP_Filter.xn[0]*adcf_data[n]
							  +LP_Filter.xn[1]*adcf_data[n-1]
							  +LP_Filter.xn[2]*adcf_data[n-2]
							  +LP_Filter.xn[3]*prev_adcf_data[BUFFER_SIZE-3]
							  +LP_Filter.xn[4]*prev_adcf_data[BUFFER_SIZE-4]
							  +LP_Filter.xn[5]*prev_adcf_data[BUFFER_SIZE-5]
							  +LP_Filter.xn[6]*prev_adcf_data[BUFFER_SIZE-6]

							  +LP_Filter.yn[1]*dacf_data[n-1]
							  +LP_Filter.yn[2]*dacf_data[n-2]
							  +LP_Filter.yn[3]*prev_dacf_data[BUFFER_SIZE-3]
							  +LP_Filter.yn[4]*prev_dacf_data[BUFFER_SIZE-4]
							  +LP_Filter.yn[5]*prev_dacf_data[BUFFER_SIZE-5]
							  +LP_Filter.yn[6]*prev_dacf_data[BUFFER_SIZE-6]
						  )*LP_Filter.G_i ;
				}
		else if(n==3){
					dacf_data[n] = (
							LP_Filter.xn[0]*adcf_data[n]
						  +LP_Filter.xn[1]*adcf_data[n-1]
						  +LP_Filter.xn[2]*adcf_data[n-2]
						  +LP_Filter.xn[3]*adcf_data[n-3]
						  +LP_Filter.xn[4]*prev_adcf_data[BUFFER_SIZE-4]
						  +LP_Filter.xn[5]*prev_adcf_data[BUFFER_SIZE-5]
						  +LP_Filter.xn[6]*prev_adcf_data[BUFFER_SIZE-6]

						  +LP_Filter.yn[1]*dacf_data[n-1]
						  +LP_Filter.yn[2]*dacf_data[n-2]
						  +LP_Filter.yn[3]*dacf_data[n-3]
						  +LP_Filter.yn[4]*prev_dacf_data[BUFFER_SIZE-4]
						  +LP_Filter.yn[5]*prev_dacf_data[BUFFER_SIZE-5]
						  +LP_Filter.yn[6]*prev_dacf_data[BUFFER_SIZE-6]

					  )*LP_Filter.G_i ;
				}

		else if(n==4){
			dacf_data[n] = (
							LP_Filter.xn[0]*adcf_data[n]
						  +LP_Filter.xn[1]*adcf_data[n-1]
						  +LP_Filter.xn[2]*adcf_data[n-2]
						  +LP_Filter.xn[3]*adcf_data[n-3]
						  +LP_Filter.xn[4]*adcf_data[n-4]
						  +LP_Filter.xn[5]*prev_adcf_data[BUFFER_SIZE-5]
						  +LP_Filter.xn[6]*prev_adcf_data[BUFFER_SIZE-6]

						  +LP_Filter.yn[1]*dacf_data[n-1]
						  +LP_Filter.yn[2]*dacf_data[n-2]
						  +LP_Filter.yn[3]*dacf_data[n-3]
						  +LP_Filter.yn[4]*dacf_data[n-4]
						  +LP_Filter.yn[5]*prev_dacf_data[BUFFER_SIZE-5]
						  +LP_Filter.yn[6]*prev_dacf_data[BUFFER_SIZE-6]

					  )*LP_Filter.G_i ;
			}
		else if(n==5){
			dacf_data[n] = (
							LP_Filter.xn[0]*adcf_data[n]
						  +LP_Filter.xn[1]*adcf_data[n-1]
						  +LP_Filter.xn[2]*adcf_data[n-2]
						  +LP_Filter.xn[3]*adcf_data[n-3]
						  +LP_Filter.xn[4]*adcf_data[n-4]
						  +LP_Filter.xn[5]*adcf_data[n-5]
						  +LP_Filter.xn[6]*prev_adcf_data[BUFFER_SIZE-6]

						  +LP_Filter.yn[1]*dacf_data[n-1]
						  +LP_Filter.yn[2]*dacf_data[n-2]
						  +LP_Filter.yn[3]*dacf_data[n-3]
						  +LP_Filter.yn[4]*dacf_data[n-4]
						  +LP_Filter.yn[5]*dacf_data[n-5]
						  +LP_Filter.yn[6]*prev_dacf_data[BUFFER_SIZE-6]

					  )*LP_Filter.G_i ;
					}
		else{
			dacf_data[n] = (
							LP_Filter.xn[0]*adcf_data[n]
						  +LP_Filter.xn[1]*adcf_data[n-1]
						  +LP_Filter.xn[2]*adcf_data[n-2]
						  +LP_Filter.xn[3]*adcf_data[n-3]
						  +LP_Filter.xn[4]*adcf_data[n-4]
						  +LP_Filter.xn[5]*adcf_data[n-5]
						  +LP_Filter.xn[6]*adcf_data[n-6]

						  +LP_Filter.yn[1]*dacf_data[n-1]
						  +LP_Filter.yn[2]*dacf_data[n-2]
						  +LP_Filter.yn[3]*dacf_data[n-3]
						  +LP_Filter.yn[4]*dacf_data[n-4]
						  +LP_Filter.yn[5]*dacf_data[n-5]
						  +LP_Filter.yn[6]*dacf_data[n-6]

					)*LP_Filter.G_i ;
		}
		dac_data[n] = ((int32_t)(dacf_data[n]*2048)) + 2048;v|
		prev_adcf_data[n] = adcf_data[n];
	    prev_dacf_data[n] = dacf_data[n];
	}
}

void High_Pass_Filter(void){
	for (int n = 0; n < BUFFER_SIZE; n++)
	{
		/*ORDEN 2*/
		if(n==0){
			dacf_data[n] = (float)(
							  HP_Filter.xn[0]*adcf_data[n]
							  +HP_Filter.xn[1]*prev_adcf_data[BUFFER_SIZE-1]
							  +HP_Filter.xn[2]*prev_adcf_data[BUFFER_SIZE-2]
							  +HP_Filter.xn[3]*prev_adcf_data[BUFFER_SIZE-3]
							  +HP_Filter.xn[4]*prev_adcf_data[BUFFER_SIZE-4]
							  +HP_Filter.xn[5]*prev_adcf_data[BUFFER_SIZE-5]
							  +HP_Filter.xn[6]*prev_adcf_data[BUFFER_SIZE-6]
							  +HP_Filter.yn[1]*dacf_data[BUFFER_SIZE-1]
							  +HP_Filter.yn[2]*prev_dacf_data[BUFFER_SIZE-2]
							  +HP_Filter.yn[3]*prev_dacf_data[BUFFER_SIZE-3]
							  +HP_Filter.yn[4]*prev_dacf_data[BUFFER_SIZE-4]
							  +HP_Filter.yn[5]*prev_dacf_data[BUFFER_SIZE-5]
							  +HP_Filter.yn[6]*prev_dacf_data[BUFFER_SIZE-6]
						  )*HP_Filter.G_i ;
		}
		else if(n==1){
			dacf_data[n] = (
							  HP_Filter.xn[0]*adcf_data[n]
							  +HP_Filter.xn[1]*adcf_data[n-1]
							  +HP_Filter.xn[2]*prev_adcf_data[BUFFER_SIZE-2]
							  +HP_Filter.xn[3]*prev_adcf_data[BUFFER_SIZE-3]
							  +HP_Filter.xn[4]*prev_adcf_data[BUFFER_SIZE-4]
							  +HP_Filter.xn[5]*prev_adcf_data[BUFFER_SIZE-5]
							  +HP_Filter.xn[6]*prev_adcf_data[BUFFER_SIZE-6]

							  +HP_Filter.yn[1]*dacf_data[n-1]
							  +HP_Filter.yn[2]*prev_dacf_data[BUFFER_SIZE-2]
							  +HP_Filter.yn[3]*prev_dacf_data[BUFFER_SIZE-3]
							  +HP_Filter.yn[4]*prev_dacf_data[BUFFER_SIZE-4]
							  +HP_Filter.yn[5]*prev_dacf_data[BUFFER_SIZE-5]
							  +HP_Filter.yn[6]*prev_dacf_data[BUFFER_SIZE-6]
						  )*HP_Filter.G_i ;
		}
		else if(n==2){
			dacf_data[n] = (
							  HP_Filter.xn[0]*adcf_data[n]
							  +HP_Filter.xn[1]*adcf_data[n-1]
							  +HP_Filter.xn[2]*adcf_data[n-2]
							  +HP_Filter.xn[3]*prev_adcf_data[BUFFER_SIZE-3]
							  +HP_Filter.xn[4]*prev_adcf_data[BUFFER_SIZE-4]
							  +HP_Filter.xn[5]*prev_adcf_data[BUFFER_SIZE-5]
							  +HP_Filter.xn[6]*prev_adcf_data[BUFFER_SIZE-6]

							  +HP_Filter.yn[1]*dacf_data[n-1]
							  +HP_Filter.yn[2]*dacf_data[n-2]
							  +HP_Filter.yn[3]*prev_dacf_data[BUFFER_SIZE-3]
							  +HP_Filter.yn[4]*prev_dacf_data[BUFFER_SIZE-4]
							  +HP_Filter.yn[5]*prev_dacf_data[BUFFER_SIZE-5]
							  +HP_Filter.yn[6]*prev_dacf_data[BUFFER_SIZE-6]
						  )*HP_Filter.G_i ;
				}
		else if(n==3){
					dacf_data[n] = (
							HP_Filter.xn[0]*adcf_data[n]
						  +HP_Filter.xn[1]*adcf_data[n-1]
						  +HP_Filter.xn[2]*adcf_data[n-2]
						  +HP_Filter.xn[3]*adcf_data[n-3]
						  +HP_Filter.xn[4]*prev_adcf_data[BUFFER_SIZE-4]
						  +HP_Filter.xn[5]*prev_adcf_data[BUFFER_SIZE-5]
						  +HP_Filter.xn[6]*prev_adcf_data[BUFFER_SIZE-6]

						  +HP_Filter.yn[1]*dacf_data[n-1]
						  +HP_Filter.yn[2]*dacf_data[n-2]
						  +HP_Filter.yn[3]*dacf_data[n-3]
						  +HP_Filter.yn[4]*prev_dacf_data[BUFFER_SIZE-4]
						  +HP_Filter.yn[5]*prev_dacf_data[BUFFER_SIZE-5]
						  +HP_Filter.yn[6]*prev_dacf_data[BUFFER_SIZE-6]

					  )*HP_Filter.G_i ;
				}

		else if(n==4){
			dacf_data[n] = (
							HP_Filter.xn[0]*adcf_data[n]
						  +HP_Filter.xn[1]*adcf_data[n-1]
						  +HP_Filter.xn[2]*adcf_data[n-2]
						  +HP_Filter.xn[3]*adcf_data[n-3]
						  +HP_Filter.xn[4]*adcf_data[n-4]
						  +HP_Filter.xn[5]*prev_adcf_data[BUFFER_SIZE-5]
						  +HP_Filter.xn[6]*prev_adcf_data[BUFFER_SIZE-6]

						  +HP_Filter.yn[1]*dacf_data[n-1]
						  +HP_Filter.yn[2]*dacf_data[n-2]
						  +HP_Filter.yn[3]*dacf_data[n-3]
						  +HP_Filter.yn[4]*dacf_data[n-4]
						  +HP_Filter.yn[5]*prev_dacf_data[BUFFER_SIZE-5]
						  +HP_Filter.yn[6]*prev_dacf_data[BUFFER_SIZE-6]

					  )*HP_Filter.G_i ;
			}
		else if(n==5){
			dacf_data[n] = (
							HP_Filter.xn[0]*adcf_data[n]
						  +HP_Filter.xn[1]*adcf_data[n-1]
						  +HP_Filter.xn[2]*adcf_data[n-2]
						  +HP_Filter.xn[3]*adcf_data[n-3]
						  +HP_Filter.xn[4]*adcf_data[n-4]
						  +HP_Filter.xn[5]*adcf_data[n-5]
						  +HP_Filter.xn[6]*prev_adcf_data[BUFFER_SIZE-6]

						  +HP_Filter.yn[1]*dacf_data[n-1]
						  +HP_Filter.yn[2]*dacf_data[n-2]
						  +HP_Filter.yn[3]*dacf_data[n-3]
						  +HP_Filter.yn[4]*dacf_data[n-4]
						  +HP_Filter.yn[5]*dacf_data[n-5]
						  +HP_Filter.yn[6]*prev_dacf_data[BUFFER_SIZE-6]

					  )*HP_Filter.G_i ;
					}
		else{
			dacf_data[n] = (
							HP_Filter.xn[0]*adcf_data[n]
						  +HP_Filter.xn[1]*adcf_data[n-1]
						  +HP_Filter.xn[2]*adcf_data[n-2]
						  +HP_Filter.xn[3]*adcf_data[n-3]
						  +HP_Filter.xn[4]*adcf_data[n-4]
						  +HP_Filter.xn[5]*adcf_data[n-5]
						  +HP_Filter.xn[6]*adcf_data[n-6]

						  +HP_Filter.yn[1]*dacf_data[n-1]
						  +HP_Filter.yn[2]*dacf_data[n-2]
						  +HP_Filter.yn[3]*dacf_data[n-3]
						  +HP_Filter.yn[4]*dacf_data[n-4]
						  +HP_Filter.yn[5]*dacf_data[n-5]
						  +HP_Filter.yn[6]*dacf_data[n-6]

					)*HP_Filter.G_i ;
		}
		dac_data[n] = ((int32_t)(dacf_data[n]*2048)) + 2048;
		prev_adcf_data[n] = adcf_data[n];
	    prev_dacf_data[n] = dacf_data[n];
	}
}




/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

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
  MX_DMA_Init();
  MX_CRC_Init();
  MX_DMA2D_Init();
  MX_FMC_Init();
  MX_I2C3_Init();
  MX_LTDC_Init();
  MX_SPI5_Init();
  MX_TIM1_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_DAC_Init();
  /* USER CODE BEGIN 2 */
	BSP_SDRAM_Init();
	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(LCD_BACKGROUND_LAYER,LCD_FRAME_BUFFER);
	BSP_LCD_LayerDefaultInit(LCD_FOREGROUND_LAYER,LCD_FRAME_BUFFER);
	BSP_LCD_SelectLayer(LCD_FOREGROUND_LAYER);
	BSP_LCD_DisplayOn();
	BSP_LCD_Clear(LCD_COLOR_BLACK);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_TS_Init(240,320);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	BSP_LCD_SetFont(&Font16);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	sprintf(texto,"KHz");
	BSP_LCD_DisplayStringAt(0, 224, texto, CENTER_MODE, 0);
	//BSP_LCD_DisplayStringAtLine(14,(uint8_t*)"24KHz", 0);
	BSP_LCD_DisplayStringAt(0, 0, "Input Signal", CENTER_MODE, 0);
	BSP_LCD_DisplayStringAt(0, 0, "Voltage", CENTER_MODE, 1);
	//BSP_LCD_DisplayStringAtLine(9,(uint8_t*)" 0123456789");
	BSP_LCD_DrawHLine(0, 11, 240);
	BSP_LCD_DrawVLine(11, 0, 320);
	contador=0;
	//Generate_Sine_Wave(dac_data, BUFFER_SIZE, ADC_RES/2); // 2048 para señal de 12 bits (amplitud)
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, &dac_data, BUFFER_SIZE, DAC_ALIGN_12B_R);
	HAL_TIM_Base_Start(&htim2);

	  if(HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc_data, BUFFER_SIZE) != HAL_OK)
	  {
	    /* Start Conversation Error */
	    Error_Handler();
	  }


  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


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
void print_Sine_Wave(void){
	//BSP_LCD_Clear(LCD_COLOR_BLACK);
	sprintf(texto,"%.4d",adc_data[1]);
	BSP_LCD_DisplayStringAt(13, 185, texto, LEFT_MODE, 0);
	sprintf(texto,"KHz");
	BSP_LCD_DisplayStringAt(0, 224, texto, CENTER_MODE, 0);
	//BSP_LCD_DisplayStringAtLine(14,(uint8_t*)"24KHz", 0);
	BSP_LCD_DisplayStringAt(0, 0, "Input Signal", CENTER_MODE, 0);
	BSP_LCD_DisplayStringAt(0, 0, "Voltage", CENTER_MODE, 1);

	for (int i = 0; i < BUFFER_SIZE; i++)
	        {
				BSP_LCD_DrawPixel(12 + prev_adcf_data[i] / 20, i + 12, LCD_COLOR_BLACK);
				BSP_LCD_DrawPixel(12 + adcf_data[i] / 20, i + 12, LCD_COLOR_RED);

				//BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
				prev_adcf_data[i] = adcf_data[i];
	        }


}


void Generate_Sine_Wave(uint32_t *buffer, uint32_t buff_size, uint32_t amplitude)
{
    uint32_t i;
    for (i = 0; i < buff_size; i++)
        {
            float theta = 2.0f * M_PI * i / buff_size; // Cálculo del ángulo para la frecuencia correcta
            buffer[i] = (uint32_t)(amplitude * (1 + sinf(theta))); // Normalización para valores positivos

            //Dibujar el punto en la pantalla, con ajustes en la posición
            BSP_LCD_DrawPixel(12 + buffer[i] / 20, i + 12, LCD_COLOR_RED);
        }
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
  /* Turn LED4 on */
  BSP_LED_On(LED4);
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
