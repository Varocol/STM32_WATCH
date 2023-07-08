#include "led.h"

/**
 * @brief  LED-初始化
 * @retval None
 */
void led_init()
{
    GPIO_InitTypeDef led_gpio_initstructure = {
        .GPIO_Pin = led_pin,
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode = GPIO_Mode_Out_PP,
    };
    RCC_APB2PeriphClockCmd(led_rcc, ENABLE);
    GPIO_Init(led_port, &led_gpio_initstructure);
}

/**
 * @brief  LED-开启
 * @retval None
 */
void led_on()
{
    GPIO_WriteBit(led_port, led_pin, Bit_RESET);
}

/**
 * @brief  LED-关闭
 * @retval None
 */
void led_off()
{
    GPIO_WriteBit(led_port, led_pin, Bit_SET);
}

/**
 * @brief  LED-反转
 * @retval None
 */
void led_toggle()
{
    uint8_t pin_val = GPIO_ReadOutputDataBit(led_port, led_pin);
    GPIO_WriteBit(led_port, led_pin, pin_val == 0x00 ? Bit_SET : Bit_RESET);
}