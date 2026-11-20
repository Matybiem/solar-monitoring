# Modelo de datos

**Estado:** pendiente de diseño y validación.

## Datos previstos por el SRS

- Mediciones solares y de batería: tensión, corriente y potencia, cuando corresponda.
- Marca de tiempo de medición y estado de comunicación.
- Indicadores de dispositivo, señal Wi-Fi y estado de batería.
- Información meteorológica para el análisis.

## Por definir

Unidades, precisión, convenciones de signo, identificación del dispositivo, formato de tiempo, origen de cada valor, estructura en Firebase, índices, retención y manejo de datos faltantes o duplicados.

El estado de carga y la salud de la batería requieren métodos y límites de confianza definidos; no deben presentarse como mediciones directas sin validación. Véase [SRS.md](SRS.md).
