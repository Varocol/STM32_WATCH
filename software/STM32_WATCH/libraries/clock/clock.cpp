#include "clock.h"

/**
 * @brief       系统时钟初始化
 * @retval      None
 */
void clock_init()
{
    // 获取系统时钟频率（SYSCLK、HCLK、PCLK1、PCLK2、ADCCLK）
    RCC_GetClocksFreq(&RCC_Clocks);
}
