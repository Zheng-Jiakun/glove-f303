# glove-f303

This is the source code of a Smart Glove product for ISDN2002 course project in Division of ISD, HKUST. 

MCU: STM32F303VCT6
Main features: ADC sampling for flex sensors
Signal processing: FFT/ FIR LPF (implemented with HAL DSP library), mean filter, difference complementary filter
Communication: a) Physical layer: USART + ESP8266;  b)Application layer: HTTP
User interface: Nokia 5110 84*48 LCD display
