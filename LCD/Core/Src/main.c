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
#define Vref			(uint8_t)3
#define SAMPLE_RATE 12000
#define FRECUENCIA  36000
#define BUFFER_SIZE 626*SAMPLE_RATE/FRECUENCIA
#define M_PI 3.1415925

//const uint32_t SAMPLE_RATE = 12000; // Ejemplo de valor
//const uint32_t FRECUENCIA = 1000;   // Ejemplo de valor
//const uint32_t BUFFER_SIZE = (uint32_t)(626 * SAMPLE_RATE / FRECUENCIA);

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

extern uint8_t Linea;
uint16_t X,Y,Z, contador;
char texto[40];
float temperatura;
uint32_t dac_data[BUFFER_SIZE];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void BSP_LCD_printf(char line, const char *text);
void Generate_Sine_Wave(uint32_t *buffer, uint32_t buff_size, uint32_t amplitude);
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
  MX_DAC_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  //HAL_DAC_Init(&hdac);
  //HAL_DMA_Init(&hdma_dac2);
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
	sprintf(texto,"%d KHz",FRECUENCIA);
	BSP_LCD_DisplayStringAt(0, 224, texto, CENTER_MODE, 0);
	//BSP_LCD_DisplayStringAtLine(14,(uint8_t*)"24KHz", 0);
	BSP_LCD_DisplayStringAt(0, 0, "Output Signal", CENTER_MODE, 0);
	BSP_LCD_DisplayStringAt(0, 0, "Voltage", CENTER_MODE, 1);
	//BSP_LCD_DisplayStringAtLine(9,(uint8_t*)" 0123456789");
	BSP_LCD_DrawHLine(0, 11, 240);
	BSP_LCD_DrawVLine(11, 0, 320);
	contador=0;
	//HAL_DACEx_TriangleWaveGenerate(&hdac, DAC_CHANNEL_2,DAC_TRIANGLEAMPLITUDE_4095);
	Generate_Sine_Wave(dac_data, BUFFER_SIZE, ADC_RES/2); // 2048 para señal de 12 bits (amplitud)
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, dac_data, BUFFER_SIZE, DAC_ALIGN_12B_R);
	HAL_TIM_Base_Start(&htim2);
	//HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 4095);
	//HAL_DACEx_TriangleWaveGenerate(&hdac, DAC_CHANNEL_2,DAC_TRIANGLEAMPLITUDE_4095);
    /*if(HAL_DACEx_TriangleWaveGenerate(&hdac, DAC_CHANNEL_2,DAC_TRIANGLEAMPLITUDE_4095)){
		BSP_LCD_DisplayStringAtLine(6,"ERROR DAC");
	}*/


  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	//HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
	//HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R,contador);

	//sprintf(texto,"C:%2d   ",contador);
	//BSP_LCD_DisplayStringAtLine(3,(uint8_t*)texto);
	/*
	  contador+=2;
	if (contador>1000)
	{

		contador=0;
	}
	*/
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

void Generate_Sine_Wave(uint32_t *buffer, uint32_t buff_size, uint32_t amplitude)
{
    uint32_t i;
    for (i = 0; i < buff_size; i++)
        {
            float theta = 2.0f * M_PI * i / buff_size; // Cálculo del ángulo para la frecuencia correcta
            buffer[i] = (uint32_t)(amplitude * (1 + sinf(theta))); // Normalización para valores positivos

            // Dibujar el punto en la pantalla, con ajustes en la posición
            //BSP_LCD_DrawPixel(12 + buffer[i] / 20, i + 12, LCD_COLOR_RED);
        }

    for(i = 0; i < 3400;i+=1){
    	// Dibujar el punto en la pantalla, con ajustes en la posición
    	BSP_LCD_DrawPixel(12 + buffer[i%buff_size]/20, i/10 + 12, LCD_COLOR_RED);

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
