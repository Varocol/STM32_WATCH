#include "variables.h"

// key 关联变量
volatile Key_Status key1_status = Released;
volatile Key_Status key2_status = Released;
volatile Key_Status key3_status = Released;
volatile Key_Status key4_status = Released;

volatile uint32_t key1_status_time = 0;
volatile uint32_t key2_status_time = 0;
volatile uint32_t key3_status_time = 0;
volatile uint32_t key4_status_time = 0;

// beep 关联变量
volatile uint8_t beep_volume = 15;
struct beep_song tip_tone = {
    .volume = 100,
    .Basicspeed = 80,
    .tone = B,
    .octachord = 2,
    .name = "提示音",
    .data = {
        0x17,
        0x02,
        0x00,
        0x00,
    },
};

// clock 关联变量
RCC_ClocksTypeDef RCC_Clocks;
