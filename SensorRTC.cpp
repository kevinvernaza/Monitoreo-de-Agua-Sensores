
 // Implementa el reloj DS3231 y entrega la fecha/hora local de Colombia.
#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <stdio.h>

#include "Configuracion.h"
#include "SensorRTC.h"

static RTC_DS3231 rtc;
static bool rtcDisponible = false;

bool iniciarRTC() {
    Wire.begin(PIN_RTC_SDA, PIN_RTC_SCL);

    if (!rtc.begin()) {
        rtcDisponible = false;
        return false;
    }

    if (rtc.lostPower()) {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    rtcDisponible = true;
    return true;
}

bool obtenerFechaHoraRTC(char *destino, size_t capacidad) {
    if (!rtcDisponible || destino == nullptr || capacidad < 26) {
        return false;
    }

    const DateTime ahora = rtc.now();

    snprintf(
        destino,
        capacidad,
        "%04d-%02d-%02dT%02d:%02d:%02d-05:00",
        ahora.year(),
        ahora.month(),
        ahora.day(),
        ahora.hour(),
        ahora.minute(),
        ahora.second()
    );

    return true;
}
