# AquaTrace – Firmware de la Boya Sensorial

Firmware desarrollado en **PlatformIO y Arduino Framework** para el sistema de monitoreo de calidad del agua AquaTrace. Este proyecto utiliza una ESP32 para adquirir mediciones de diferentes sensores ambientales, obtener información de ubicación mediante GPS, registrar fecha y hora mediante un módulo RTC y transmitir los datos hacia un nodo Edge.

El firmware está organizado mediante una arquitectura modular, separando la configuración, adquisición de datos y comunicación en archivos independientes.

## 1. Objetivo del proyecto

Desarrollar el firmware de una boya sensorial capaz de medir y transmitir variables relacionadas con la calidad del agua, permitiendo su posterior procesamiento, almacenamiento y visualización.

El sistema contempla las siguientes funcionalidades:

- Adquisición de variables fisicoquímicas del agua.
- Lectura de temperatura.
- Obtención de coordenadas geográficas mediante GPS.
- Registro de fecha y hora mediante RTC.
- Validación de mediciones y gestión de valores no disponibles (`null`).
- Transmisión inalámbrica hacia un nodo Edge.
- Arquitectura preparada para una futura implementación de comunicación LoRa.

## 2. Hardware utilizado

| Componente | Referencia | Función |
|---|---|---|
| Microcontrolador | ESP32 DOIT DevKit V1 | Unidad principal de procesamiento |
| Sensor de pH | PH-4502C | Medición de pH |
| Sensor de temperatura | DS18B20 | Medición de temperatura |
| Sensor de oxígeno disuelto | Módulo OD analógico | Estimación de oxígeno disuelto |
| Sensor de turbidez | Sensor Modbus RTU | Medición de turbidez |
| GPS | NEO-6M | Posicionamiento geográfico |
| RTC | DS1302 | Fecha y hora |
| Comunicación | WiFi | Transmisión experimental de datos |

## 3. Arquitectura del firmware

El proyecto utiliza una estructura modular para facilitar el mantenimiento, las pruebas individuales y la incorporación de nuevos dispositivos.

```text
AquaTrace/
│
├── include/
│   ├── Configuracion.h
│   ├── Medicion.h
│   ├── SensorGPS.h
│   ├── SensorOxigeno.h
│   ├── SensorPH.h
│   ├── SensorRTC.h
│   ├── SensorTemperatura.h
│   ├── SensorTurbidez.h
│   └── Transmision.h
│
├── src/
│   ├── main.cpp
│   ├── SensorGPS.cpp
│   ├── SensorOxigeno.cpp
│   ├── SensorPH.cpp
│   ├── SensorRTC.cpp
│   ├── SensorTemperatura.cpp
│   ├── SensorTurbidez.cpp
│   └── Transmision.cpp
│
├── test/
├── platformio.ini
└── README.md
```

### Descripción de los archivos

| Archivo | Responsabilidad |
|---|---|
| `main.cpp` | Coordina la inicialización, adquisición y flujo de trabajo. |
| `Configuracion.h` | Centraliza pines, parámetros y constantes. |
| `Medicion.h` | Define la estructura de datos de una medición. |
| `SensorPH.cpp/.h` | Inicialización, lectura y cálculo del pH. |
| `SensorOxigeno.cpp/.h` | Lectura analógica y estimación de OD. |
| `SensorTemperatura.cpp/.h` | Lectura del sensor DS18B20. |
| `SensorTurbidez.cpp/.h` | Comunicación Modbus RTU y lectura de turbidez. |
| `SensorGPS.cpp/.h` | Procesamiento GPS y obtención de coordenadas. |
| `SensorRTC.cpp/.h` | Lectura de fecha y hora del DS1302. |
| `Transmision.cpp/.h` | Gestión de la comunicación inalámbrica y envío de mediciones. |

Los archivos `.h` declaran las funciones y los archivos `.cpp` contienen sus implementaciones.

## 4. Asignación de pines

La siguiente tabla corresponde a la configuración de hardware definida para el proyecto.

| Dispositivo | Señal | GPIO ESP32 |
|---|---|---|
| PH-4502C | Salida analógica | GPIO 34 |
| DS18B20 | Datos | GPIO 4 |
| Oxígeno disuelto | Salida analógica | GPIO 35 |
| GPS NEO-6M | RX | GPIO 25 |
| GPS NEO-6M | TX | GPIO 26 |
| RTC DS1302 | CLK | GPIO 21 |
| RTC DS1302 | DAT | GPIO 22 |
| RTC DS1302 | RST | GPIO 23 |
| Turbidez | RX UART1 | GPIO 19 |
| Turbidez | TX UART1 | GPIO 18 |

**Nota:** La comunicación con el sensor de turbidez se realiza mediante UART directa, sin utilizar un pin adicional DE/RE en la configuración actual.

## 5. Configuración de sensores

### Sensor de pH

- Modelo: PH-4502C.
- Entrada analógica: GPIO 34.
- Resolución ADC: 12 bits.
- Promedio: 20 muestras.
- Calibración lineal implementada mediante pendiente e intercepto.

Ecuación utilizada:

\[
pH = PH\_SLOPE \cdot V + PH\_OFFSET
\]

Parámetros actuales:

- `PH_SLOPE = -5.70`
- `PH_OFFSET = 21.34`

### Sensor de oxígeno disuelto

- Entrada analógica: GPIO 35.
- Promedio de 20 muestras.
- Conversión de voltaje a concentración estimada.

La conversión actual utiliza una relación lineal entre el voltaje y el rango configurado de concentración.

### Sensor de temperatura

- Modelo: DS18B20.
- Comunicación mediante OneWire.
- Lectura no bloqueante.
- Validación de disponibilidad y rango de temperatura.

### Sensor de turbidez

- Comunicación UART1.
- Protocolo Modbus RTU.
- Velocidad: 9600 baudios.
- Identificador Modbus: 1.
- Función de lectura: 0x03.
- Verificación de CRC16.
- Escala configurada: 10 unidades por unidad de NTU.

### GPS

- Modelo: NEO-6M.
- Comunicación mediante UART2.
- Velocidad: 9600 baudios.
- Obtención de latitud y longitud.
- Validación de antigüedad de las coordenadas.

### RTC

- Modelo: DS1302.
- Comunicación serial de tres hilos.
- Pines CLK, DAT y RST.
- Conservación de fecha y hora mediante batería CR2032.
- Formato de fecha y hora: ISO 8601 con zona horaria UTC-5.

## 6. Estructura de los datos

Las mediciones se agrupan en una estructura denominada `Medicion`, que centraliza los valores adquiridos
