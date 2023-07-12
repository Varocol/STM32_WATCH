#pragma once

#include "stm32f10x.h"
#define NVIC_PriorityGroup NVIC_PriorityGroup_0

typedef enum
{
    NV_TIM1_UP,
    NV_TIM3,
} NVIC_Selection;

void nvic_group_init();
void nvic_config(NVIC_Selection nvic_selection);