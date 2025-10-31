/******************************************************************************
 * @file    main.c
 * @brief   STM32C071RB HAL example – LED follows user button (polling mode)
 *
 * @details
 *   - Initializes the HAL library and system clock.
 *   - Configures:
 *       • PA5  as output (LED1, active HIGH)
 *       • PC13 as input  (User button B1, active LOW with pull-up)
 *   - In the main loop:
 *       • Reads PC13 using HAL_GPIO_ReadPin()
 *       • Turns LED on when the button is pressed, off otherwise
 *
 *   Hardware (NUCLEO-C071RB):
 *       LED1 (LD1)  → PA5  → active HIGH
 *       Button (B1) → PC13 → active LOW, pull-up
 *
 ******************************************************************************/
#include "main.h"

/* -----------------------------------------------------------------------*/
/*  Clock setup (HSI @ 16 MHz)                                           */
/* -----------------------------------------------------------------------*/
static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef  RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef  RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
    RCC_OscInitStruct.HSIDiv         = RCC_HSI_DIV1;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                                       RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
}

/* -----------------------------------------------------------------------*/
/*  GPIO configuration: LED = PA5, Button = PC13                          */
/* -----------------------------------------------------------------------*/
static void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* LED: PA5 output push-pull */
    GPIO_InitStruct.Pin   = GPIO_PIN_5;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Button: PC13 input with pull-up */
    GPIO_InitStruct.Pin  = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/* -----------------------------------------------------------------------*/
/*  Main: LED ON while button pressed                                     */
/* -----------------------------------------------------------------------*/
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    GPIO_Config();

    while (1)
    {
        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    }
}
