# Acta del proyecto

**Estado:** borrador.

## Objetivo

Desarrollar un sistema de monitoreo solar que reúna telemetría de generación y batería, la almacene y la presente en un panel web accesible remotamente.

El proyecto también busca servir como portafolio para mostrar prácticas de desarrollo, pruebas y documentación. La funcionalidad comprometida se define en el [SRS](SRS.md).

## Alcance del MVP

Medición y transmisión de telemetría, visualización actual e histórica, estado del dispositivo e información meteorológica. El SRS enumera los criterios de éxito y las funcionalidades excluidas.

## Restricciones y riesgos

- El SRS especifica ESP32-C3, sensores INA219, Wi-Fi de 2,4 GHz y Firebase Realtime Database.
- La estimación de carga de batería, la protección de escritura y el manejo de desconexiones requieren definición.
- La operación autónoma y el plazo de visualización de 30 segundos son objetivos del SRS, aún no verificados.
