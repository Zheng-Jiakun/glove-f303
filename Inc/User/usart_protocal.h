#ifndef USART_PROTOCAL
#define USART_PROTOCAL

#include "stm32f3xx.h"
#include "main.h"
#include "stdlib.h"

void uart_sent_fft();


int8_t check_wifi(char ip[16]);
int8_t check_register(const char *device_id);
int8_t register_device(const char *device_id);
int8_t sent_training_start(const char *device_id, char data_id[4]);
int8_t sent_training_end(const char *data_id);
int8_t sent_training_data(const char *data_id, uint16_t data[5], uint16_t timestamp);

#endif