
 // Implementa la recepción GPS y la fecha/hora de Colombia.
#include <Arduino.h>
#include <TinyGPSPlus.h>
#include <stdio.h>
#include "Configuracion.h"
#include "SensorGPS.h"

static TinyGPSPlus gps;
static HardwareSerial gpsSerial(2);

void iniciarGPS() {
    gpsSerial.begin(
        BAUD_GPS,
        SERIAL_8N1,
        PIN_GPS_RX,
        PIN_GPS_TX
    );
}

void procesarGPS() {
    while (gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read());
    }
}

bool obtenerCoordenadas(double &latitud, double &longitud) {
    procesarGPS();

    const bool valida =
        gps.location.isValid() &&
        gps.location.age() < EDAD_MAXIMA_GPS_MS;

    if (!valida) {
        latitud = NAN;
        longitud = NAN;
        return false;
    }

    latitud = gps.location.lat();
    longitud = gps.location.lng();
    return true;
}

static bool esBisiesto(int anio) {
    return (anio % 4 == 0 && anio % 100 != 0) ||
           (anio % 400 == 0);
}

static int diasDelMes(int anio, int mes) {
    const int dias[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    if (mes == 2 && esBisiesto(anio)) {
        return 29;
    }

    return dias[mes - 1];
}

bool obtenerFechaHoraColombia(char *destino, size_t capacidad) {
    procesarGPS();

    if (destino == nullptr || capacidad < 26) {
        return false;
    }

    const bool valida =
        gps.date.isValid() &&
        gps.time.isValid() &&
        gps.date.age() < EDAD_MAXIMA_GPS_MS &&
        gps.time.age() < EDAD_MAXIMA_GPS_MS;

    if (!valida) {
        destino[0] = '\0';
        return false;
    }

    int anio = gps.date.year();
    int mes = gps.date.month();
    int dia = gps.date.day();

    int hora = static_cast<int>(gps.time.hour()) - 5;
    const int minuto = gps.time.minute();
    const int segundo = gps.time.second();

    if (hora < 0) {
        hora += 24;
        dia--;

        if (dia < 1) {
            mes--;

            if (mes < 1) {
                mes = 12;
                anio--;
            }

            dia = diasDelMes(anio, mes);
        }
    }

    snprintf(
        destino,
        capacidad,
        "%04d-%02d-%02dT%02d:%02d:%02d-05:00",
        anio,
        mes,
        dia,
        hora,
        minuto,
        segundo
    );

    return true;
}
