#include "adc_fft.h"

uint16_t adc_results[5];

uint16_t adc_result[ADC_SAMPLE_POINTS];
float32_t fft_input[ADC_SAMPLE_POINTS];
float32_t fft_output[ADC_SAMPLE_POINTS];
float32_t fft_result[FFT_POINTS];


arm_rfft_fast_instance_f32 s;

void init_adc_fft()
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_results, 5);
    HAL_TIM_Base_Start_IT(&htim6);

    // HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_result, ADC_SAMPLE_POINTS);
    // HAL_TIM_Base_Start(&htim6);
    // arm_rfft_fast_init_f32(&s, ADC_SAMPLE_POINTS);
}

void process_fft()
{
    for (uint16_t i = 0; i < ADC_SAMPLE_POINTS; i++)
        fft_input[i] = adc_result[i];
    arm_rfft_fast_f32(&s, fft_input, fft_output, 0);

  /* Process the data through the Complex Magnitude Module for
  calculating the magnitude at each bin */
    arm_cmplx_mag_f32(fft_output, fft_result, FFT_POINTS);

  /* Calculates maxValue and returns corresponding BIN value */
//   arm_max_f32(testOutput, fftSize, &maxValue, &testIndex);
}
