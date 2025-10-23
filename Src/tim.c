#include "tim.h"
#include "rcc.h"  // Para rcc_tim3_clock_enable
#include "gpio.h" // Para gpio_setup_pin

void tim3_ch1_pwm_init(uint32_t pwm_freq_hz)
{
    // 1. Configurar PB4 como Alternate Function (AF2) para TIM3_CH1  PARCIAL
    gpio_setup_pin(GPIOB, 4, GPIO_MODE_AF, 2);

    // 2. Habilitar el reloj para TIM3
    rcc_tim3_clock_enable();

    // 3. Configurar TIM3
    TIM3->PSC = 100 - 1; // (4MHz / 1 = 4MHz)
    TIM3->ARR = (TIM_PCLK_FREQ_HZ / 100 / pwm_freq_hz) - 1; // 4MHz / pwm_freq_hz

    // Configurar el Canal 1 (CH1) en modo PWM 1
    TIM3->CCMR1 = (6U << 4);                    // PWM mode 1 on CH1
    TIM3->CCER  |= (1 << 0);                    // Enable CH1 output

    // Finalmente, habilitar el contador del timer
    TIM3->CR1 |= 0x01 << 0;
}

void tim3_ch1_pwm_set_duty_cycle(uint8_t duty_cycle_percent)
{
    if (duty_cycle_percent > 100) {
        duty_cycle_percent = 100;
    }

    // Calcular el valor de CCR1 basado en el porcentaje y el valor de ARR
    uint16_t tim3_ch1_arr_value = TIM3->ARR;
    uint32_t ccr_value = (((uint32_t)tim3_ch1_arr_value + 1U) * duty_cycle_percent) / 100U;

    TIM3->CCR1 = ccr_value;
}
