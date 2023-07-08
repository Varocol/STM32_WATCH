#pragma once

#include "variables.h"
#include "stm32f10x.h"
#include "nvic.h"

#define key1_port GPIOA
#define key1_pin GPIO_Pin_6
#define key1_rcc RCC_APB2Periph_GPIOA

#define key2_port GPIOA
#define key2_pin GPIO_Pin_4
#define key2_rcc RCC_APB2Periph_GPIOA

#define key3_port GPIOA
#define key3_pin GPIO_Pin_5
#define key3_rcc RCC_APB2Periph_GPIOA

#define key4_port GPIOC
#define key4_pin GPIO_Pin_13
#define key4_rcc RCC_APB2Periph_GPIOC

#define key_tim TIM2
#define key_rcc_periph RCC_APB1Periph_TIM2
#define key_polling_ms 1

void keys_init();
void keys_intterupt_init();
void keys_intterupt_function();