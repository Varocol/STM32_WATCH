#pragma once
#include "stm32f10x.h"
#include "variables.h"
#include "stdio.h"
#include <queue>
using namespace std;

/*
   以下模板详见:
   https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/tutorial/beep-player/decode
*/

#define beep_port GPIOA
#define beep_pin GPIO_Pin_1
#define beep_rcc RCC_APB2Periph_GPIOA

#define beep_tim_counter TIM1
#define beep_tim_counter_rcc RCC_APB2Periph_TIM1

#define beep_tim_output TIM2
#define beep_tim_output_channel 2
#define beep_tim_output_rcc RCC_APB1Periph_TIM2

/*蜂鸣器音乐参数*/
#define SONG_NAME_LENGTH_MAX 30
#define SONG_DATA_LENGTH_MAX 500
#define SOUND_SPACE 0.8
/*音调*/
typedef enum
{
    C,
    uC,
    D,
    uD,
    E,
    F,
    uF,
    G,
    uG,
    A,
    uA,
    B
    /*对应C、#C、D、#D、E、F、#F、G、A、#A、B调*/
} Tone;
/*乐曲信息*/
struct beep_song
{
    const uint8_t volume;                     // 音量(这里留了音量的接口，但会在系统层与beep_volume强行同步，也就是这个值没有用)
    const uint16_t Basicspeed;                // 这里的基础节拍是以全音符为单位，例如40表示一分钟40个全音符
    const Tone tone;                          // 基础音调
    const int8_t octachord;                   // 升降八度(-2到+2)
    const uint8_t name[SONG_NAME_LENGTH_MAX]; // 歌曲名
    const uint8_t data[SONG_DATA_LENGTH_MAX]; // 歌曲编码
};

/*每一个音符的信息*/
struct beep_song_data
{
    uint16_t freq;        // 音符频率
    uint16_t sound_len;   // 音符有声长度
    uint16_t nosound_len; // 音符无声长度
};

/*pwm帧信息*/
struct pwm_frame
{
    uint16_t freq; // 帧频率
    uint16_t len;  // 帧长度
};

/*C调基础频率表*/
static const uint16_t freq_tab[12] = {262, 277, 294, 311, 330, 349, 369, 392, 415, 440, 466, 494}; // 频率表 CDEFGAB
/*1~7在频率表中的位置*/
static const uint8_t sign_tab[7] = {0, 2, 4, 5, 7, 9, 11};
/*几分音符 2^0,2^1,2^2,2^3...*/
static const uint8_t length_tab[7] = {1, 2, 4, 8, 16, 32, 64};

void beep_init();
void beep_interrupt_init();
void beep_interrupt_function();

void beep_tip();
void beep_music(const struct beep_song *song);