@ In the startup_stm32c071rbtx.s 'b SystemIniti' needs to be commented

.syntax unified
.cpu cortex-m0plus
.thumb

@ ----------------------------
@ RCC registers
.equ RCC_BASE,        0x40021000
.equ RCC_IOPENR,      (RCC_BASE + 0x34)
.equ RCC_IOPENR_GPIOAEN, (1<<0)
.equ RCC_IOPENR_GPIOCEN, (1<<2)

@ ----------------------------
@ GPIO base addresses
.equ GPIOA_BASE,      0x50000000
.equ GPIOC_BASE,      0x50000800

.equ GPIOA_MODER,     (GPIOA_BASE + 0x00)
.equ GPIOA_ODR,       (GPIOA_BASE + 0x14)
.equ GPIOA_BSRR,      (GPIOA_BASE + 0x18)

.equ GPIOC_MODER,     (GPIOC_BASE + 0x00)
.equ GPIOC_PUPDR,     (GPIOC_BASE + 0x0C)
.equ GPIOC_IDR,       (GPIOC_BASE + 0x10)

@ ----------------------------
@ GPIO masks
.equ PA5_MASK,        (1<<5)
.equ PC13_MASK,       (1<<13)

.equ GPIO_MODER_5_MASK,   (0b11 << (5*2))
.equ GPIO_MODER_5_OUTPUT, (0b01 << (5*2))

.equ GPIO_PUPDR_13_MASK,  (0b11 << (13*2))
.equ GPIO_PUPDR_13_PULLUP,(0b01 << (13*2))

.global main
main:
    @ ----------------------------
    @ Enable GPIOA and GPIOC clocks
    ldr     r0, =RCC_IOPENR
    ldr     r1, [r0]
    ldr     r2, =(RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOCEN)
    orrs    r1, r2
    str     r1, [r0]

    @ ----------------------------
    @ Configure PA5 as output
    ldr     r0, =GPIOA_MODER
    ldr     r1, [r0]
    ldr     r2, =GPIO_MODER_5_MASK
    bics    r1, r2
    ldr     r2, =GPIO_MODER_5_OUTPUT
    orrs    r1, r2
    str     r1, [r0]

    @ ----------------------------
    @ Configure PC13 as input with pull-up
    ldr     r0, =GPIOC_MODER
    ldr     r1, [r0]
    movs    r2, #0          @ input mode = 00
    ldr     r3, =GPIO_PUPDR_13_MASK
    bics    r1, r3
    str     r1, [r0]

    ldr     r0, =GPIOC_PUPDR
    ldr     r1, [r0]
    ldr     r2, =GPIO_PUPDR_13_PULLUP
    orrs    r1, r2
    str     r1, [r0]

    @ ----------------------------
    @ LED and button masks
    ldr     r3, =PA5_MASK
    ldr     r4, =PC13_MASK

loop:
    @ ----------------------------
    @ Read PC13
    ldr     r0, =GPIOC_IDR
    ldr     r1, [r0]
    movs    r2, r4
    ands    r1, r2

    @ ----------------------------
    @ PC13 = 0 (pressed) → LED ON, PC13 = 1 → LED OFF
    ldr     r0, =GPIOA_ODR
    movs    r2, #0
    cmp     r1, #0
    beq     led_on
    str     r2, [r0]      @ LED off
    b       loop

led_on:
    str     r3, [r0]      @ LED on
    b       loop
