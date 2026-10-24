#include <stdint.h>
#include <stm32f10x.h>
#include "spi.h"
#include "display.h"

void delay_ms(uint32_t ms) {
    for (uint32_t m = 0; m < ms; m++)
        for (volatile uint32_t i = 0; i < 7200; i++)
            __NOP();
}

int __attribute((noreturn)) main(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN;

    GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);
    GPIOC->CRH |= GPIO_CRH_MODE13_0;

    GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0 |
                    GPIO_CRL_CNF2 | GPIO_CRL_MODE2);
    GPIOA->CRL |= (GPIO_CRL_CNF0_1 | GPIO_CRL_CNF2_1);
    GPIOA->ODR |= (1U << 0) | (1U << 2);

    SPI1_Init();
    display_init();
    display_clear();
    display_draw_chessboard();

    int8_t exp = 0;

    while (1) {
        if (!(GPIOA->IDR & (1U << 0))) {
            delay_ms(20);
            if (!(GPIOA->IDR & (1U << 0))) {
                if (exp < 6) exp++;
                while (!(GPIOA->IDR & (1U << 0)));
                delay_ms(20);
            }
        }

        if (!(GPIOA->IDR & (1U << 2))) {
            delay_ms(20);
            if (!(GPIOA->IDR & (1U << 2))) {
                if (exp > -6) exp--;
                while (!(GPIOA->IDR & (1U << 2)));
                delay_ms(20);
            }
        }

        uint32_t half;
        if (exp >= 0) half = 500U >> exp;
        else          half = 500U << (-exp);
        if (half == 0) half = 1;

        GPIOC->ODR |=  (1U << 13);
        delay_ms(half);
        GPIOC->ODR &= ~(1U << 13);
        delay_ms(half);
    }
}