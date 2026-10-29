
 // Implementa la lectura ADC y la conversión estimada de oxígeno disuelto.
#include <Arduino.h>
#include <math.h>
#include "Configuracion.h"
#include "SensorOxigeno.h"

void iniciarOxigeno() {
    pinMode(PIN_OD, INPUT);
    analogSetPinAttenuation(PIN_OD, ADC_11db);
}

float leerOxigenoPpm() {
    uint32_t sumaMilivoltios = 0;

    for (uint8_t i = 0; i < NUM_MUESTRAS_OD; i++) {
        sumaMilivoltios += analogReadMilliVolts(PIN_OD);
        delay(3);
    }

    const float voltaje =
        (static_cast<float>(sumaMilivoltios) /
         NUM_MUESTRAS_OD) / 1000.0f;

    if (!isfinite(voltaje) ||
        voltaje < 0.0f ||
        voltaje > OD_VOLTAJE_MAX) {
        return NAN;
    }

    return (voltaje / OD_VOLTAJE_MAX) * OD_MAX_PPM;
}
