#include "rcc.h"
#include "nvic.h"  // Para RCC_APB2ENR_SYSCFGEN

void rcc_init(void)
{
    RCC->AHB2ENR |= (1 << 0);                      // Habilita reloj GPIOA
    RCC->AHB2ENR |= (1 << 1);                      // Habilita reloj GPIOB
    RCC->AHB2ENR |= (1 << 2);                      // Habilita reloj GPIOC
}

void rcc_syscfg_clock_enable(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
}

void rcc_tim3_clock_enable(void)
{
    RCC->APB1ENR1 |= (1 << 1);  // TIM3EN
}