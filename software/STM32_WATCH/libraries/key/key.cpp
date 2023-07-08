#include "key.h"

/**
 * @brief  按键初始化
 * @retval None
 */
void keys_init()
{
    GPIO_InitTypeDef key_gpio_initstructure = {
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode = GPIO_Mode_IN_FLOATING,
    };
    // key1 初始化
    key_gpio_initstructure.GPIO_Pin = key1_pin;
    RCC_APB2PeriphClockCmd(key1_rcc, ENABLE);
    GPIO_Init(key1_port, &key_gpio_initstructure);
    // key2 初始化
    key_gpio_initstructure.GPIO_Pin = key2_pin;
    RCC_APB2PeriphClockCmd(key2_rcc, ENABLE);
    GPIO_Init(key2_port, &key_gpio_initstructure);
    // key3 初始化
    key_gpio_initstructure.GPIO_Pin = key3_pin;
    RCC_APB2PeriphClockCmd(key3_rcc, ENABLE);
    GPIO_Init(key3_port, &key_gpio_initstructure);
    // key4 初始化
    key_gpio_initstructure.GPIO_Pin = key4_pin;
    RCC_APB2PeriphClockCmd(key4_rcc, ENABLE);
    GPIO_Init(key4_port, &key_gpio_initstructure);
}

/**
 * @brief  按键中断初始化(定时器中断轮询)
 * @retval None
 */
void keys_intterupt_init()
{
    // 获取系统时钟频率
    RCC_ClocksTypeDef RCC_Clocks;
    RCC_GetClocksFreq(&RCC_Clocks);
    // 配置TIM2
    TIM_TimeBaseInitTypeDef key_tim_TimeBaseInitstructure = {
        .TIM_Prescaler = (uint16_t)(RCC_Clocks.SYSCLK_Frequency / 1000000), // 72
        .TIM_CounterMode = TIM_CounterMode_Up,
        .TIM_Period = 1000 - key_polling_ms, // 计1000次,这样每次1ms
        .TIM_ClockDivision = TIM_CKD_DIV1,   // 输入信号滤波频率
        .TIM_RepetitionCounter = 0           // 重复次数,只用通用和高级定时器才有
    };
    // 开启RCC
    RCC_APB1PeriphClockCmd(key_rcc_periph, ENABLE);
    // TIMx寄存器复位
    TIM_DeInit(key_tim);
    // 配置基本参数
    TIM_TimeBaseInit(key_tim, &key_tim_TimeBaseInitstructure);
    // 清除计时器中断标志位
    TIM_ClearFlag(key_tim, TIM_FLAG_Update);
    // 配置中断向量
    NVIC_Config(NV_TIM2);
    // 配置计时器中断
    TIM_ITConfig(key_tim, TIM_IT_Update, ENABLE);
    // 开启定时器
    TIM_Cmd(key_tim, ENABLE);
}

/**
 * @brief  按键中断服务函数
 * @retval None
 */
void keys_intterupt_function()
{
    
}