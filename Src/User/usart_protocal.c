#include "usart_protocal.h"
#include "adc_fft.h"
#include "usart.h"
#include "dsp.h"
#include "string.h"

void uart_sent_float(float32_t num)
{
    uint8_t tx_dat[5];
    tx_dat[0] = 0xA5;
    memcpy(tx_dat + 1, &num, 4);

    // HAL_UART_Transmit_DMA(&huart1, tx_dat, 5);
    HAL_UART_Transmit(&huart1, tx_dat, 5, 1000);
}

void uart_sent_fft()
{
    uint8_t header = 0xA6;
    HAL_UART_Transmit(&huart1, &header, 1, 1000);
    for (int8_t i = 0; i < FFT_POINTS; i++)
        uart_sent_float(fft_result[i]);
}

int8_t check_wifi(char ip[16])
{
    memset(xtUart1.RxBuf, 0, xtUart1.RxLen);
    uint8_t tx_dat[] = "IP\n";
    Usart1SendData_DMA(tx_dat, 3);
    for (uint8_t i = 0; xtUart1.RxBuf[0] == '\0'; i++)
    {
        HAL_Delay(100);
        if (i >= 30)
            return -1;
    }
    if (xtUart1.Receive)
    {
        xtUart1.Receive = 0;
        strcpy(ip, (const char*)xtUart1.RxBuf);
    }
    return 1;
}

int8_t check_register(const char *device_id)
{
    uint8_t post_data[] = "device-id=xxxx&header=check\n";
    memcpy((char *)post_data + 10, (const char *)device_id, 4);
    memset(xtUart1.RxBuf, 0, xtUart1.RxLen);
    Usart1SendData_DMA(post_data, sizeof(post_data) / sizeof(post_data[0]));
    for (uint8_t i = 0; xtUart1.RxBuf[0] == '\0'; i++)
    {
        HAL_Delay(100);
        if (i >= 30)
            return -1;
    }
    if (strcmp((const char *)xtUart1.RxBuf, "yes") == 0)
        return 1;
    else if (strcmp((const char *)xtUart1.RxBuf, "no") == 0)
        return 0;
    else
        return -1;
}

int8_t register_device(const char *device_id)
{
    uint8_t post_data[] = "device-id=xxxx&header=register\n";
    memcpy((char *)post_data + 10, (const char *)device_id, 4);
    memset(xtUart1.RxBuf, 0, xtUart1.RxLen);
    Usart1SendData_DMA(post_data, sizeof(post_data) / sizeof(post_data[0]));
    for (uint8_t i = 0; strcmp((const char *)xtUart1.RxBuf, "success") != 0; i++)
    {
        HAL_Delay(100);
        if (i >= 30)
            return -1;
    }
    return 1;
}

int8_t sent_training_start(const char *device_id, char data_id[4])
{
    uint8_t post_data[] = "device-id=xxxx&header=training&status=start";
    memcpy((char *)post_data + 10, (const char *)device_id, 4);
    memset(xtUart1.RxBuf, 0, xtUart1.RxLen);
    Usart1SendData_DMA(post_data, sizeof(post_data) / sizeof(post_data[0]));
    for (uint8_t i = 0; xtUart1.RxBuf[0] == '\0'; i++)
    {
        HAL_Delay(100);
        if (i >= 30)
            return -1;
    }
    if (xtUart1.Receive)
    {
        xtUart1.Receive = 0;
        strcpy(data_id, (const char *)xtUart1.RxBuf);
    }
    return 1;
}

int8_t sent_training_end(const char *data_id)
{
    uint8_t post_data[] = "data-id=\0\0\0\0&header=training&status=end";
    strcpy((char *)post_data + 8, data_id);
    memset(xtUart1.RxBuf, 0, xtUart1.RxLen);
    Usart1SendData_DMA(post_data, sizeof(post_data) / sizeof(post_data[0]));
    for (uint8_t i = 0; strcmp((const char *)xtUart1.RxBuf, "success") != 0; i++)
    {
        HAL_Delay(100);
        if (i >= 30)
            return -1;
    }
    return 1;
}

int8_t sent_training_data(const char *data_id, uint16_t data[5], uint16_t timestamp)
{
    char data_str[5][6];
    char time[6];
    for (uint8_t i = 0; i < 5; i++)
        __itoa(data[i], data_str[i], 10);
    __itoa(timestamp, time, 10);

    static uint8_t post_data[] = "data-id=\0\0\0\0&header=training&status=doing&finger1=\0\0\0\0\0\0&finger2=\0\0\0\0\0\0&finger3=\0\0\0\0\0\0&finger4=\0\0\0\0\0\0&finger5=\0\0\0\0\0\0&timestamp=\0\0\0\0\0\0";
    strcpy((char *)post_data + 8, data_id);

    uint8_t training_data[] = "&finger1=\0\0\0\0\0\0&finger2=\0\0\0\0\0\0&finger3=\0\0\0\0\0\0&finger4=\0\0\0\0\0\0&finger5=\0\0\0\0\0\0&timestamp=\0\0\0\0\0\0";
    for (uint8_t i = 0; i < 5; i++)
        strcpy((char *)training_data + 9 + i * 15, (const char *)data_str[i]);
    strcpy((char *)training_data + 86, (const char *)time);

    memcpy((char *)post_data + 41, (const char *)training_data, sizeof(training_data) / sizeof(training_data[0]));

    memset(xtUart1.RxBuf, 0, xtUart1.RxLen);
    Usart1SendData_DMA(post_data, sizeof(post_data) / sizeof(post_data[0]));

    for (uint8_t i = 0; strcmp((const char *)xtUart1.RxBuf, "success") != 0; i++)
    {
        HAL_Delay(100);
        if (i >= 30)
            return -1;
    }
    return 1;
}