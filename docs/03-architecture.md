# Arquitectura

**Estado:** propuesta inicial; no implementada.

## Componentes previstos

1. Dispositivo ESP32-C3 con sensores INA219 para adquirir mediciones.
2. Firebase Realtime Database para almacenar y sincronizar datos del MVP.
3. Dashboard web con React, TypeScript y Vite para consultar y mostrar telemetría.
4. Fuente meteorológica por definir.

El [SRS](SRS.md) describe el alcance y las restricciones. El flujo y la topología eléctrica deberán validarse por separado; el diagrama conceptual del SRS no es un plano de conexión.

## Decisiones pendientes

- Identidad del dispositivo, autenticación de escritura y reglas de lectura.
- Estructura, consultas, retención y costo de los datos.
- Sincronización de hora, reconexión y tratamiento de mediciones sin conexión.
- Despliegue del dashboard y selección del servicio meteorológico.
- Estrategia de pruebas y medición de latencia de extremo a extremo.

No se ha decidido incorporar un backend propio ni una base SQL.
