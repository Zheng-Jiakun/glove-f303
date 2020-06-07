#include "main.h"
#include "adc_fft.h"
#include "usart.h"
#include "usart_protocal.h"
#include "string.h"
#include "tim.h"
#include "dsp.h"
#include "nokia5110_LCD.h"

uint8_t training_state = 0;
uint16_t adc_results[5];
uint16_t timestamp;

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
    LCD_print("Waiting for WiFi...", 0, 0);

    HAL_Delay(1000);

    char ip_address[16];
    int8_t wifi_status;

CHECK_WIFI:
    wifi_status = check_wifi(ip_address);

    if (wifi_status == -1)
    {
        LCD_print("Timeout, retrying...", 0, 0);
        goto CHECK_WIFI;
    }
    else if (wifi_status == 1)
    {
        LCD_print("Connected! IP:", 0, 0);
        LCD_print(ip_address, 0, 1);
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
    }

    int8_t register_status = 2;

CHECK_REGISTER:
    register_status = check_register("1234");
    while (register_status == 2)
        ;
    if (register_status == 1)
    {
        LCD_print("  Registered  ", 0, 4);
        LCD_print(" Ready to use ", 0, 5);
        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
    }
    else if (register_status == 0)
    {
        LCD_print("   Added but  ", 0, 4);
        LCD_print("not registered", 0, 5);
    }
    else if (register_status == -1)
    {
        LCD_print(" Not added or ", 0, 4);
        LCD_print("   timeout    ", 0, 5);
        goto CHECK_REGISTER;
    }

    while (register_status != 1 && HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_SET)
        ;

    register_device("1234");

    if (register_status != 1)
    {
        LCD_print("  Retrying..  ", 0, 4);
        LCD_print("              ", 0, 5);
        goto CHECK_REGISTER;
    }

    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_results, 5);
    HAL_TIM_Base_Start_IT(&htim6);
}

void user_loop()
{
    if (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET)
    {
        HAL_Delay(20);
        if (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET)
        {
            HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
            if (training_state == 0)
                training_state = 1;
            else if (training_state == 2)
                training_state = 3;
        }
    }

    static char dat_id[4];

    switch (training_state)
    {
    case 0:
        LCD_print(" Ready to use ", 0, 5);
        break;

    case 1:
        timestamp = 0;
        LCD_print(" Starting...  ", 0, 5);
        sent_training_start("1234", dat_id);
        training_state = 2;
        LCD_print(" Training...  ", 0, 5);
        break;

    case 2:
        sent_training_data(dat_id, adc_results, timestamp);
        break;

    case 3:
        LCD_print(" Finishing... ", 0, 5);
        sent_training_end(dat_id);
        training_state = 0;
        break;
    
    default:
        break;
    }

    // HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
}