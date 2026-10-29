
 // Declara las funciones de lectura de coordenadas y hora del GPS.
#ifndef SENSOR_GPS_H
#define SENSOR_GPS_H

#include <Arduino.h>

void iniciarGPS();
void procesarGPS();
bool obtenerCoordenadas(double &latitud, double &longitud);
bool obtenerFechaHoraColombia(char *destino, size_t capacidad);

#endif
