#include "gpio.h"                                     // Pin PC13 (Button)

#define GPIO_MODE_INPUT     0x00
#define GPIO_MODE_OUTPUT    0x01
#define GPIO_MODE_AF        0x02
#define GPIO_MODE_ANALOG    0x03

#define GPIO_OTYPE_PP       0x00
#define GPIO_OTYPE_OD       0x01

#define GPIO_OSPEED_LOW     0x00
#define GPIO_OSPEED_MEDIUM  0x01
#define GPIO_OSPEED_HIGH    0x02
#define GPIO_OSPEED_VHIGH   0x03

#define GPIO_PUPD_NONE      0x00
#define GPIO_PUPD_UP        0x01
#define GPIO_PUPD_DOWN      0x02

void gpio_init(GPIO_Typedef_t * GPIO, uint8_t pin, uint8_t mode, uint8_t type, uint8_t speed, uint8_t pupd, uint8_t initial_value)
{
    GPIO->MODER &= ~(3 << (pin*2));           // Limpia bits
    GPIO->MODER |=  (mode << (pin*2));           // Configura modo

    GPIO->TYPER &= ~(1 << pin);           // Limpia bits
    GPIO->TYPER |=  (type << pin);

    GPIO->SPEEDR &= ~(3 << (pin*2));
    GPIO->SPEEDR |= (speed << (pin*2));

    GPIO->PUPDR &= ~(3 << (pin*2));
    GPIO->PUPDR |= (pupd << (pin*2));

    GPIO->ODR &= ~(1 << pin);
    GPIO->ODR |= (initial_value << pin);
}

void gpio_setup_pin(GPIO_Typedef_t * GPIO, uint8_t pin, uint8_t mode, uint8_t af_num)
{
    // Set mode
    GPIO->MODER &= ~(3U << (pin * 2U));
    GPIO->MODER |= (mode << (pin * 2U));

    // Set alternate function
    if (pin < 8) {
        GPIO->AFRL &= ~(0xFU << (pin * 4U));
        GPIO->AFRL |= (af_num << (pin * 4U));
    } else {
        GPIO->AFRH &= ~(0xFU << ((pin - 8) * 4U));
        GPIO->AFRH |= (af_num << ((pin - 8) * 4U));
    }
}

void gpio_set(GPIO_Typedef_t * GPIO, uint8_t pin)
{
    GPIO->ODR |= (1 << pin);
}

void gpio_clear(GPIO_Typedef_t * GPIO, uint8_t pin)
{
    GPIO->ODR &= ~(1 << pin);
}

void gpio_toggle(GPIO_Typedef_t * GPIO, uint8_t pin)
{
    GPIO->ODR ^= (1 << pin);
}

uint8_t gpio_read(GPIO_Typedef_t * GPIO, uint8_t pin)
{
    // Leer estado del botón PC13 (botón presionado = 0, no presionado = 1)
    if ((GPIO->IDR & (1 << pin)) == 0) { // pressed
        return 1;
    }
    return 0;
}

