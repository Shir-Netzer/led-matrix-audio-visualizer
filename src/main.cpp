// #include <Arduino.h>

// // put function declarations here:
// int myFunction(int, int);

// void setup() {
//   // put your setup code here, to run once:
//   int result = myFunction(2, 3);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
// }

// // put function definitions here:
// int myFunction(int x, int y) {
//   return x + y;
// }

// import the libreries
#include <Arduino.h>
#include <FastLED.h>
#include <arduinoFFT.h>

// 1. הגדרות לדים
#define LED_PIN     18
#define NUM_LEDS    64
#define BRIGHTNESS  40
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

// 2. הגדרות שמע ו-FFT
#define MIC_PIN         34
#define SAMPLES         64
#define SAMPLING_FREQ   40000



void setup() { //compare to main in react
    Serial.begin(115200);
    Serial.println("System initialized successfully!");
}

void loop() {
    // לולאה ראשית תוגדר כאן
}