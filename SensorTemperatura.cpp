
 // Implementa la lectura no bloqueante del DS18B20.
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <math.h>
#include "Configuracion.h"
#include "SensorTemperatura.h"

static OneWire busOneWire(PIN_TEMP);
static DallasTemperature sensorTemperatura(&busOneWire);

void iniciarTemperatura() {
    sensorTemperatura.begin();
    sensorTemperatura.setWaitForConversion(false);
    sensorTemperatura.requestTemperatures();
}

bool leerTemperatura(float &temperaturaC) {
    temperaturaC = sensorTemperatura.getTempCByIndex(0);

    const bool valida =
        temperaturaC != DEVICE_DISCONNECTED_C &&
        isfinite(temperaturaC) &&
        temperaturaC >= -55.0f &&
        temperaturaC <= 125.0f;

    // Solicita la siguiente conversión sin bloquear el programa.
    sensorTemperatura.requestTemperatures();

    if (!valida) {
        temperaturaC = NAN;
        return false;
    }

    return true;
}
