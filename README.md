# led-matrix-audio-visualizer
# Audio-Reactive LED Matrix Spectrum Analyzer

A embedded real-time audio visualizer built on the ESP32 platform. The system samples ambient sound through an INMP441 I2S MEMS microphone, executes fast frequency analysis via FFT, and renders dynamic spectrum visual patterns onto a WS2812B addressable LED matrix.

---

## Technical Overview

This project demonstrates low-level embedded software engineering, real-time audio processing, and peripheral hardware interfacing.

### Core Highlights
- **Direct I2S DMA Audio Ingestion:** Configured the ESP32 I2S peripheral with Direct Memory Access (DMA) to capture high-fidelity, low-latency digital audio without blocking the main CPU loop.
- **On-Chip Signal Processing:** Applied Fast Fourier Transform (FFT) algorithms on incoming audio buffers to partition the frequency spectrum into distinct frequency bands (Bass, Mids, Treble).
- **Dynamic Gain & Noise Floor Calibration:** Implemented adaptive software scaling and dynamic range adjustment to keep visual output balanced across varying ambient sound levels.
- **Hardware Integration:** Interfaced a high-density WS2812B RGB LED matrix with precision timing and minimal jitter using optimized data drivers.

---

## Architecture & Hardware Stack

- **Microcontroller:** ESP32 (Dual-core 32-bit LX6)
- **Audio Interface:** INMP441 I2S Digital MEMS Microphone
- **Display Output:** WS2812B Addressable RGB LED Matrix
- **Development Environment:** C++ / PlatformIO / ESP-IDF drivers

---

## Hardware Pinout Matrix

| Peripheral | Signal | ESP32 Pin | Protocol / Description |
| :--- | :--- | :--- | :--- |
| **INMP441** | SCK (BCLK) | GPIO 22 | I2S Serial Clock |
| **INMP441** | WS (LRCLK) | GPIO 23 | I2S Word Select / Frame Clock |
| **INMP441** | SD (DOUT) | GPIO 35 | I2S Serial Data Out |
| **WS2812B** | DIN | GPIO 18 | High-Speed Single-Wire Protocol |

---

## Key Modules & Software Structure

- `AudioEngine` — Initializes I2S DMA buffers, manages audio sampling rates, and performs noise filtering.
- `FFTProcessor` — Handles windowing functions and Fast Fourier Transform computations over raw PCM buffers.
- `VisualizerRenderer` — Maps normalized frequency band magnitudes to spatial pixel vectors and color gradients via FastLED.
