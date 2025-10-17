#pragma once
#include <Arduino.h>            // pinMode, digitalWrite, etc.
#include <SPI.h>                // SPI.begin(...)
#include "hal/ledc_types.h"     // Compatibilidad con core 3.x
#include "esp32-hal-ledc.h"     // LEDC (buzzer PWM)

// ==========================================================
// Pines principales ESP32-S2 Lolin Mini
// ==========================================================

// I2C (pantalla SSD1306)
#define PIN_I2C_SDA 8
#define PIN_I2C_SCL 9
#define I2C_FREQ_HZ 400000

// Encoder (2 canales)
#define PIN_ENC_A 4
#define PIN_ENC_B 5

// Botones adicionales
#define PIN_BTN1 2
#define PIN_BTN2 3

// Buzzer
#define PIN_BUZZER 14
#define BUZZER_LEDC_CHANNEL 0
#define BUZZER_LEDC_FREQ 2000
#define BUZZER_LEDC_RES_BITS 10

// Relay
#define PIN_RELAY 16

// SPI (SD + MAX6675 compartiendo bus)
#define PIN_SPI_MISO 10
#define PIN_SPI_MOSI 11
#define PIN_SPI_SCK 12
#define PIN_SD_CS 7
#define PIN_MAX_CS 13

// LED RGBW SK6812
#define PIN_SK6812 6
#define SK6812_RMT_CHANNEL 0

// ==========================================================
// Configuración de periféricos
// ==========================================================

inline void pins_setup_inputs() {
  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);
  pinMode(PIN_ENC_A, INPUT_PULLUP);
  pinMode(PIN_ENC_B, INPUT_PULLUP);
}

inline void pins_setup_buzzer() {
  ledcSetup(BUZZER_LEDC_CHANNEL, BUZZER_LEDC_FREQ, BUZZER_LEDC_RES_BITS);
  ledcAttachPin(PIN_BUZZER, BUZZER_LEDC_CHANNEL);
}

inline void pins_setup_relay() {
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, LOW);
}

inline void pins_setup_spi() {
  SPI.begin(PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI);
}

inline void pins_setup_all() {
  pins_setup_inputs();
  pins_setup_buzzer();
  pins_setup_relay();
  pins_setup_spi();
}
