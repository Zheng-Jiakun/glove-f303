#include "main.h"
#include "adc_fft.h"
#include "usart.h"
#include "usart_protocal.h"
#include "string.h"

uint8_t rx_dat[100];

void user_setup()
{
    init_adc_fft();
    HAL_UART_Receive_DMA(&huart1, rx_dat, 100);
}

void user_loop()
{
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    HAL_Delay(1000);
}