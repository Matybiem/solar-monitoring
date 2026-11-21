# Architecture Decision Records (ADR) / Registro de Decisiones de Arquitectura

**Estado:** v1.0 — Solar Monitoring

---

## Propósito del documento

Registra **decisiones arquitectónicas significativas** con su contexto, alternativas y consecuencias. Permite trazabilidad y onboarding futuro. Formato basado en Michael Nygard / Thoughtworks.

---

## Formato estándar por ADR

### ADR-[XXX]: [Título corto de la decisión]

**Fecha:** YYYY-MM-DD
**Estado:** `[Proposed / Accepted / Superseded / Deprecated]`
**Decisores:** `[Nombres / roles]`
**Contexto técnico:** `[Link a issue, PR, discusión]`

---

#### Contexto y problema
> ¿Qué fuerza a tomar esta decisión? Describir restricciones, requisitos, dolores actuales.

#### Alternativas consideradas
| Opción | Pros | Contras | Veredicto |
|--------|------|---------|-----------|
| `[Opción A]` | `[Pros]` | `[Contras]` | `[Descartada / Elegida]` |
| `[Opción B]` | `[Pros]` | `[Contras]` | `[Elegida]` |

#### Decisión
> Declaración clara: **"Elegimos [Opción B] porque..."**

#### Consecuencias

**Positivas:**
- `[Beneficio 1]`
- `[Beneficio 2]`

**Negativas / Riesgos:**
- `[Coste / complejidad añadida]`
- `[Mitigación planeada]`

**Neutrales / Seguimiento:**
- `[Métrica a vigilar]`

#### Plan de implementación / Migración
- `[Paso 1]`
- `[Paso 2]`

#### Referencias
- `[Link a doc, RFC, benchmark, PoC]`

---

## Índice de ADRs del proyecto

| ID | Título | Estado | Fecha | Superseded by |
|----|--------|--------|-------|---------------|
| ADR-001 | Arquitectura Modular por Capas (Firmware + Frontend) | Accepted | 2024-11-20 | — |
| ADR-002 | Firebase Realtime Database como Backend MVP | Accepted | 2024-11-20 | — |
| ADR-003 | Sincronización Tiempo Real vía Firebase Listeners | Accepted | 2024-11-20 | — |
| ADR-004 | Deep Sleep Duty-Cycling (30s activo / 5min sleep) | Accepted | 2024-11-20 | — |
| ADR-005 | Fluent Design System (Microsoft 365 style) para UI | Accepted | 2024-11-20 | — |
| ADR-006 | Grid Responsive 12-col: Desktop 12, Tablet 8, Mobile 4 | Accepted | 2024-11-20 | — |
| ADR-007 | Estructura Datos Firebase: Plana por deviceId + timestamp | Accepted | 2024-11-20 | — |
| ADR-008 | Seguridad MVP: Auth Anónimo + Reglas Write-Restringidas | Accepted | 2024-11-20 | — |
| ADR-009 | Testing Trophy: Unit (Vitest/Unity) → Integration (Emulator) → E2E (Playwright) | Accepted | 2024-11-20 | — |
| ADR-010 | Monorepo Turborepo: apps/web, apps/firmware, packages/* | Accepted | 2024-11-20 | — |
| ADR-011 | SoC Estimation: Hybrid OCV + Coulomb Counting | Proposed | 2024-11-20 | — |
| ADR-012 | Weather API: OpenWeatherMap One Call 3.0 (cache 10min) | Accepted | 2024-11-20 | — |
| ADR-013 | OTA Futuro: Firebase Storage + HTTPS + ECDSA Signature | Proposed | 2024-11-20 | — |
| ADR-014 | Provisioning Device: BLE (v2) / Serial CLI (MVP) | Proposed | 2024-11-20 | — |

---

## ADRs Detallados

### ADR-001: Arquitectura Modular por Capas

**Fecha:** 2024-11-20
**Estado:** Accepted
**Decisores:** Matías Biem (Tech Lead)

#### Contexto y problema
El proyecto combina firmware embebido (ESP32-C3) y frontend web (React). Necesitamos una arquitectura que permita:
- Desarrollo y testing independiente de cada capa
- Reutilización de código (drivers, services, utils)
- Onboarding fácil para nuevos contribuidores
- Escalabilidad para añadir sensores/features sin rediseño

#### Alternativas consideradas
| Opción | Pros | Contras | Veredicto |
|--------|------|---------|-----------|
| **Monolito único (todo en main.c / App.tsx)** | Simple inicio | Acoplamiento alto, testing difícil, no escala | Descartada |
| **Microservicios (backend propio + firmware + frontend)** | Desacoplamiento total | Overhead operacional, coste, complejidad MVP | Descartada |
| **Modular por capas (Firmware: App/Services/Drivers/HAL/RTOS; Frontend: Feature-based)** | Separación responsabilidades, testing aislado, reutilizable, estándar industria | Requiere disciplina en boundaries | **Elegida** |

#### Decisión
**Elegimos arquitectura modular por capas** para ambos lados:
- **Firmware:** `app/` → `services/` → `drivers/` → `hal/` → `rtos/`. Cada capa solo conoce la inferior.
- **Frontend:** `features/{domain}/{components,hooks,api}` + `shared/{ui,hooks,utils,types}`. Features no se importan entre sí.
- **Monorepo:** Turborepo con `apps/web`, `apps/firmware`, `packages/*` compartidos.

#### Consecuencias
**Positivas:**
- Tests unitarios rápidos mockeando solo la capa inferior
- Nuevo sensor = nuevo driver + service, sin tocar app/core
- Feature flags por feature frontend (lazy loading)
- Paralelización trabajo: firmware/frontend independientes

**Negativas / Riesgos:**
- Overhead inicial configuración (CMake components, Turborepo, shared packages)
- Requiere code review estricto para evitar violaciones de capas
- Build system más complejo

**Mitigación:**
- ESLint rules: `no-restricted-imports` entre features
- CMake: `PRIVATE`/`PUBLIC` deps en components
- CI: `depcheck` / `madge` para detectar ciclos

---

### ADR-002: Firebase Realtime Database como Backend MVP

**Fecha:** 2024-11-20
**Estado:** Accepted
**Decisores:** Matías Biem

#### Contexto y problema
Necesitamos persistir telemetría (2880 pts/día) y servirla en tiempo real a dashboard. Requisitos: cero coste, tiempo real nativo, auth simple, hosting incluido.

#### Alternativas consideradas
| Opción | Pros | Contras | Veredicto |
|--------|------|---------|-----------|
| **Firebase Realtime DB (Spark Plan)** | Gratis, realtime nativo (WebSocket), auth anónimo, hosting gratis, reglas seguridad, SDK multi-plataforma | Límite 1GB/10GB, NoSQL, vendor lock-in, queries limitadas | **Elegida** |
| Firebase Firestore | Queries más potentes, escalabilidad | Coste en reads/writes, no realtime tan eficiente para alta frecuencia | Descartada |
| Supabase (PostgreSQL) | SQL, realtime, gratis generoso | Self-hosted complejo, menos maduro para IoT embebido | Descartada |
| Backend propio (FastAPI + PostgreSQL + TimescaleDB) | Control total, SQL, Timescale nativo | Infraestructura, coste, mantenimiento, auth, hosting | Descartada (v2) |
| MQTT + Broker propio (Mosquitto/VEMQ) | Estándar IoT, ligero | Requiere broker, persistencia, auth, frontend bridge | Descartada |

#### Decisión
**Elegimos Firebase Realtime Database (plan Spark gratuito)** para MVP. Migración a backend propio + TimescaleDB en v2 si escala.

#### Consecuencias
**Positivas:**
- Cero coste operativo MVP
- Realtime nativo sin código extra (WebSocket automático)
- Offline support en SDKs
- Hosting incluido para dashboard
- Reglas seguridad declarativas

**Negativas / Riesgos:**
- Límite 1GB storage / 10GB download/mes → retención 30d + agregados diarios
- Queries limitadas (orderByKey, limitToFirst/Last) → estructura datos optimizada
- Vendor lock-in → abstraer en `communication/firebase_client.c` y `features/*/api/`

**Mitigación:**
- Cloud Function diaria: agregados + limpieza > 30d
- `schema_version` en payload para migración futura
- Interfaces `TelemetryRepository` en frontend para swap backend

---

### ADR-003: Sincronización Tiempo Real vía Firebase Listeners

**Fecha:** 2024-11-20
**Estado:** Accepted

#### Decisión
**Frontend usa `onValue()` listeners de Firebase SDK** para tiempo real. Device escribe a `/telemetry/{deviceId}/latest` + `/telemetry/{deviceId}/{timestamp}`. Frontend suscribe a `latest` para live indicator y usa queries históricas para gráficas.

---

### ADR-004: Deep Sleep Duty-Cycling (30s activo / 5min sleep)

**Fecha:** 2024-11-20
**Estado:** Accepted

#### Decisión
**ESP32-C3 usa Deep Sleep (10-20µA) por 5 minutos entre ciclos de medición de 30 segundos.** Configurable vía NVS. Si batería < 3.2V → extiende sleep. Si < 3.0V → hibernación.

---

### ADR-005: Fluent Design System (Microsoft 365 style) para UI

**Fecha:** 2024-11-20
**Estado:** Accepted

#### Decisión
**Diseño visual basado en Fluent Design System (Microsoft 365):** luz, profundidad, movimiento, material, escala. Tokens definidos en `packages/design-tokens` → Style Dictionary → CSS vars + TS + JSON. Componentes en `packages/ui` con Storybook.

**No es copia pixel-perfect**, sino adopción de principios y lenguaje visual: acrylics/backgrounds, elevation/shadows, rounded corners, motion easing, iconografía similar, tipografía variable (Inter).

---

### ADR-006: Grid Responsive 12-col: Desktop 12, Tablet 8, Mobile 4

**Fecha:** 2024-11-20
**Estado:** Accepted

#### Decisión
**Sistema de grid 12 columnas basado en Fluent:**
- **Desktop (≥1024px): 12 columnas** — Layout completo, sidebar + contenido principal
- **Tablet (768-1023px): 8 columnas** — Sidebar colapsable, contenido adaptado
- **Mobile (<768px): 4 columnas** — Stack vertical, navegación bottom/drawer

**Breakpoints:** `bp.sm: 640px`, `bp.md: 768px`, `bp.lg: 1024px`, `bp.xl: 1280px`, `bp.2xl: 1536px`.
**Destacado en README y Project Context** como decisión clave de UX.

---

### ADR-007: Estructura Datos Firebase: Plana por deviceId + timestamp

**Fecha:** 2024-11-20
**Estado:** Accepted

#### Decisión
```
/telemetry/{deviceId}/{timestamp}  (timestamp = key = epoch ms)
/telemetry/{deviceId}/latest       (último registro para live)
/devices/{deviceId}                (metadatos dispositivo)
```
Optimizado para `orderByKey()` + `startAt()/endAt()` + `limitToFirst(1000)`.

---

### ADR-008: Seguridad MVP: Auth Anónimo + Reglas Write-Restringidas

**Fecha:** 2024-11-20
**Estado:** Accepted

#### Decisión
- **Device:** Anonymous Auth + Custom Token (`uid = deviceId`, claims `{deviceId}`)
- **Frontend:** Anonymous Auth (`signInAnonymously()`)
- **Rules:** Lectura pública en telemetría, escritura solo `auth.uid == deviceId`. Admin via `auth.token.admin`.

---

### ADR-009: Testing Trophy

**Fecha:** 2024-11-20
**Estado:** Accepted

#### Decisión
- **Unit:** Vitest (frontend) / Unity (firmware) — >80% lógica pura
- **Integration:** Firebase Emulator + Vitest (frontend) / Unity en target (firmware)
- **Contract:** Zod schemas + TypeScript types (compile-time)
- **E2E:** Playwright (5-15 critical journeys)
- **Non-functional:** k6 load, Lighthouse CI, CodeQL SAST

---

### ADR-010: Monorepo Turborepo

**Fecha:** 2024-11-20
**Estado:** Accepted

#### Decisión
```
apps/
  web/          # React + Vite + TypeScript
  firmware/     # ESP-IDF (CMake components)
packages/
  design-tokens/   # Fluent tokens → Style Dictionary
  ui/              # Component library (Storybook)
  eslint-config/
  tsconfig/
  firebase-types/  # Types generados
```

---

### ADR-011: SoC Estimation: Hybrid OCV + Coulomb Counting

**Fecha:** 2024-11-20
**Estado:** Proposed

#### Contexto
Batería 18650 2P (5200mAh). Necesitamos estimar SoC sin fuel gauge HW.

#### Alternativas
| Método | Precisión | Complejidad | Veredicto |
|--------|-----------|-------------|-----------|
| Solo OCV (voltaje reposo) | ±15% (solo en reposo) | Baja | Insuficiente |
| Solo Coulomb Counting | Deriva temporal, necesita calibración | Media | Insuficiente solo |
| **Híbrido OCV + Coulomb Counting** | ±10% con calibración | Media-Alta | **Propuesta** |
| Fuel Gauge HW (MAX17048) | ±1-2% | HW extra, coste | v2 |

#### Decisión (Pendiente validación)
Implementar híbrido: OCV para reset inicial + Coulomb Counting para tracking continuo. Requiere calibración inicial (ciclo carga/descarga completo).

---

### ADR-012: Weather API: OpenWeatherMap One Call 3.0

**Fecha:** 2024-11-20
**Estado:** Accepted

#### Decisión
OpenWeatherMap One Call API 3.0 (gratis 1000 calls/día). Cache 10min en Firebase `/weather_cache/{lat,lon}` + React Query. Fallback graceful.

---

### ADR-013: OTA Futuro: Firebase Storage + HTTPS + ECDSA

**Fecha:** 2024-11-20
**Estado:** Proposed

#### Decisión
v2: Firmware binario en Firebase Storage (privado, signed URL). Device descarga streaming → verifica firma ECDSA P-256 + SHA256 → escribe partición `ota_0` → bootloader mark pending → health check → confirm/rollback.

---

### ADR-014: Provisioning Device: BLE (v2) / Serial CLI (MVP)

**Fecha:** 2024-11-20
**Estado:** Proposed

#### Decisión
MVP: Credenciales Wi-Fi + Firebase config via Serial CLI (UART) en desarrollo. v2: Bluetooth LE provisioning app móvil (ESP32-C3 tiene BLE 5).

---

## Plantilla rápida para nuevo ADR

```markdown
### ADR-XXX: [Título]

**Fecha:** YYYY-MM-DD
**Estado:** Proposed
**Decisores:** [Tech Lead, Arquitecto]

#### Contexto
[Describir el problema]

#### Alternativas
| Opción | Pros | Contras |
|--------|------|---------|
| A | | |
| B | | |

#### Decisión
[Elegimos X porque...]

#### Consecuencias
- Positivas: ...
- Negativas: ...
```

---

## Guía: ¿Cuándo crear un ADR?

Crear ADR cuando la decisión:
- [x] Afecta a múltiples equipos/módulos (firmware + frontend)
- [x] Es costosa de revertir (one-way door)
- [x] Define tecnología base (lenguaje, framework, DB, cloud)
- [x] Establece patrón transversal (auth, logging, error handling, caching)
- [x] Tiene impacto en seguridad, rendimiento o coste significativo
- [x] Resuelve un debate técnico prolongado

**No crear ADR para:**
- Decisiones triviales o reversibles (two-way doors)
- Detalles de implementación de bajo nivel
- Preferencias de estilo de código (usar linter/formatter config)

---

## Historial de este documento

| Versión | Fecha | Autor | Cambios |
|---------|-------|-------|---------|
| 0.1 | 2024-11-20 | Matías Biem | Plantilla base |
| 1.0 | 2024-11-20 | Matías Biem | ADRs 001-014 Solar Monitoring v1.0 |