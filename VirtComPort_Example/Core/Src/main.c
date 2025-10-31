/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  * Licensed per the LICENSE in project root; if absent, provided AS-IS.
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "vcp.h"

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* On NUCLEO-C0xx boards the USER button (B1) is usually PC13.
   If your BSP already defines USER_BUTTON_PIN you can remove this. */
#ifndef USER_BUTTON_PIN
#define USER_BUTTON_PIN GPIO_PIN_13
#endif

/* Debounce time for USER button in ms */
#define BTN_DEBOUNCE_MS 150U
/* USER CODE END PD */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
static volatile uint8_t led_green = 0;
static volatile uint8_t led_blue  = 0;
static volatile uint8_t btn_event_pending = 0;  /* set in ISR, handled in main */
static uint32_t last_btn_tick = 0U;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  /* Init LEDs */
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_BLUE);

  /* Init USER button with EXTI (interrupt) */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Bring up VCP */
  if (VCP_Init(115200) != 0)
  {
    Error_Handler();
  }

  /* Initial states */
  led_green = 0U; led_blue = 0U;
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Off(LED_BLUE);

  VCP_Printf("\r\nSTM32C0: LED toggle + button state stream @115200 8N1\r\n");

  /* Main loop: stream state every 100 ms, and print event lines when pending */
  while (1)
  {
    /* Read raw button level (0/1) */
    uint32_t btn = BSP_PB_GetState(BUTTON_USER);

    /* Periodic state line */
    VCP_Printf("LEDs,G:%u,B:%u, BTN:%lu\r\n", led_green, led_blue, btn);

    /* If ISR flagged an event, print a dedicated event line here (thread ctx) */
    if (btn_event_pending)
    {
      btn_event_pending = 0;
      VCP_Printf("EVENT,ButtonPress,G:%u,B:%u, BTN:%lu\r\n", led_green, led_blue, btn);
    }

    HAL_Delay(100);
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_0);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV4;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/* Debounced button EXTI: toggle LEDs and set a print flag (no UART in ISR) */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == USER_BUTTON_PIN)
  {
    uint32_t now = HAL_GetTick();
    if ((now - last_btn_tick) < BTN_DEBOUNCE_MS)
    {
      return; /* ignore bounce */
    }
    last_btn_tick = now;

    /* Toggle software state */
    led_green ^= 1U;
    led_blue  ^= 1U;

    /* Apply to hardware */
    if (led_green) BSP_LED_On(LED_GREEN); else BSP_LED_Off(LED_GREEN);
    if (led_blue)  BSP_LED_On(LED_BLUE);  else BSP_LED_Off(LED_BLUE);

    /* Signal main loop to print an event line */
    btn_event_pending = 1U;
  }
}
/* USER CODE END 4 */

void Error_Handler(void)
{
  __disable_irq();
  while (1) { }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  /* Optional diagnostics */
}
#endif /* USE_FULL_ASSERT */
