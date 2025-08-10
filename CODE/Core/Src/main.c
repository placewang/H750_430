/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "dma2d.h"
#include "fdcan.h"
#include "ltdc.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "string.h"

#include "bsp_lcd.h"
#include "bsp_touch.h"
#include "bsp_mx25l_spi.h"
#include "bsp_dcan.h"
#include "bsp_sdram.h"
#include "bsp_qspi_flash.h"
#include "bsp_uart.h"
#include "bsp_encoder.h"
#include "bsp_iic.h"
#include "bsp_gt911.h"
#include "bsp_TouchSet.h"

#include "app_version.h"
#include "app_cmd.h"
#include "app_config.h"
#include "app_pid.h"
#include "app_balanceRoll_code.h"
#include "app_DischargeSystem.h"
#include "app_RowknifeSystem.h"
#include "app_delta_servo.h"

#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lv_demo_stress.h"
#include "lv_demo_widgets.h"
#include "lv_demo_music.h"

#include "ui.h"
#include "ui_my_deal.h"


#include "p_set.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
unsigned char G_timer_1ms_wr = 0;
unsigned char G_timer_1ms_rd = 0;
const unsigned int G_app_flag __attribute__((at(0x90001000))) = 0xE8280750;
const unsigned char G_app_version[4] __attribute__((at(0x90001004))) = {APP_VERSION_H, APP_VERSION_L, APP_VERSION_M, APP_VERSION_S};
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
/********************************************************************************/
      
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x) 
{ 
	x = x; 
} 

void delay_us(int us)
{
	int count;
	int i;
	for (count = 0; count < us; count++)
	{
		for (i = 0; i < 13; i++)
		{
			__nop();
		}
	}
}

/*****************************************************************
BSP_init: 所有的BSP驱动初始化函数调用
******************************************************************/
void BSP_init(void)
{
    BSP_LCD_init();
    BSP_UART_init();
	BSP_SDRAM_init();
	MX25L_SPI_init();
    BSP_TouchInit();
    bsp_can_int();
//	BSP_QSPI_FLASH_init();
	BSP_ENCODER_init();
	BSP_SDRAM_test();

	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
}
/*****************************************************************
APP_init: 所有的APP初始化函数调用
******************************************************************/
void APP_init(void)
{
	ui_init();
	UI_my_deal_init();
    
	APP_CONFIG_load();
}

void MAIN_timer_1ms_Handler(void)
{
	G_timer_1ms_wr++;
    Pretr_RealTimeing_setPage();
    appRowKnifeRealTimeing();
//    Encode_handle.ReadIntervalTime++;
    BSP_TOUCH_loop_1ms(); 
	lv_tick_inc(1);
}

void MAIN_loop_1ms(void)
{
	static int timer_100ms_count = 0;
	static int timer_5ms_count = 0;
	
	timer_100ms_count++;
    
	if (timer_100ms_count >= 500)
	{
		timer_100ms_count = 0;
		HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);
	}
	timer_5ms_count++;
	if (timer_5ms_count >= 5)
	{
		timer_5ms_count = 0;
		lv_timer_handler();
	}
}

void PeriphCommonClock_Config(void);

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
   SCB->VTOR = QSPI_BASE;   
  /* USER CODE END 1 */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  SCB->CACR |= SCB_CACR_FORCEWT_Msk;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_UART4_Init();
  MX_LTDC_Init();
  MX_DMA2D_Init();
  MX_SPI1_Init();
  MX_SPI3_Init();
  MX_FMC_Init();
  MX_FDCAN1_Init();
  MX_TIM4_Init();
  MX_FDCAN2_Init();
  MX_RTC_Init();
  MX_UART5_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  BSP_init();
  APP_init();

  DELTA_Servo_init(); 
  UI_StartRecovery();
  BSP_LCD_backlight_Brightness(_LCD_ScreenBrightness);
  Pretr_setInit();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      appRowKnifeSystemTask_loop();
      bsp_Can1SendQueueMsg_loop();
//      bsp_Can2SendQueueMsg_loop();
      Pretr_RealTimeRefreshShowloop_setPage();      
      if (G_timer_1ms_wr != G_timer_1ms_rd)
	  {
		G_timer_1ms_rd++;
		MAIN_loop_1ms(); 
		UI_my_deal_loop_1ms();
	  }
      BSP_UART_loop();
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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 64;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 10;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI3|RCC_PERIPHCLK_SPI1
                              |RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLL3.PLL3M = 25;
  PeriphClkInitStruct.PLL3.PLL3N = 192;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 4;
  PeriphClkInitStruct.PLL3.PLL3R = 20;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_0;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL3;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
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
