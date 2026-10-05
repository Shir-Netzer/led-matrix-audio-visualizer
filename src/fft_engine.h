#ifndef FFT_ENGINE_H
#define FFT_ENGINE_H

#include <Arduino.h>
#include <driver/i2s.h>
#include <math.h>

#define I2S_WS      15
#define I2S_SD      5
#define I2S_SCK     4
#define SAMPLE_RATE 16000
#define SAMPLES     64

typedef struct {
    float real; 
    float imag; 
} Complex;

// המערך של הדגימות
extern Complex fft_input[SAMPLES];

// הצהרות בלבד על הפונקציות
void initI2S();
void bitReversal(Complex *v, int N);
void computeFFT(Complex *v, int N);
void fft_func(int32_t sample, int i);

#endif