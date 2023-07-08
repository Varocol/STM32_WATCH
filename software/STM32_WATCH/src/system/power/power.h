#pragma once
#include "delay.h"

#define pwr_port GPIOB
#define pwr_pin GPIO_Pin_7
#define pwr_rcc RCC_APB2Periph_GPIOB
#define pwr_on_delay_ms 200

void power_on();
void power_off();
