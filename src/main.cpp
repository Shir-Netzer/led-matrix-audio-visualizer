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

Complex fft_input[SAMPLES];

void setup() {
    Serial.begin(115200);
    delay(1000);

    const i2s_config_t i2s_config = {
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = i2s_bits_per_sample_t(32),
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S),
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

void bitReversal(Complex *v, int N) {
    // i for index, j for reversed index
    int j = 0;
    for (int i = 0; i < N - 1; i++) {
        if (i < j) {
            Complex temp = v[i];
            v[i] = v[j];
            v[j] = temp;
        }
        int k = N >> 1; // חלוקה ב-2 בשיפט ביטים
        while (k <= j) {
            j -= k;
            k >>= 1;
        }
        j += k;
    }
}

// פונקציה ראשית לחישוב FFT
void computeFFT(Complex *v, int N) {
    bitReversal(v, N);

    // len is the block size
    for (int len = 2; len <= N; len <<= 1) {
        
        // rotate angle
        float angle = -2.0f * M_PI / len;
        Complex wlen = { cosf(angle), sinf(angle) };

        for (int i = 0; i < N; i += len) {
            Complex w = { 1.0f, 0.0f }; // W_0 = 1 + 0i

            for (int k = 0; k < len / 2; k++) {
                Complex u = v[i + k];
                Complex v_target = v[i + k + len / 2];

                // t = w * v_target
                // (a+bi)(c+di) = (ac - bd) + (ad + bc)i
                Complex t;
                t.real = w.real * v_target.real - w.imag * v_target.imag;
                t.imag = w.real * v_target.imag + w.imag * v_target.real;

                v[i + k].real = u.real + t.real;
                v[i + k].imag = u.imag + t.imag;

                v[i + k + len / 2].real = u.real - t.real;
                v[i + k + len / 2].imag = u.imag - t.imag;

                // עדכון ה-Twiddle Factor k לשלב הבא: w = w * wlen
                float next_w_real = w.real * wlen.real - w.imag * wlen.imag;
                float next_w_imag = w.real * wlen.imag + w.imag * wlen.real;
                w.real = next_w_real;
                w.imag = next_w_imag;
            }
        }
    }
}

void fft_func(int32_t sample, int i) {
    fft_input[i].real = (float)(sample >> 14); 
    fft_input[i].imag = 0.0f;
}

void loop() {
    for (int i = 0; i < SAMPLES; i++) {
        int32_t sample = 0;
        size_t bytes_read = 0;
        
        i2s_read(I2S_NUM_0, &sample, sizeof(sample), &bytes_read, portMAX_DELAY);
        fft_func(sample, i);
    }


    computeFFT(fft_input, SAMPLES);

    for (int i = 0; i < SAMPLES / 2; i++) {
        float magnitude = sqrtf(fft_input[i].real * fft_input[i].real + 
                                fft_input[i].imag * fft_input[i].imag);
        
        Serial.print(magnitude);
        Serial.print(" ");
    }
    Serial.println(); 

    delay(20);
}