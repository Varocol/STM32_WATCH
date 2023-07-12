#include "nvic.h"

// 中断向量列表(与NVIC_Selection对应)
uint8_t NVIC_IRQChannel_list[] = {
    TIM1_UP_IRQn,
    TIM3_IRQn,
};

/**
 * @brief  中断向量组别初始化
 * @retval None
 */
void nvic_group_init()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup);
}

/**
 * @brief  中断向量初始化
 * @param  nvic_selection   中断向量选择
 * @retval None
 */
void nvic_config(NVIC_Selection nvic_selection)
{
    NVIC_InitTypeDef NVIC_InitStructure = {
        .NVIC_IRQChannel = NVIC_IRQChannel_list[nvic_selection],
        .NVIC_IRQChannelPreemptionPriority = 0,
        .NVIC_IRQChannelSubPriority = (uint8_t)nvic_selection,
        .NVIC_IRQChannelCmd = ENABLE};
    NVIC_Init(&NVIC_InitStructure);
}