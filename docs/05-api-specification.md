# Especificación de interfaces

**Estado:** pendiente. No hay endpoints ni contratos implementados.

## Interfaces previstas

- Dispositivo a plataforma: envío de telemetría y estado.
- Dashboard a plataforma: lectura de datos actuales e históricos.
- Servicio meteorológico: consulta de información relevante para el análisis.

Para cada interfaz se deberán especificar formato, unidades, autenticación, permisos, validación, errores y límites. Las reglas de Firebase deben restringir la escritura incluso si se permite la lectura pública.

No se ha decidido si habrá una API propia. Véanse [arquitectura](03-architecture.md) y [modelo de datos](04-data-model.md).
