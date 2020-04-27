#ifndef ADC_FFT
#define ADC_FFT

#include "adc.h"
#include "main.h"
#include "arm_math.h"

#define ADC_SAMPLE_POINTS   128

extern uint16_t adc_result[ADC_SAMPLE_POINTS];

void init_adc_fft();
void process_fft();

#endif