#ifndef LED_DISPLAY_H
#define LED_DISPLAY_H

#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN     18
#define NUM_LEDS    64
#define BRIGHTNESS  50

extern CRGB leds[NUM_LEDS];

void initLEDs();
void updateLEDs();

#endif