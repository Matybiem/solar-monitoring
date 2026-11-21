# Changelog / Registro de Cambios

**Estado:** v1.0 — Solar Monitoring

---

## Propósito del documento

Historial cronológico de cambios notables para **usuarios y desarrolladores**. Formato: **Keep a Changelog** + **SemVer**.

---

## Formato y convenciones

### Versiones (Semantic Versioning)
- **MAJOR** (`X.0.0`): Cambios incompatibles (breaking changes)
- **MINOR** (`0.X.0`): Funcionalidad nueva backward-compatible
- **PATCH** (`0.0.X`): Bug fixes backward-compatible
- **Pre-release:** `1.0.0-alpha.1`, `1.0.0-beta.2`, `1.0.0-rc.1`

### Categorías (en orden)
| Categoría | Emoji | Qué incluye |
|-----------|-------|-------------|
| `Added` | ✨ | Nueva funcionalidad |
| `Changed` | 🔄 | Cambios en funcionalidad existente |
| `Deprecated` | ⚠️ | Funcionalidad que se eliminará |
| `Removed` | 🗑️ | Funcionalidad eliminada |
| `Fixed` | 🐛 | Corrección de bugs |
| `Security` | 🔒 | Vulnerabilidades corregidas |
| `Performance` | ⚡ | Mejoras de rendimiento |
| `Infrastructure` | 🏗️ | CI/CD, deps, tooling, docs |

---

## Changelog del proyecto

## [Unreleased] - En desarrollo (main branch)

### Added
- Estructura inicial de documentación (SRS, Architecture, Data Model, API Spec, Firmware, ADRs, Roadmap, Changelog, Project Context, Glossary)
- Stack web aceptado: React, TypeScript, Vite
- Stack embebido: ESP-IDF v5.2, ESP32-C3 Super Mini
- Cloud MVP: Firebase Realtime Database (plan Spark)
- Diseño visual: Fluent Design System (Microsoft 365 style)
- Grid responsive: Desktop 12, Tablet 8, Mobile 4
- ADRs 001-014 registrados

### Changed
- 

### Fixed
- 

### Security
- 

---

## [0.1.0] - 2024-11-20 — **Documentación inicial**

### Added
- ✨ SRS completo: requisitos funcionales (RF-001 a RF-015) y no-funcionales (RNF-001 a RNF-019)
- ✨ Project Charter: objetivos, alcance, stakeholders, riesgos, KPIs
- ✨ Arquitectura v1.0: 3 capas (embebida, cloud, presentación), diagrama contexto C4
- ✨ Data Model v1.0: Device, Telemetry, WeatherCache, AggregatedDaily; Firebase Rules
- ✨ API Spec v1.0: Firebase Realtime DB patterns, Weather API, TypeScript types
- ✨ Firmware Design v1.0: modular por capas, FreeRTOS tasks, Deep Sleep duty-cycling, NVS encrypted, OTA v2 plan
- ✨ ADRs 001-014: arquitectura modular, Firebase backend, listeners tiempo real, deep sleep, Fluent Design, grid 12-col, estructura Firebase, seguridad MVP, testing trophy, monorepo Turborepo
- ✨ Roadmap v1.0: MVP Q1 2025, Next Q2-Q4 2025, Later visionario
- ✨ Project Context: stack, fuentes de verdad, notas de planificación
- ✨ Glossary: 13 términos clave (ESP32-C3, INA219, IoT, Telemetría, SoC, RSSI, MVP, SRS, ADR)

### Changed
- 

### Fixed
- 

### Security
- 🔒 Firebase Rules MVP: read público, write restringido por deviceId
- 🔒 NVS encrypted para credenciales dispositivo
- 🔒 Secure Boot + Flash Encryption planificados para producción

### Performance
- ⚡ Duty-cycling: 30s activo / 5min deep sleep (ESP32-C3 10-20µA)

### Infrastructure
- 🏗️ Monorepo Turborepo: apps/web, apps/firmware, packages/*
- 🏗️ CI/CD: GitHub Actions (lint, typecheck, test, build, deploy preview)
- 🏗️ Firebase Hosting para dashboard

---

## [0.0.1] - 2024-11-20 — **Scaffold inicial**

### Added
- ✨ Repositorio inicial con estructura de carpetas
- ✨ Documentación base organizando SRS existente
- ✨ Stack web aceptado registrado (React, TypeScript, Vite)

---

## Guía de mantenimiento

### Cuándo actualizar
- **En cada PR:** El autor añade entradas en `[Unreleased]` bajo la categoría correcta
- **En release:** El maintainer mueve `[Unreleased]` → `[X.Y.Z] - YYYY-MM-DD`, limpia vacío, tagea en Git

### Automatización recomendada
- **GitHub Action:** `changelog-generator` desde PR labels (`feat`, `fix`, `docs`, `perf`, `security`)
- **Release Drafter:** Genera borrador de release notes automáticamente
- **Commitlint:** Enfuerza conventional commits (`feat:`, `fix:`, `chore:`, etc.)

---

## Historial de versiones de este documento

| Versión | Fecha | Autor | Cambios |
|---------|-------|-------|---------|
| 0.1 | 2024-11-20 | Matías Biem | Plantilla base |
| 1.0 | 2024-11-20 | Matías Biem | Changelog Solar Monitoring v1.0 con documentación inicial |