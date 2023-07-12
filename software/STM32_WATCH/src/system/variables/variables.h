#pragma once
#include "libraries.h"

extern volatile Key_Status key1_status;
extern volatile Key_Status key2_status;
extern volatile Key_Status key3_status;
extern volatile Key_Status key4_status;

extern volatile uint32_t key1_status_time;
extern volatile uint32_t key2_status_time;
extern volatile uint32_t key3_status_time;
extern volatile uint32_t key4_status_time;

extern volatile uint8_t beep_volume;
extern struct beep_song tip_tone;

extern RCC_ClocksTypeDef RCC_Clocks;