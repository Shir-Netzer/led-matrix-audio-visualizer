

#include <Arduino.h>
#include "fft_engine.h"
#include "led_display.h"

void setup() {
    Serial.begin(115200);
    delay(1000);
    initI2S(); 
    Serial.println("I2S Initialized Successfully");
    initLEDs();
    Serial.println("LEDs Initialized Successfully");
}

void loop() {
    // 1. קריאת 64 דגימות ממיקרופון ה-I2S
    for (int i = 0; i < SAMPLES; i++) {
        int32_t sample = 0;
        size_t bytes_read = 0;
        
        i2s_read(I2S_NUM_0, &sample, sizeof(sample), &bytes_read, portMAX_DELAY);
        fft_func(sample, i);
    }

    computeFFT(fft_input, SAMPLES);

    updateLEDs();

    delay(10); // השהייה קטנה ליציבות
}