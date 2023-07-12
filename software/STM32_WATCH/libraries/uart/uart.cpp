#include "uart.h"

/**
 * @brief  USART-初始化
 * @param  None
 * @retval None
 */
void uart_init()
{
    // 开启RCC时钟
    if (uart != USART1)
    {
        RCC_APB1PeriphClockCmd(uart_rcc, ENABLE);
    }
    else
    {
        RCC_APB2PeriphClockCmd(uart_rcc, ENABLE);
    }
    // USARTx寄存器复位
    USART_DeInit(uart);
    // 引脚初始化
    GPIO_InitTypeDef uart_gpio_initstructure = {
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode = GPIO_Mode_IN_FLOATING,
    };
    // uart_rx 初始化
    uart_gpio_initstructure.GPIO_Pin = uart_rx_pin;
    RCC_APB2PeriphClockCmd(uart_rx_rcc, ENABLE);
    GPIO_Init(uart_rx_port, &uart_gpio_initstructure);

    // uart_tx 初始化
    uart_gpio_initstructure.GPIO_Mode = GPIO_Mode_AF_PP;
    uart_gpio_initstructure.GPIO_Pin = uart_tx_pin;
    // 开启AFIO复用时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(uart_tx_rcc, ENABLE);
    GPIO_Init(uart_tx_port, &uart_gpio_initstructure);

    // 配置串口
    USART_InitTypeDef uart_initstructure = {
        .USART_BaudRate = uart_baudrate,
        .USART_WordLength = USART_WordLength_8b,
        .USART_StopBits = USART_StopBits_1,
        .USART_Parity = USART_Parity_No,
        .USART_Mode = USART_Mode_Rx | USART_Mode_Tx,
        .USART_HardwareFlowControl = USART_HardwareFlowControl_None};
    // 初始化USART
    USART_Init(uart, &uart_initstructure);
    // 开启USART
    USART_Cmd(uart, ENABLE);
}

/**
 * @brief  USART-发送字节
 * @param  data             数据
 * @retval None
 */
void uart_send_data(uint8_t data)
{
    USART_SendData(uart, data);
    // 这个地方一定要同时使用,为了防止串口发送第一个数据时丢失以及最后一个数据时间不够丢失。
    while (USART_GetFlagStatus(uart, USART_FLAG_TXE) == RESET)
        ;
    while (USART_GetFlagStatus(uart, USART_FLAG_TC) == RESET)
        ;
}

/**
 * @brief  USART-接收字节
 * @param  None
 * @retval 接收缓冲器的值
 */
uint8_t uart_receive_data()
{
    while (USART_GetFlagStatus(uart, USART_FLAG_RXNE) == RESET)
        ;
    return USART_ReceiveData(uart);
}

/**
 * @brief  USART-发送字符串
 * @param  str              字符串
 * @retval None
 */
void uart_send_string(const char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        uart_send_data(str[i]);
    }
}
