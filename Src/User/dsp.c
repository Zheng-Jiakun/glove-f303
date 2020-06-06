#include "dsp.h"

/* ----------------------------------------------------------------------
** Macro Defines
** ------------------------------------------------------------------- */

#define BLOCK_SIZE            ADC_SAMPLE_POINTS
#define NUM_TAPS              29
// #define SNR_THRESHOLD_F32    140.0f

/* -------------------------------------------------------------------
 * Declare Test output buffer
 * ------------------------------------------------------------------- */

float32_t fir_input[ADC_SAMPLE_POINTS];
float32_t fir_output[ADC_SAMPLE_POINTS];
// float32_t  snr;

uint16_t dac_dat[ADC_SAMPLE_POINTS];

/* -------------------------------------------------------------------
 * Declare State buffer of size (numTaps + blockSize - 1)
 * ------------------------------------------------------------------- */

static float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1];

/* ----------------------------------------------------------------------
** FIR Coefficients buffer generated using fir1() MATLAB function.
** fir1(28, 6/24)
** ------------------------------------------------------------------- */

// const float32_t firCoeffs32[NUM_TAPS] = {
//   -0.0018225230f, -0.0015879294f, +0.0000000000f, +0.0036977508f, +0.0080754303f, +0.0085302217f, -0.0000000000f, -0.0173976984f,
//   -0.0341458607f, -0.0333591565f, +0.0000000000f, +0.0676308395f, +0.1522061835f, +0.2229246956f, +0.2504960933f, +0.2229246956f,
//   +0.1522061835f, +0.0676308395f, +0.0000000000f, -0.0333591565f, -0.0341458607f, -0.0173976984f, -0.0000000000f, +0.0085302217f,
//   +0.0080754303f, +0.0036977508f, +0.0000000000f, -0.0015879294f, -0.0018225230f
// };

//below is 1k filter for 100k sample rate
// const float32_t firCoeffs32[NUM_TAPS] = {
//     0.0047f, 0.0055f, 0.0077f, 0.0112f, 0.0159f, 0.0217f, 0.0282f, 0.0351f, 
//     0.0422f, 0.0489f, 0.0550f, 0.0601f, 0.0640f, 0.0664f, 0.0672f, 0.0664f, 
//     0.0640f, 0.0601f, 0.0550f, 0.0489f, 0.0422f, 0.0351f, 0.0282f, 0.0217f,
//     0.0159f, 0.0112f, 0.0077f, 0.0055f, 0.0047f
// };
//below is 400hz
// const float32_t firCoeffs32[NUM_TAPS] = {
//     0.0052f, 0.0060f, 0.0082f, 0.0118f, 0.0166f, 0.0224f, 0.0288f, 0.0355f, 
//     0.0423f, 0.0487f, 0.0544f, 0.0592f, 0.0628f, 0.0651f, 0.0658f, 0.0651f, 
//     0.0628f, 0.0592f, 0.0544f, 0.0487f, 0.0423f, 0.0355f, 0.0288f, 0.0244f, 
//     0.0166f, 0.0118f, 0.0082f, 0.0060f, 0.0052f
// };
//10hz
const float32_t firCoeffs32[NUM_TAPS] = {
    0.0053f, 0.0060f, 0.0083f, 0.0119f, 0.0167f, 0.0224f, 0.0288f, 
    0.0355f, 0.0423f, 0.0487f, 0.0544f, 0.0592f, 0.0628f, 0.0650f, 
    0.0658f, 0.0650f, 0.0628f, 0.0592f, 0.0544f, 0.0487f, 0.0423f, 
    0.0355f, 0.0288f, 0.0224f, 0.0167f, 0.0119f, 0.0083f, 0.0060f, 0.0053f
};


/* ----------------------------------------------------------------------
 * FIR LPF Example
 * ------------------------------------------------------------------- */

arm_fir_instance_f32 S;
arm_status status;
float32_t  *inputF32, *outputF32;

void fir_lpf_init()
{
    /* Initialize input and output buffer pointers */
    outputF32 = &fir_output[0];
    inputF32 = &fir_input[0];
    /* Call FIR init function to initialize the instance structure. */
    arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32[0], &firStateF32[0], BLOCK_SIZE);

    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)dac_dat, ADC_SAMPLE_POINTS, DAC_ALIGN_12B_R);
}

void fir_lpf()
{
  /* ----------------------------------------------------------------------
  ** Call the FIR process function for every blockSize samples
  ** ------------------------------------------------------------------- */
    for (uint16_t i = 0; i < ADC_SAMPLE_POINTS; i++)
        inputF32[i] = adc_result[i];

    arm_fir_f32(&S, inputF32, outputF32, BLOCK_SIZE);

    // snr = arm_snr_f32(&refOutput[0], outputF32, ADC_SAMPLE_POINTS);

    // if (snr < SNR_THRESHOLD_F32)
    
    //     status = ARM_MATH_TEST_FAILURE;
    // else
    //     status = ARM_MATH_SUCCESS;
    
    // for (uint16_t i = 0; i < ADC_SAMPLE_POINTS; i++)
    //     outputF32[i] *= 2.0f;
}

uint32_t dac_out;

void dac_get_input()
{
    for (uint16_t i = 0; i < ADC_SAMPLE_POINTS; i++)
        dac_dat[i] = fir_output[i];

    dac_out = HAL_DAC_GetValue(&hdac, DAC_CHANNEL_1);
}