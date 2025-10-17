#ifndef PINS_S2_MINI_H
#define PINS_S2_MINI_H

// ─────────────────────────────────────────────────────────────────────────────
//  ESP32-S2 Lolin Mini — Mapeo de pines y utilidades
//  Basado en: asignación que compartiste (I2C, SPI, SD, MAX6675, botones,
//  encoder, buzzer, relé y SK6812).
//  Nota: Este header no fuerza periféricos; solo define pines y helpers.
// ─────────────────────────────────────────────────────────────────────────────

#include <Arduino.h>

// ===== Sanity checks (opcional pero útil) ===================================
#if !defined(ARDUINO_ARCH_ESP32)
#  error "Este proyecto está pensado para ESP32 (familia). Selecciona ESP32-S2 en el IDE."
#endif

// Si compilas con core ESP32 moderno puedes detectar target:
#if defined(CONFIG_IDF_TARGET_ESP32S2)
// OK
#else
#  warning "Parece que no estás en ESP32-S2. Verifica la placa seleccionada."
#endif

// ===== Pines — TU ASIGNACIÓN =================================================
// I2C (SSD1306)
#define PIN_I2C_SDA          8
#define PIN_I2C_SCL          9

// Encoder (2 canales)
#define PIN_ENC_A            4
#define PIN_ENC_B            5

// Botones
#define PIN_BTN1             2
#define PIN_BTN2             3   // (corrección del texto: el segundo botón es BTN2)

// Buzzer (LEDC)
#define PIN_BUZZER          14

// Relé
#define PIN_RELAY           16
#ifndef RELAY_ACTIVE_HIGH
#  define RELAY_ACTIVE_HIGH 1    // Cambia a 0 si tu módulo de relé es activo en LOW
#endif

// SPI (SD + MAX6675 comparten bus)
#define PIN_SPI_MISO        10
#define PIN_SPI_MOSI        11
#define PIN_SPI_SCK         12
#define PIN_SD_CS            7
#define PIN_MAX6675_CS      13

// LED RGBW (SK6812)
#define PIN_SK6812           6

// ===== Parámetros por defecto (puedes ajustarlos) ============================
// I2C pull-ups externos recomendados: 4.7k a 3.3V en SDA/SCL
#ifndef I2C_FREQ_HZ
#  define I2C_FREQ_HZ     400000UL   // 400 kHz
#endif

// Encoder
#ifndef ENCODER_USE_INTERRUPTS
#  define ENCODER_USE_INTERRUPTS 1   // 1 = adjuntar a interrupts, 0 = polling
#endif

// Botones (debounce)
#ifndef BTN_DEBOUNCE_MS
#  define BTN_DEBOUNCE_MS 25
#endif

// Buzzer (LEDC)
#ifndef BUZZER_LEDC_FREQ
#  define BUZZER_LEDC_FREQ   2000    // 2 kHz
#endif
#ifndef BUZZER_LEDC_RES_BITS
#  define BUZZER_LEDC_RES_BITS 10    // 10 bits
#endif
#ifndef BUZZER_LEDC_CHANNEL
#  define BUZZER_LEDC_CHANNEL  0
#endif
#ifndef BUZZER_LEDC_TIMER
#  define BUZZER_LEDC_TIMER    0
#endif

// SPI
#ifndef SD_SPI_CLOCK_HZ
#  define SD_SPI_CLOCK_HZ   20000000UL  // 20 MHz típico para SD
#endif
#ifndef MAX6675_SPI_CLOCK_HZ
#  define MAX6675_SPI_CLOCK_HZ 400000UL // MAX6675 requiere reloj lento (~0.4 MHz)
#endif

// SK6812 — usa RMT (recomendado para S2). Canal 0 por defecto.
#ifndef NEOPIXEL_RMT_CHANNEL
#  define NEOPIXEL_RMT_CHANNEL 0
#endif

// ===== Helpers de inicialización (opcionales) ================================
static inline void pins_setup_buttons()
{
  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);
}

static inline void pins_setup_encoder()
{
  pinMode(PIN_ENC_A, INPUT_PULLUP);
  pinMode(PIN_ENC_B, INPUT_PULLUP);

#if ENCODER_USE_INTERRUPTS
  // Nota: Ajusta a CHANGE/RISING/FALLING según tu lib/decodificador
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_A), []{}, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_B), []{}, CHANGE);
#endif
}

static inline void pins_setup_buzzer()
{
  ledcSetup(BUZZER_LEDC_CHANNEL, BUZZER_LEDC_FREQ, BUZZER_LEDC_RES_BITS);
  ledcAttachPin(PIN_BUZZER, BUZZER_LEDC_CHANNEL);
  // Para sonar: ledcWrite(BUZZER_LEDC_CHANNEL, duty);
}

static inline void pins_setup_relay()
{
  pinMode(PIN_RELAY, OUTPUT);
#if RELAY_ACTIVE_HIGH
  digitalWrite(PIN_RELAY, LOW);   // reposo: apagado
#else
  digitalWrite(PIN_RELAY, HIGH);  // reposo: apagado (activo en LOW)
#endif
}

static inline void pins_setup_spi()
{
  // Configura el bus SPI por pines (Arduino core ESP32 permite esto)
  // Ojo: no llamamos SD.begin() ni transacciones aquí; solo preparamos pines.
  SPI.begin(PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI);
  pinMode(PIN_SD_CS, OUTPUT);
  digitalWrite(PIN_SD_CS, HIGH);       // deseleccionado
  pinMode(PIN_MAX6675_CS, OUTPUT);
  digitalWrite(PIN_MAX6675_CS, HIGH);  // deseleccionado
}

static inline void pins_setup_i2c()
{
  // Wire.begin(SDA, SCL, freq) se hace mejor en el .ino (por orden de init).
  // Aquí solo dejamos la intención:
  // Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL, I2C_FREQ_HZ);
}

static inline void pins_setup_sk6812()
{
  pinMode(PIN_SK6812, OUTPUT);
  digitalWrite(PIN_SK6812, LOW); // línea de datos en reposo
  // Inicializa tu librería (NeoPixelBus/FastLED/Adafruit_NeoPixel con backend RMT)
  // Ejemplo (Adafruit_NeoPixel con backend RMT para S2):
  // strip.begin(); strip.show();
}

// Inicialización integral (llámala en setup() si quieres algo rápido)
static inline void pins_setup_all()
{
  pins_setup_buttons();
  pins_setup_encoder();
  pins_setup_buzzer();
  pins_setup_relay();
  pins_setup_spi();
  // Llama a pins_setup_i2c() y a la init de tu lib I2C en tu sketch principal.
  pins_setup_sk6812();
}

// ===== Utilidades de uso común ===============================================
static inline void relay_on()
{
#if RELAY_ACTIVE_HIGH
  digitalWrite(PIN_RELAY, HIGH);
#else
  digitalWrite(PIN_RELAY, LOW);
#endif
}
static inline void relay_off()
{
#if RELAY_ACTIVE_HIGH
  digitalWrite(PIN_RELAY, LOW);
#else
  digitalWrite(PIN_RELAY, HIGH);
#endif
}

#endif // PINS_S2_MINI_H
