#include "fft_engine.h"


Complex fft_input[SAMPLES];

void initI2S() {
    const i2s_config_t i2s_config = {
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = i2s_bits_per_sample_t(32),
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
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
}

void bitReversal(Complex *v, int N) {
    int j = 0;
    for (int i = 0; i < N - 1; i++) {
        if (i < j) {
            Complex temp = v[i];
            v[i] = v[j];
            v[j] = temp;
        }
        int k = N >> 1;
        while (k <= j) {
            j -= k;
            k >>= 1;
        }
        j += k;
    }
}

void computeFFT(Complex *v, int N) {
    bitReversal(v, N);

    for (int len = 2; len <= N; len <<= 1) {
        float angle = -2.0f * M_PI / len;
        Complex wlen = { cosf(angle), sinf(angle) };

        for (int i = 0; i < N; i += len) {
            Complex w = { 1.0f, 0.0f };

            for (int k = 0; k < len / 2; k++) {
                Complex u = v[i + k];
                Complex v_target = v[i + k + len / 2];

                Complex t;
                t.real = w.real * v_target.real - w.imag * v_target.imag;
                t.imag = w.real * v_target.imag + w.imag * v_target.real;

                v[i + k].real = u.real + t.real;
                v[i + k].imag = u.imag + t.imag;

                v[i + k + len / 2].real = u.real - t.real;
                v[i + k + len / 2].imag = u.imag - t.imag;

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