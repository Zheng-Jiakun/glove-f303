#ifndef ADC_FFT
#define ADC_FFT

#include "adc.h"
#include "main.h"
#include "arm_math.h"
#include "tim.h"

#define ADC_SAMPLE_POINTS   32
#define FFT_POINTS  ADC_SAMPLE_POINTS/2

extern uint16_t adc_result[ADC_SAMPLE_POINTS];
extern float32_t fft_result[ADC_SAMPLE_POINTS/2];

void init_adc_fft();
void process_fft();

#endif