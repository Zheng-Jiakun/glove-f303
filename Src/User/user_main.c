#include "main.h"
#include "adc_fft.h"
#include "usart.h"

uint8_t rx_dat[100];

void user_setup()
{
    init_adc_fft();
    // HAL_UART_Transmit_DMA(&huart1, adc_result, ADC_SAMPLE_POINTS);
    HAL_UART_Receive_DMA(&huart1, &rx_dat, 100);
}

void user_loop()
{
}