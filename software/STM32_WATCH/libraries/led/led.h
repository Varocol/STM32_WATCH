#ifndef __LED_H
#define __LED_H
#include "stm32f10x.h"

#define led_port GPIOB
#define led_pin  GPIO_Pin_9
#define led_rcc  RCC_APB2Periph_GPIOB

void led_init();
void led_on();
void led_off();
void led_toggle();

#endif // __LED_H