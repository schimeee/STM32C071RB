.syntax unified
.cpu cortex-m0plus
.thumb

.equ RCC_BASE,              0x40021000
.equ RCC_IOPENR,            (RCC_BASE + 0x34)
.equ RCC_IOPENR_GPIOAEN,    (1 << 0)
.equ RCC_IOPENR_GPIOCEN,    (1 << 2)

.equ GPIOA_BASE,            0x50000000
.equ GPIOC_BASE,            0x50000800

.equ GPIOA_MODER,           (GPIOA_BASE + 0x00)
.equ GPIOC_MODER,           (GPIOC_BASE + 0x00)

.equ GPIOA_BSRR,            (GPIOA_BASE + 0x18)
.equ GPIOC_BSRR,            (GPIOC_BASE + 0x18)

@ PA5 → MODER5 bits (10–11)
.equ GPIO_MODER_5_MASK,     (0b11 << (5 * 2))
.equ GPIO_MODER_5_OUTPUT,   (0b01 << (5 * 2))

.global main
main:
    @ ---- Enable GPIOA and GPIOC clocks ----
    ldr     r0, =RCC_IOPENR
    ldr     r1, [r0]
    movs    r2, #RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOCEN
    orrs    r1, r2
    str     r1, [r0]

    @ ---- Configure PA5 as output ----
    ldr     r0, =GPIOA_MODER
    ldr     r1, [r0]

    ldr     r2, =GPIO_MODER_5_MASK
    bics    r1, r2
    ldr     r2, =GPIO_MODER_5_OUTPUT
    orrs    r1, r2
    str     r1, [r0]

loop:
    @ ---- Turn PA5 HIGH ----
    ldr     r0, =GPIOA_BSRR
    ldr     r1, = (1 << 5)
    str     r1, [r0]


    @ delay
    ldr     r0, =2000000
    bl      delay

    @ ---- Turn PA5 LOW ----
    ldr     r0, =GPIOA_BSRR
    ldr     r1, = (1 << (5 + 16))
    str     r1, [r0]

    @ delay
    ldr     r0, =2000000
    bl      delay

    b       loop


delay:
delay_loop:
    subs    r0, #1
    bne     delay_loop
    bx      lr
