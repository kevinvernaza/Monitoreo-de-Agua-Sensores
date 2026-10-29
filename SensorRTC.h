
 // Declara las funciones para inicializar y consultar el RTC DS3231.
#ifndef SENSOR_RTC_H
#define SENSOR_RTC_H

#include <Arduino.h>

bool iniciarRTC();
bool obtenerFechaHoraRTC(char *destino, size_t capacidad);

#endif
