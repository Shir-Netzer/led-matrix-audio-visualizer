// // #include <Arduino.h>

// // // put function declarations here:
// // int myFunction(int, int);

// // void setup() {
// //   // put your setup code here, to run once:
// //   int result = myFunction(2, 3);
// // }

// // void loop() {
// //   // put your main code here, to run repeatedly:
// // }

// // // put function definitions here:
// // int myFunction(int x, int y) {
// //   return x + y;
// // }

// import the libreries
// #include <Arduino.h>
// #include <FastLED.h>
// #include <arduinoFFT.h>

// // 1. הגדרות לדים
// #define LED_PIN     18
// #define NUM_LEDS    64
// #define BRIGHTNESS  40
// #define LED_TYPE    WS2812B
// #define COLOR_ORDER GRB

// // 2. הגדרות שמע ו-FFT
// #include <driver/i2s.h>
// #define I2S_WS 15
// #define I2S_SD 32
// #define I2S_SCK 14
// #define SAMPLES         64
// #define SAMPLING_FREQ   40000

// // הגדרת מערך הלדים ל־FastLED
// CRGB leds[NUM_LEDS];


// // פונקציה להגדרת המיקרופון
// void i2s_install() {
//     const i2s_config_t i2s_config = {
//         .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
//         .sample_rate = SAMPLING_FREQ,
//         .bits_per_sample = i2s_bits_per_sample_t(32),
//         .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
//         .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
//         .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
//         .dma_buf_count = 8,
//         .dma_buf_len = SAMPLES,
//         .use_apll = false
//     };

//     const i2s_pin_config_t pin_config = {
//         .bck_io_num = I2S_SCK,
//         .ws_io_num = I2S_WS,
//         .data_out_num = I2S_PIN_NO_CHANGE,
//         .data_in_num = I2S_SD
//     };

//     i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
//     i2s_set_pin(I2S_NUM_0, &pin_config);
// }

// void setup() {
//     Serial.begin(115200);
//     Serial.println("System initialized successfully!");

//     FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
//     FastLED.setBrightness(BRIGHTNESS);

//     i2s_install(); // הפעלת המיקרופון
// }

// void loop() {
//     int32_t raw_sample = 0;
//     size_t bytes_read = 0;
    
//     // קריאת דגימה אחת מהמיקרופון
//     i2s_read(I2S_NUM_0, &raw_sample, 4, &bytes_read, portMAX_DELAY);
    
//     // המרת הדגימה ל-16 ביט כדי שהמספרים יהיו קריאים במסך
//     int16_t sample = raw_sample >> 14; 

//     // הדפסה ל-Serial Monitor כדי לראות את עוצמת הרעש
//     Serial.println(sample); 

//     // נדליק לד אחד בירוק רק כדי לדעת שהלולאה רצה
//     leds[0] = CRGB::Green;
//     FastLED.show();
// }

// #include <Arduino.h>
// #include <FastLED.h>

// #define LED_PIN     18
// #define NUM_LEDS    64
// #define BRIGHTNESS  50

// CRGB leds[NUM_LEDS];

// void setup() {
//     FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
//     FastLED.setBrightness(BRIGHTNESS);
// }

// void loop() {
//     FastLED.clear();
//     leds[0] = CRGB::Red; // הדלקת הלד הראשון באדום
//     FastLED.show();
//     delay(1000);
// }

#include <Arduino.h>
#include <driver/i2s.h>

#define I2S_WS      15
#define I2S_SD      5
#define I2S_SCK     4
#define SAMPLE_RATE 16000

void setup() {
    Serial.begin(115200);
    delay(1000);

    const i2s_config_t i2s_config = {
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = i2s_bits_per_sample_t(32),
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S), // שינוי קל בתקן התקשורת
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 4,
        .dma_buf_len = 512,
        .use_apll = false
    };

    const i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_SCK,
        .ws_io_num = I2S_WS,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = I2S_SD
    };

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
    Serial.println("New Mic Test Started...");
}

void loop() {
    int32_t sample = 0;
    size_t bytes_read = 0;
    i2s_read(I2S_NUM_0, &sample, sizeof(sample), &bytes_read, portMAX_DELAY);
    
    Serial.println(sample);
    delay(50);
}