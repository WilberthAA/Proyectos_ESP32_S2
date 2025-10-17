#pragma once
// ====== ESP32-S2 LOLIN MINI — SOLO DEFINES DE PINES ======

// I2C (SSD1306)
#define PIN_I2C_SDA   8
#define PIN_I2C_SCL   9

// Encoder (2 canales)
#define PIN_ENC_A     4
#define PIN_ENC_B     5

// Botones
#define PIN_BTN1      2
#define PIN_BTN2      3

// Buzzer
#define PIN_BUZZER    14

// Relay
#define PIN_RELAY     16

// SPI (SD + MAX6675 comparten bus)
#define PIN_SPI_MISO  10
#define PIN_SPI_MOSI  11
#define PIN_SPI_SCK   12
#define PIN_SD_CS     7
#define PIN_MAX_CS    13

// LED RGBW (SK6812)
#define PIN_SK6812    6
