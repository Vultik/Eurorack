/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "ssd1306.h"
#include "fonts.h"
#include "test.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SAMPLES_LENGTH			512
#define ADC_CHANNELS			6
#define ADC_BUFFER_SIZE			SAMPLES_LENGTH*ADC_CHANNELS
#define ADC_BUFFER_HALF_SIZE	(SAMPLES_LENGTH/2)*ADC_CHANNELS
#define DAC_BUFFER_1_SIZE		SAMPLES_LENGTH
#define DAC_BUFFER_1_HALF_SIZE	SAMPLES_LENGTH/2
#define DAC_BUFFER_2_SIZE		SAMPLES_LENGTH
#define DAC_BUFFER_2_HALF_SIZE	SAMPLES_LENGTH/2


/*	CV_0 -> PA0 -> CHANNEL_5 -> RANK_2
 * 	CV_1 -> PA1 -> CHANNEL_6 -> RANK_3
 *	CV_2 -> PA6 -> CHANNEL_11 -> RANK_4
 *	CV_3 -> PB0 -> CHANNEL_15 -> RANK_6
 *	IN_L -> PA7 -> CHANNEL_12 -> RANK_5
 *	IN_R -> PA3 -> CHANNEL_8 -> RANK_1
 * */
// adc_buffer[6n] -> ADC_RANK_1
// adc_buffer[6n+1] -> ADC_RANK_2
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint16_t adc_buffer[ADC_BUFFER_SIZE];
uint16_t dac_buffer_1[DAC_BUFFER_1_SIZE];
uint16_t dac_buffer_2[DAC_BUFFER_2_SIZE];
uint8_t start_dsp = 0;
int16_t counter = 0;

static volatile uint16_t* in_buffer_ptr;
static volatile uint16_t* out_buffer_1_ptr;
static volatile uint16_t* out_buffer_2_ptr;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void processDSP(){
	__NOP();
	//For DAC 1 (from IN_L)
	for(uint16_t i=0; i!= SAMPLES_LENGTH; i++){
		out_buffer_1_ptr[i] = in_buffer_ptr[6*i+5];
	}

	//For DAC 2 (from IN_R)
	for(uint16_t i=0; i!= SAMPLES_LENGTH; i++){
		out_buffer_2_ptr[i] = in_buffer_ptr[6*i+1];
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
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_ADC1_Init();
  MX_DAC1_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_Base_Start(&htim6);
  HAL_ADC_Start_DMA(&hadc1 , (uint32_t *) adc_buffer, ADC_BUFFER_SIZE);
  HAL_Delay(1);
  HAL_DAC_Start_DMA(&hdac1,DAC_CHANNEL_1 , (uint32_t *) dac_buffer_1, DAC_BUFFER_1_SIZE, DAC_ALIGN_12B_R);
  HAL_DAC_Start_DMA(&hdac1,DAC_CHANNEL_2 , (uint32_t *) dac_buffer_2, DAC_BUFFER_2_SIZE, DAC_ALIGN_12B_R);
  printf("oh, un gens\r\n");

 /* uint8_t res = SSD1306_Init();
  printf("OLED init: %d\n", res);

  SSD1306_GotoXY (10,10); // goto 10, 10
  SSD1306_Puts ("HELLO", &Font_11x18, 1); // print Hello
  SSD1306_GotoXY (10, 30);
  SSD1306_Puts ("WORLD !!", &Font_11x18, 1);
  SSD1306_UpdateScreen(); // update screen*/



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //printf("%ld %ld %ld\r\n", value[0],value[1],value[2]);

		  /*for (int i=0; i!= 512;i++){
			  printf("%u\r\n", adc_buffer[6*i+1]);
		  }*/
	  if(start_dsp){
		  start_dsp = 0;
		  processDSP();
	  }
	  __NOP();

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 16;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/* USER CODE BEGIN 4 */

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc){
	__NOP();
	//First half ot the buffer is full
	in_buffer_ptr =  adc_buffer;
	out_buffer_1_ptr = &dac_buffer_1[DAC_BUFFER_1_HALF_SIZE];
	out_buffer_2_ptr = &dac_buffer_2[DAC_BUFFER_2_HALF_SIZE];
	start_dsp = 1;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	__NOP();
	//The buffer is now full
	in_buffer_ptr = &adc_buffer[ADC_BUFFER_HALF_SIZE];
	out_buffer_1_ptr = dac_buffer_1;
	out_buffer_2_ptr = dac_buffer_2;
	start_dsp = 1;
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	//Pour le switch
	if(GPIO_Pin == KNOB_SWITCH_Pin){
		//On a appuyé sur le bouton
		__NOP();
	}else{
		__NOP();
	}
	//Pour gérer la roue codeuse
	if(GPIO_Pin == GPIO_PIN_1){
		//Gestion de l'interruption
		if(HAL_GPIO_ReadPin(KNOB_CH_A_GPIO_Port, KNOB_CH_A_Pin)){
			counter++;
		}else{
			counter--;
		}
	}
}

/* USER CODE END 4 */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
