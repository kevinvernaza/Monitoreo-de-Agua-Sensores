# AquaTrace - ESP32 principal

Proyecto PlatformIO para la placa **DOIT ESP32 DEVKIT V1** (`esp32doit-devkit-v1`) con framework Arduino.

## Archivos
- `platformio.ini`: placa, framework, dependencias y baudrate del monitor.
- `src/main.cpp`: integra pH, DS18B20, GPS, turbidez por Modbus RTU y OD por ADC.
- `include/`, `lib/` y `test/`: carpetas normales de PlatformIO, sin archivos adicionales necesarios.

## Funcionamiento
- pH analógico en GPIO34, divisor R1=10k y R2=20k; calibración provisional pH7=2.50 V, 0.18 V/pH.
- DS18B20 en GPIO4, lectura sin bloqueo.
- GPS NEO-6M, UART2 RX25/TX26 a 9600.
- Oxígeno disuelto emulado: entrada ADC en GPIO35, leyendo el DAC del otro ESP32, relación nominal 0-3.3 V -> 0-20 ppm. No detecta desconexión, la lectura exige calibración.
- Turbidez emulada: maestro RS485/Modbus RTU, UART1 RX16/TX17; DE y /RE unidos a GPIO27. Esclavo ID1, función 03, registro 0x0000, 1 registro, NTU entero.
- Monitor serial 115200, lectura nominal cada 2 segundos (puede variar según latencia de Modbus).

## Requisitos antes de interconectar
1. Confirmar que la placa principal es ESP32 clásico DOIT DEVKIT V1.
2. Comprobar alimentación y niveles lógicos del transceptor RS485: un MAX485 de 5 V no debe conectarse directamente a GPIO del ESP32 sin adaptación. Preferir transceptor 3.3 V.
3. El emulador de turbidez original NO funcionará con este maestro sin añadir CRC16 real, control de DE//RE y responder al registro 0x0000. Mantener el emulador en su proyecto PlatformIO independiente.
4. No exceder 3.3 V en las entradas ADC34/35. Compartir GND entre circuitos.
5. No se incluyen transmisión LoRa ni conexión web en esta etapa.

## Uso
1. Abrir esta carpeta `AquaTrace` en VS Code con PlatformIO.
2. Ejecutar Build, seleccionar puerto del ESP32 principal y cargar Upload.
3. Abrir Monitor a 115200 baud.
4. Verificar cada sensor por separado antes de la integración física completa.
