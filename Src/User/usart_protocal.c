#include "usart_protocal.h"
#include "adc_fft.h"
#include "usart.h"
#include "dsp.h"
#include "string.h"


void uart_sent_float(float32_t num)
{
    uint8_t tx_dat[5];
    tx_dat[0] = 0xA5;
    memcpy(tx_dat+1, &num, 4);
    
    // HAL_UART_Transmit_DMA(&huart1, tx_dat, 5);
    HAL_UART_Transmit(&huart1, tx_dat, 5, 1000);
}

void uart_sent_fft ()
{
    uint8_t header = 0xA6;
    HAL_UART_Transmit(&huart1, &header, 1, 1000);
    for (int8_t i = 0; i < FFT_POINTS; i++)
        uart_sent_float(fft_result[i]);
}

uint8_t post_data[] = "device-id=xxxx&header=check";

void uart_http_sent_check(const char* device_id)
{
    memcpy((char*)post_data+10, (const char*)device_id, 4);
    HAL_UART_Transmit(&huart1, post_data, sizeof(post_data)/sizeof(post_data[0]), 1000);
}