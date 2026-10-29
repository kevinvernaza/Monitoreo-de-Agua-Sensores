
 // Coordina los sensores y genera la salida serial de AquaTrace.
#include <Arduino.h>
#include <math.h>

#include "Configuracion.h"
#include "Medicion.h"

#include "SensorPH.h"
#include "SensorTemperatura.h"
#include "SensorOxigeno.h"
#include "SensorGPS.h"
#include "SensorTurbidez.h"
#include "SensorRTC.h"

static uint32_t ultimaLectura = 0;

static Medicion tomarMedicion() {
    Medicion m;

    // pH.
    m.ph = leerPH();

    // Temperatura.
    m.temperaturaValida = leerTemperatura(m.temperaturaC);

    // Oxígeno disuelto.
    m.oxigenoPpm = leerOxigenoPpm();

    // Turbidez por RS485.
    m.turbidezValida = leerTurbidez(m.turbidezNTU);

    // Coordenadas provenientes del GPS.
    m.gpsValido = obtenerCoordenadas(m.latitud, m.longitud);

    // Fecha y hora provenientes del RTC.
    m.fechaHoraValida = obtenerFechaHoraRTC(
        m.fechaHora,
        sizeof(m.fechaHora)
    );

    return m;
}

static void imprimirMedicion(const Medicion &m) {
    Serial.print("ID:");
    Serial.println(ID_BOYA);

    Serial.print("FechaHora:");
    if (m.fechaHoraValida) {
        Serial.println(m.fechaHora);
    } else {
        Serial.println("null");
    }

    Serial.print("Temp:");
    if (m.temperaturaValida) {
        Serial.println(m.temperaturaC, 2);
    } else {
        Serial.println("null");
    }

    Serial.print("Turbidez:");
    if (m.turbidezValida) {
        Serial.println(m.turbidezNTU, 2);
    } else {
        Serial.println("null");
    }

    Serial.print("Ph:");
    if (isfinite(m.ph)) {
        Serial.println(m.ph, 2);
    } else {
        Serial.println("null");
    }

    Serial.print("OxigenoDisuelto:");
    if (isfinite(m.oxigenoPpm)) {
        Serial.println(m.oxigenoPpm, 2);
    } else {
        Serial.println("null");
    }

    Serial.print("Latitud:");
    if (m.gpsValido) {
        Serial.println(m.latitud, 6);
    } else {
        Serial.println("null");
    }

    Serial.print("Longitud:");
    if (m.gpsValido) {
        Serial.println(m.longitud, 6);
    } else {
        Serial.println("null");
    }
}

void setup() {
    Serial.begin(BAUD_CONSOLA);

    iniciarPH();
    iniciarTemperatura();
    iniciarOxigeno();
    iniciarGPS();
    iniciarTurbidez();

    // Inicializar el reloj. Si no responde, FechaHora será null.
    iniciarRTC();

    ultimaLectura = millis();
}

void loop() {
    // Procesar continuamente los datos de coordenadas del GPS.
    procesarGPS();

    if (millis() - ultimaLectura >= INTERVALO_LECTURA_MS) {
        ultimaLectura = millis();

        const Medicion medicion = tomarMedicion();
        imprimirMedicion(medicion);
    }
}
