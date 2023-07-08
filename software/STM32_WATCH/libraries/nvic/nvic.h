#pragma once
#include "stm32f10x.h"
#define NVIC_PriorityGroup NVIC_PriorityGroup_0

typedef enum
{
    NV_TIM2,
} NVIC_Selection;

void NVIC_GroupInit();
void NVIC_Config(NVIC_Selection nvic_selection);