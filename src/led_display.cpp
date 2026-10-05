#include "led_display.h"
#include "fft_engine.h"

CRGB leds[NUM_LEDS];

void initLEDs() {
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear(); // (מגדיר הכל כ-BLACK)
    FastLED.show();  
}

void updateLEDs(){
    for (int col=0; col<8; col++ ){
        float sum_magnitude = 0;
        for (int i = 0; i < 4; i++) {
            int bin = col * 4 + i;
            // vector size:(|real| + |imag|)
            sum_magnitude += fabs(fft_input[bin].real) + fabs(fft_input[bin].imag);
        }

        int height = map((int)sum_magnitude, 0, 2000, 0, 8);// Scale range 0..8
        height = constrain(height, 0, 8); // Keep within bounds

        for (int row = 0; row < height; row++) {
                int led_index = col * 8 + row; // location
                leds[led_index] = CHSV(col * 32, 255, 255); //color on matrix
            }

            FastLED.show();
    }

}