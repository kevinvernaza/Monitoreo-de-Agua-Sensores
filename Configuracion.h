
 // Centraliza los pines y parámetros de configuración de AquaTrace.
#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <Arduino.h>

constexpr uint8_t ID_BOYA = 1;

// Sensores analógicos y digitales.
constexpr uint8_t PIN_PH = 34;
constexpr uint8_t PIN_TEMP = 4;
constexpr uint8_t PIN_OD = 35;

// GPS NEO-6M: UART2.
constexpr uint8_t PIN_GPS_RX = 25;
constexpr uint8_t PIN_GPS_TX = 26;

// RTC DS3231: I2C.
constexpr uint8_t PIN_RTC_SDA = 21;
constexpr uint8_t PIN_RTC_SCL = 22;

// Turbidez: RS485/UART1.
constexpr uint8_t PIN_RS485_RX = 16;
constexpr uint8_t PIN_RS485_TX = 17;
constexpr uint8_t PIN_RS485_DE_RE = 27;

// Velocidades seriales.
constexpr uint32_t BAUD_CONSOLA = 115200;
constexpr uint32_t BAUD_GPS = 9600;
constexpr uint32_t BAUD_MODBUS = 9600;

// Intervalos.
constexpr uint32_t INTERVALO_LECTURA_MS = 2000;
constexpr uint32_t TIMEOUT_MODBUS_MS = 350;
constexpr uint32_t EDAD_MAXIMA_GPS_MS = 5000;

// Calibración provisional de pH.
constexpr uint8_t NUM_MUESTRAS_PH = 30;
constexpr float R1_OHM = 10000.0f;
constexpr float R2_OHM = 20000.0f;
constexpr float VOLTAJE_PH7 = 2.50f;
constexpr float VOLTIOS_POR_PH = 0.18f;

// Oxígeno disuelto estimado.
constexpr uint8_t NUM_MUESTRAS_OD = 20;
constexpr float OD_MAX_PPM = 20.0f;
constexpr float OD_VOLTAJE_MAX = 3.3f;

// Modbus de turbidez.
constexpr uint8_t MODBUS_ID = 1;
constexpr uint16_t MODBUS_REGISTRO = 0x0000;

#endif
