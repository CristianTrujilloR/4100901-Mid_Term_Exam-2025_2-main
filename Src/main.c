#include "gpio.h"
#include "systick.h"
#include "rcc.h"
#include "uart.h"
#include "nvic.h"
#include "tim.h"
#include "room_control.h"

// Flags para eventos
volatile uint8_t button_event = 0;
volatile char uart_event_char = 0;

// Contador de milisegundos del sistema
volatile uint32_t system_ms_counter = 0;

// Función local para inicializar periféricos
static void peripherals_init(void)
{
    // Inicialización del sistema
    rcc_init();

    // Configuración de GPIOs
    gpio_init(GPIOA, 5, 0x01, 0x00, 0x01, 0x00, 0x00);  // LD2 (heartbeat)
    gpio_init(GPIOC, 13, 0x00, 0x00, 0x01, 0x00, 0x00);  // Botón

    // Inicialización de periféricos
    init_systick();
    init_gpio_uart();  // Inicialización GPIO para UART
    init_uart();       // Inicialización UART
    nvic_exti_pc13_button_enable();
    nvic_usart2_irq_enable();
    tim3_ch1_pwm_init(1000);  // 1 kHz PWM
}

static void heartbeat_toggle(void)
{
    static uint32_t last_toggle_time = 0;
    if ((system_ms_counter - last_toggle_time) >= 500) { // Toggle cada 500 ms
        gpio_toggle(GPIOA, 5);
        last_toggle_time = system_ms_counter;
    }
}

int main(void)
{
    peripherals_init();
    room_control_app_init();
    uart_send_string("Sistema de Control de Sala Inicializado!\r\n");

    // Bucle principal: procesa eventos
    while (1) {
        heartbeat_toggle();

        if (button_event) {
            button_event = 0;
            room_control_on_button_press();
        }
        if (uart_event_char) {
            char c = uart_event_char;
            uart_event_char = 0;
            room_control_on_uart_receive(c);
        }
        // Llamar a la función de actualización periódica
        room_control_update();
    }
}

// Manejador de SysTick
void SysTick_Handler(void)
{
    system_ms_counter++;
}

// Manejadores de interrupciones
void EXTI15_10_IRQHandler(void)
{
    // Limpiar flag de interrupción
    if (EXTI->PR1 & (1 << 13)) {
        EXTI->PR1 |= (1 << 13);  // Clear pending
        button_event = 1;
    }
}

void USART2_IRQHandler(void)
{
    // Verificar si es recepción
    if (USART2->ISR & (1 << 5)) {  // RXNE
        uart_event_char = (char)(USART2->RDR & 0xFF);
    }
}