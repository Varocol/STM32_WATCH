#include "power.h"

/**
 * @brief  开机
 * @retval None
 */
void power_on()
{
    // 延时一段时间再开机
    delay_ms(pwr_on_delay_ms);
    // 开机初始化引脚
    GPIO_InitTypeDef pwr_gpio_initstructure = {
        .GPIO_Pin = pwr_pin,
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode = GPIO_Mode_Out_PP,
    };
    RCC_APB2PeriphClockCmd(pwr_rcc, ENABLE);
    GPIO_Init(pwr_port, &pwr_gpio_initstructure);
    GPIO_SetBits(pwr_port, pwr_pin);
}

/**
 * @brief  关机
 * @retval None
 */
void power_off()
{
    
}
