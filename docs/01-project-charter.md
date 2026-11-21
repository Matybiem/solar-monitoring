# Project Charter / Acta del Proyecto

**Estado:** borrador — reemplazar con valores del proyecto real.

---

## Propósito del documento

Este documento autoriza formalmente el proyecto y define su alcance de alto nivel, objetivos, stakeholders y restricciones. Sirve como referencia durante todo el ciclo de vida para decisiones de alcance y priorización.

---

## 1. Información general del proyecto

| Campo | Valor |
|-------|-------|
| **Nombre del proyecto** | Solar Monitoring |
| **Código / ID** | SOLAR-MON-2024 |
| **Fecha de inicio** | 2024-11-20 |
| **Fecha estimada de fin (MVP)** | 2025-03-31 |
| **Patrocinador / Sponsor** | Matías Biem (Desarrollador / Portfolio Owner) |
| **Project Manager / Tech Lead** | Matías Biem |

---

## 2. Justificación y objetivos de negocio

### 2.1 Problema que resuelve
Monitorear una instalación solar fotovoltaica pequeña (panel solar + batería 18650) para visualizar generación de energía, estado de carga de batería y correlacionar con condiciones meteorológicas, todo operando con energía solar como fuente principal.

### 2.2 Objetivos SMART
| # | Objetivo | Métrica de éxito | Fecha objetivo |
|---|----------|------------------|----------------|
| 1 | MVP funcional: telemetría solar + batería en dashboard | Datos recibidos y visualizados < 30s latencia | 2025-03-31 |
| 2 | Operación autónoma 24/7 con energía solar | Uptime > 95% en condiciones normales | 2025-03-31 |
| 3 | Portfolio demostrable | Demo accesible públicamente, código documentado | 2025-03-31 |
| 4 | Arquitectura modular documentada | ADRs registrados, componentes desacoplados | 2025-02-28 |

---

## 3. Alcance (In-Scope / Out-of-Scope)

### 3.1 In-Scope (MVP / Fase 1)
- Medición de voltaje, corriente y potencia solar (INA219 #1)
- Medición de voltaje, corriente (carga/descarga) y estimación SoC batería (INA219 #2)
- Transmisión telemetría vía Wi-Fi 2.4GHz (ESP32-C3)
- Persistencia en Firebase Realtime Database
- Dashboard web responsive (React + TypeScript + Vite)
- Visualización tiempo real (< 30s) e histórica
- Estado del dispositivo: RSSI, última actualización, salud batería
- Información meteorológica básica (proveedor por definir)
- Gráficas de tendencias de generación

### 3.2 Out-of-Scope (Explícitamente excluido del MVP)
- OTA firmware updates
- Push notifications / Email alerts / Telegram
- Multi-user management / Autenticación (lectura pública MVP)
- Predictive analytics / Machine learning
- Home Assistant integration
- Native mobile applications
- Backend propio / Base de datos SQL

> **Regla:** Todo lo no listado en In-Scope requiere cambio formal de alcance (change request / ADR).

---

## 4. Stakeholders y roles

| Rol | Nombre | Responsabilidad | Contacto |
|-----|--------|-----------------|----------|
| Product Owner / Sponsor | Matías Biem | Priorización, aceptación, funding | — |
| Tech Lead / Arquitecto | Matías Biem | Decisiones técnicas, arquitectura, ADRs | — |
| Desarrollador Fullstack | Matías Biem | Frontend, Firmware, Integración Firebase | — |
| QA / Tester | Matías Biem | Validación, pruebas manuales y automatizadas | — |
| DevOps / Infra | Matías Biem | CI/CD, Firebase, despliegue dashboard | — |

---

## 5. Restricciones y supuestos

### 5.1 Restricciones técnicas (del SRS)
- **MCU:** ESP32-C3 Super Mini (RISC-V, Wi-Fi + BLE)
- **Sensores:** 2× INA219 (I2C) — solar generation + battery monitoring
- **Conectividad:** Wi-Fi 2.4 GHz únicamente
- **Cloud MVP:** Firebase Realtime Database
- **Frontend:** React 18 + TypeScript + Vite
- **Energía:** Panel solar → Charge Controller → 18650 Battery Pack → ESP32

### 5.2 Restricciones de negocio
- **Presupuesto:** Proyecto personal / portfolio (costo mínimo)
- **Fecha dura:** MVP demostrable Q1 2025
- **Portafolio:** Código público, documentación completa, demo accesible

### 5.3 Supuestos
- Red Wi-Fi 2.4GHz estable disponible en ubicación de instalación
- Radiación solar suficiente para carga de baterías 18650
- Firebase Realtime Database disponible (plan Spark gratuito suficiente para MVP)
- APIs meteorológicas gratuitas disponibles (OpenWeatherMap, WeatherAPI, etc.)

---

## 6. Riesgos identificados (inicial)

| Riesgo | Probabilidad | Impacto | Mitigación | Dueño |
|--------|--------------|---------|------------|-------|
| Wi-Fi inestable en ubicación | Media | Alto | Deep sleep + reintento exponencial, buffer local en ESP32 | Tech Lead |
| Estimación SoC batería inexacta | Alta | Medio | Calibración empírica, rangos de confianza documentados, no presentar como medición directa | Desarrollador |
| Firebase costs / límites plan gratuito | Baja | Medio | Monitoreo uso, migración a PostgreSQL si escala | Tech Lead |
| Seguridad: lectura pública telemetría | Media | Bajo | Reglas Firebase: write restringido, read público MVP; auth en v2 | Tech Lead |
| Componentes electrónicos (protección batería) | Alta | Crítico | Revisión diseño eléctrico antes de energizar, protecciones HW (BMS, fusibles) | Desarrollador |

---

## 7. Criterios de éxito del proyecto (KPIs)

| KPI | Meta | Cómo se mide |
|-----|------|--------------|
| Latencia telemetría → dashboard (p95) | `< 30s` | Timestamp sensor vs render dashboard |
| Uptime sistema (condiciones normales) | `> 95%` | Heartbeat dispositivo + disponibilidad dashboard |
| Cobertura tests (unit + integration) | `> 70%` | CI pipeline (Vitest + Playwright) |
| Documentación completa | `100%` ADRs, SRS, Architecture, Data Model | Revisión manual / checklist |
| Demo portfolio accesible | Pública | URL funcionando 24/7 |

---

## 8. Aprobación

| Rol | Nombre | Firma | Fecha |
|-----|--------|-------|-------|
| Sponsor / PO | Matías Biem | | |
| Tech Lead | Matías Biem | | |

---

## Historial de cambios

| Versión | Fecha | Autor | Descripción |
|---------|-------|-------|-------------|
| 0.1 | 2024-11-20 | Matías Biem | Creación inicial desde plantilla con datos del proyecto Solar Monitoring |