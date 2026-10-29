
 // Implementa la lectura analógica y el cálculo del pH.
#include <Arduino.h>
#include <math.h>
#include "Configuracion.h"
#include "SensorPH.h"

void iniciarPH() {
    pinMode(PIN_PH, INPUT);

    analogReadResolution(12);
    analogSetPinAttenuation(PIN_PH, ADC_11db);
}

static float leerVoltajePH() {
    float sumaMilivoltios = 0.0f;

    for (uint8_t i = 0; i < NUM_MUESTRAS_PH; i++) {
        sumaMilivoltios += analogReadMilliVolts(PIN_PH);
        delay(10);
    }

    const float voltajeADC =
        (sumaMilivoltios / NUM_MUESTRAS_PH) / 1000.0f;

    return voltajeADC * (R1_OHM + R2_OHM) / R2_OHM;
}

float leerPH() {
    const float voltaje = leerVoltajePH();

    if (!isfinite(voltaje)) {
        return NAN;
    }

    const float ph =
        7.0f + (VOLTAJE_PH7 - voltaje) / VOLTIOS_POR_PH;

    // Evita propagar resultados numéricamente inválidos.
    if (!isfinite(ph)) {
        return NAN;
    }

    return ph;
}
