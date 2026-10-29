
 // Define la estructura de datos de una medición de AquaTrace.
#ifndef MEDICION_H
#define MEDICION_H

#include <Arduino.h>
#include <math.h>

struct Medicion {
    float ph = NAN;
    float temperaturaC = NAN;
    float turbidezNTU = NAN;
    float oxigenoPpm = NAN;

    double latitud = NAN;
    double longitud = NAN;

    bool temperaturaValida = false;
    bool turbidezValida = false;
    bool gpsValido = false;
    bool fechaHoraValida = false;

    char fechaHora[32] = {0};
};

#endif
