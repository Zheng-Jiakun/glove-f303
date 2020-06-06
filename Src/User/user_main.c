#include "main.h"
#include "adc_fft.h"
#include "usart.h"
#include "usart_protocal.h"
#include "string.h"
#include "tim.h"
#include "dsp.h"
#include "dac.h"
#include "nokia5110_LCD.h"

void user_setup()
{
    // init_adc_fft();
    // HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

    // fir_lpf_init();

    LCD_setRST(GPIOC, GPIO_PIN_15);
    LCD_setCE(GPIOC, GPIO_PIN_14);
    LCD_setDC(GPIOC, GPIO_PIN_13);
    LCD_setDIN(GPIOC, GPIO_PIN_12);
    LCD_setCLK(GPIOC, GPIO_PIN_10);

    LCD_init();
    LCD_print("No WiFi", 0, 0);
}

void user_loop()
{
    uint8_t tx_dat[] = "IP\n";
    if (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET)
    {
        HAL_Delay(20);
        if (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET)
        {
            Usart1SendData_DMA(tx_dat, 3); //串口打印收到的数据。
            // while (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET)
                // ;
        }
    }
    // if (xtUart1.Receive) //如果产生了空闲中断
    // {
    //     xtUart1.Receive = 0;                              //清零标记
    //     Usart1SendData_DMA(xtUart1.RxBuf, xtUart1.RxLen); //串口打印收到的数据。
    // }
    // HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    // uart_http_sent_check("2345");
    // HAL_Delay(2000);
}