#ifndef USART_PROTOCAL
#define USART_PROTOCAL

#include "stm32f3xx.h"
#include "main.h"

void uart_sent_fft();
void uart_http_sent_check(const char* device_id);
void UsartReceive_IDLE(UART_HandleTypeDef *huart);

#endif