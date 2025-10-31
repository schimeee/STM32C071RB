/******************************************************************************
 * @file    main.c
 * @brief   Minimal bare-metal example for STM32C071RB:
 *          Turns on the user LED (LD1, connected to PA5).
 *
 * @details
 *   - Enables the GPIOA peripheral clock.
 *   - Configures PA5 as a general-purpose output (push-pull).
 *   - Sets PA5 high, which lights the on-board LED (if SB1 is ON).
 *
 *   Hardware:
 *     • Target MCU : STM32C071RB (NUCLEO-C071RB board)
 *     • LED        : LD1 on pin PA5 (active HIGH)
 *     • Button		: User button on pin PC13 (active LOW)
 *     • Clock      : Default HSI (16 MHz)
 *
 *   Build & Flash:
 *     Compile as a freestanding C program (no HAL).
 *     Flash to the board and verify that LD1 lights up.
 *
 * @note
 *   This example does not use interrupts, HAL, or CMSIS helper macros.
 *   It directly accesses peripheral registers by address for clarity.
 *
 ******************************************************************************/

#include "stm32c0xx.h"

int main(void)
{
    // 1. Enable GPIOA and GPIOC clocks
    RCC->IOPENR |= (RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOCEN);

    // 2. Release GPIOs from reset  (STM32C0-specific)
    RCC->IOPRSTR = 0;

    // 3. Configure PA5 as output
    GPIOA->MODER &= ~(3U << (5 * 2));
    GPIOA->MODER |=  (1U << (5 * 2));  // general purpose output

    // 4. Configure PC13 as input, pull-up
    GPIOC->MODER &= ~(3U << (13 * 2));
    GPIOC->PUPDR &= ~(3U << (13 * 2));
    GPIOC->PUPDR |=  (1U << (13 * 2));

    // 5. Main loop
    while (1)
    {
        if ((GPIOC->IDR & (1U << 13)) == 0)
            GPIOA->ODR |=  (1U << 5);
        else
            GPIOA->ODR &= ~(1U << 5);
    }
}
