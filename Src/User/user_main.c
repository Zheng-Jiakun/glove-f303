#include "main.h"
#include "adc_fft.h"
#include "usart.h"
#include "usart_protocal.h"
#include "string.h"
#include "tim.h"
#include "dsp.h"
#include "dac.h"

uint8_t rx_dat[100];

void user_setup()
{
    init_adc_fft();
    HAL_UART_Receive_DMA(&huart1, rx_dat, 100);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

    fir_lpf_init();
}

void user_loop()
{
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    HAL_Delay(1000);
}