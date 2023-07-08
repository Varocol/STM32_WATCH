#pragma once

#include "stm32f10x.h"
#include "core_cm3.h"

void delay_ms(uint32_t ms);
void delay_us(uint32_t us);
void delay(uint32_t count, uint32_t ticks);