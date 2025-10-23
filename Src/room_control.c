#include "room_control.h"

#include "gpio.h"    // Para controlar LEDs
#include "systick.h" // Para obtener ticks y manejar tiempos
#include "uart.h"    // Para enviar mensajes
#include "tim.h"     // Para controlar el PWM

static volatile uint32_t g_door_open_tick = 0;
static volatile uint32_t g_door_open_tick_2 = 0;
static volatile uint32_t duty_cycle_percent = 20;
static volatile uint8_t g_door_open = 0;
static volatile uint32_t g_last_button_tick = 0;
static volatile uint32_t g_led_increment = 0;

// Estados de la sala
typedef enum {
    ROOM_IDLE,
    ROOM_OCCUPIED
} room_state_t;

// Variable de estado global
room_state_t current_state = ROOM_IDLE;
static uint32_t led_on_time = 0;

void room_control_app_init(void)
{
    uart_send_string("Controlador de sala v2.0\r\n Estado inicial:\r\n  - Lámpara: 20%\r\n  - Puerta: Cerrada\r\n ");
    // Inicializar PWM al duty cycle inicial (estado IDLE -> LED apagado)
    tim3_ch1_pwm_set_duty_cycle(PWM_INITIAL_DUTY);
}

void room_control_on_button_press(void)
{
    if (current_state == ROOM_IDLE) {
        current_state = ROOM_OCCUPIED;
        tim3_ch1_pwm_set_duty_cycle(100);  // PWM al 100%
        led_on_time = systick_get_ms();
        uart_send_string("Sala ocupada\r\n");
    } else {
        current_state = ROOM_IDLE;
        tim3_ch1_pwm_set_duty_cycle(0);  // PWM al 0%
        uart_send_string("Sala vacía\r\n");
    }
}

void room_control_on_uart_receive(char received_char)
{
    switch (received_char) {
        case 'h':
        case 'H':
            tim3_ch1_pwm_set_duty_cycle(100);
            uart_send_string("PWM: 100%\r\n");
            break;
        case 'l':
        case 'L':
            tim3_ch1_pwm_set_duty_cycle(0);
            uart_send_string("PWM: 0%\r\n");
            break;
        case 'O':
        case 'o':
            current_state = ROOM_OCCUPIED;
            tim3_ch1_pwm_set_duty_cycle(100);
            led_on_time = systick_get_ms();
            uart_send_string("Sala ocupada\r\n");
            break;
        case 'I':
        case 'i':
            current_state = ROOM_IDLE;
            tim3_ch1_pwm_set_duty_cycle(0);
            uart_send_string("Sala vacía\r\n");
            break;
        case '1':
            tim3_ch1_pwm_set_duty_cycle(10);
            uart_send_string("PWM: 10%\r\n");
            break;
        case '2':
            tim3_ch1_pwm_set_duty_cycle(20);
            uart_send_string("PWM: 20%\r\n");
            break;
        case '3':
            tim3_ch1_pwm_set_duty_cycle(30);
            uart_send_string("PWM: 30%\r\n");
            break;
        case '4':
            tim3_ch1_pwm_set_duty_cycle(40);
            uart_send_string("PWM: 40%\r\n");
            break;
        case '5':
            tim3_ch1_pwm_set_duty_cycle(50);
            uart_send_string("PWM: 50%\r\n");
            break;
        default:
            uart_send_string("Comando desconocido: ");
            uart_send(received_char);
            uart_send_string("\r\n");
            break;
        case 's':
        case 'S':
            uart_send_string("Estado actual:\r\n");
            uart_send_string("Lámpara: ");
            uart2_send_string(duty_cycle_percent);
            uart_send_string("%\r\n");
            if (g_door_open == 0){
                uart_send_string("Puerta: Cerrada\r\n");
            }
            else {
                uart_send_string("Puerta: Abierta.\r\n");
            }
            break;
        case '?':
            uart_send_string("Comandos disponibles:\r\n");
            uart_send_string("'1'-'5': Ajustar brillo lámpara (10%, 20%, 30%, 40%), 50%\r\n");
            uart_send_string("'0'   : Apagar lámpara\r\n");
            uart_send_string("'o', 'O'  : Abrir puerta\r\n");
            uart_send_string("'c', 'C'  : Cerrar puerta\r\n");
            uart_send_string("'s', 'S'  : Estado del sistema\r\n");
            uart_send_string("'g', 'G'  : Aumentar gradualmente el brillo de la lámpara\r\n");
            uart_send_string("'?'   : Ayuda\r\n");
            break;
        
        case 'g':
        case 'G':
            g_led_increment = systick_get_ms();
            uart_send_string("Aumentando gradualmente el brillo de la lámpara\r\n");
            for (int i = 10; i <= 100; i += 1) {
                if (i % 10 == 0 && (systick_get_ms() - g_led_increment >= 500)) {
                    tim3_ch1_pwm_set_duty_cycle(i);
                    g_led_increment=systick_get_ms();
                }
            }    
            
    }
}

void room_control_update(void)
{
    if (current_state == ROOM_OCCUPIED) {
        if (systick_get_ms() - led_on_time >= LED_TIMEOUT_MS) {
            current_state = ROOM_IDLE;
            tim3_ch1_pwm_set_duty_cycle(0);
            uart_send_string("Timeout: Sala vacía\r\n");
        }
    }
}

