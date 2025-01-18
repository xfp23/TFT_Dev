/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// #include "CTP.h"
// #include "ILI9341.h"
#include "LCD.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

LCD_Class_t LCD = {
  .touch = {
  .i2c.addr = 0x70,
  .i2c.byte = NULL,
  .i2c.buf = NULL,
  .i2c.handle = &hi2c2,
  .i2c.Timeout = 10,
  .flag.DMA = 0,
  .Hardware.RST.Pin = GPIO_PIN_10,
  .Hardware.RST.Port = GPIOA,
  },
  .display = {
  .spi.buf = NULL,
  .spi.bytes = NULL,
  .spi.DMA = true,
  .spi.Timeout = 10,
  .spi.handle = &hspi1,
  .spi.size = 0,
  .Hardware.CS.Pin = GPIO_PIN_9,
  .Hardware.CS.Port = GPIOB,
  .Hardware.LED.Pin = GPIO_PIN_6,
  .Hardware.LED.Port = GPIOB,
  .Hardware.RS.Pin = GPIO_PIN_7,
  .Hardware.RS.Port = GPIOB,
  .Hardware.RST.Pin = GPIO_PIN_8,
  .Hardware.RST.Port = GPIOB,
  },
  .width = 240,
  .height = 320,
};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  MX_SPI1_Init();
  MX_I2C2_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  
  LCD_Init(&LCD);
//  LCD_DrawLine(&LCD,0,0,240,320,RED,50);
//  LCD_DrawLine(&LCD,240,0,0,320,BLUE,50);
//  LCD_DrawFille_Circle(&LCD,120,120,100,MAGENTA);
//  LCD_DrawFille_Circle(&LCD,120,120,90,BROWN);
//  LCD_DrawFille_Circle(&LCD,120,120,80,TURQUOISE);
//  LCD_DrawFille_Circle(&LCD,120,120,70,GRAY);
//  LCD_DrawFille_Circle(&LCD,120,120,60,YELLOW);
//  LCD_DrawFille_Circle(&LCD,120,120,50,BLACK);
//  LCD_DrawFille_Circle(&LCD,120,120,40,RED);
//  LCD_DrawFille_Circle(&LCD,120,120,30,VIOLET);
//  LCD_DrawFille_Circle(&LCD,120,120,20,IVORY);
//  LCD_DrawFille_Circle(&LCD,120,120,10,CYAN);
//  LCD_DrawFille_Circle(&LCD,120,120,5,SALMON);
//  LCD_DrawFille_Circle(&LCD,120,120,3,BLACK);
//  LCD_DrawOval(&LCD, 120, 120, 50, 30, BLACK,2);
//  LCD_DrawFillOval(&LCD, 120, 120, 50, 30, BLACK);
//   LCD_DrawCircle(&LCD,120,200,50,BLUE,3);
//  LCD_DrawFillRectangle(&LCD,0,0,120,120,BLACK);
//  LCD_DrawRectangle(&LCD,50,20,120,120,BLACK,1);
//  LCD_DrawFilledTriangle(&LCD,50,50,20,100,40,100,PURPLE);
  LCD_ShowString(&LCD,10,45,GREEN,YELLOW,(uint8_t *)"6X12:ABCDEFGHIJKL@MNOPQRSTUVWXYZ0123456789",18,0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    CTP_mainLoop(&LCD.touch);
	//LCD_DrawLine(&LCD,0,0,240,320,BLUE);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
