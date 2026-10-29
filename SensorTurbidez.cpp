
 // Implementa la lectura de turbidez por RS485 y Modbus RTU.
#include <Arduino.h>
#include "Configuracion.h"
#include "SensorTurbidez.h"

static HardwareSerial rs485Serial(1);

static uint16_t calcularCRC16(const uint8_t *datos, size_t longitud) {
    uint16_t crc = 0xFFFF;

    for (size_t i = 0; i < longitud; i++) {
        crc ^= datos[i];

        for (uint8_t bit = 0; bit < 8; bit++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }

    return crc;
}

void iniciarTurbidez() {
    pinMode(PIN_RS485_DE_RE, OUTPUT);
    digitalWrite(PIN_RS485_DE_RE, LOW);

    rs485Serial.begin(
        BAUD_MODBUS,
        SERIAL_8N1,
        PIN_RS485_RX,
        PIN_RS485_TX
    );
}

bool leerTurbidez(float &ntu) {
    uint8_t solicitud[8] = {
        MODBUS_ID, 0x03,
        static_cast<uint8_t>(MODBUS_REGISTRO >> 8),
        static_cast<uint8_t>(MODBUS_REGISTRO & 0xFF),
        0x00, 0x01, 0x00, 0x00
    };

    const uint16_t crcSolicitud = calcularCRC16(solicitud, 6);
    solicitud[6] = static_cast<uint8_t>(crcSolicitud & 0xFF);
    solicitud[7] = static_cast<uint8_t>(crcSolicitud >> 8);

    while (rs485Serial.available() > 0) {
        rs485Serial.read();
    }

    digitalWrite(PIN_RS485_DE_RE, HIGH);
    delayMicroseconds(200);

    rs485Serial.write(solicitud, sizeof(solicitud));
    rs485Serial.flush();

    delayMicroseconds(200);
    digitalWrite(PIN_RS485_DE_RE, LOW);

    uint8_t respuesta[7] = {};
    size_t recibidos = 0;
    const uint32_t inicio = millis();

    while (recibidos < sizeof(respuesta) &&
           millis() - inicio < TIMEOUT_MODBUS_MS) {
        while (rs485Serial.available() > 0 &&
               recibidos < sizeof(respuesta)) {
            respuesta[recibidos++] =
                static_cast<uint8_t>(rs485Serial.read());
        }

        delay(1);
    }

    if (recibidos != sizeof(respuesta)) {
        return false;
    }

    if (respuesta[0] != MODBUS_ID ||
        respuesta[1] != 0x03 ||
        respuesta[2] != 0x02) {
        return false;
    }

    const uint16_t crcRecibido =
        static_cast<uint16_t>(respuesta[5]) |
        (static_cast<uint16_t>(respuesta[6]) << 8);

    if (crcRecibido != calcularCRC16(respuesta, 5)) {
        return false;
    }

    const uint16_t registro =
        (static_cast<uint16_t>(respuesta[3]) << 8) |
        static_cast<uint16_t>(respuesta[4]);

    ntu = static_cast<float>(registro);
    return true;
}
