#pragma once

#include "stm32f10x.h"

#define uart USART1
#define uart_rcc RCC_APB2Periph_USART1
#define uart_baudrate 115200

#define uart_rx_port GPIOA
#define uart_rx_pin GPIO_Pin_10
#define uart_rx_rcc RCC_APB2Periph_GPIOA

#define uart_tx_port GPIOA
#define uart_tx_pin GPIO_Pin_9
#define uart_tx_rcc RCC_APB2Periph_GPIOA

void uart_init();
void uart_send_data(uint8_t data);
void uart_send_string(const char *str);
uint8_t uart_receive_data();