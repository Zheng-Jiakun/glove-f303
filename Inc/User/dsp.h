#ifndef DSP_H
#define DSP_H

#include "main.h"
#include "adc_fft.h"
#include "dac.h"

void fir_lpf_init();
void fir_lpf();
void dac_get_input();

extern float32_t fir_output[ADC_SAMPLE_POINTS];

#endif