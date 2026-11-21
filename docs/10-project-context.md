# Project Context / Contexto del Proyecto

**Estado:** v1.0 — Solar Monitoring

---

## Propósito del documento

Responde a: **"Acabo de llegar al proyecto, ¿qué necesito saber para ser productivo hoy?"**. Condensa decisiones clave, arquitectura, flujos, tooling y contactos en una sola página escaneable.

---

## 1. Elevator Pitch (30 segundos)

> **Solar Monitoring** es un proyecto personal de portafolio para monitorear una instalación solar y sus baterías. Adquiere telemetría de generación (panel solar) y almacenamiento (batería 18650) vía ESP32-C3 + 2× INA219, la transmite por Wi-Fi a Firebase Realtime Database, y la presenta en un dashboard web responsive (React + TypeScript + Vite) con diseño tipo Microsoft 365 (Fluent Design System) y grid 12-col: **Desktop 12, Tablet 8, Mobile 4**. Estado: planificación / MVP en desarrollo.

---

## 2. Fuente de verdad

| Documento | Contenido | Ruta |
|-----------|-----------|------|
| **SRS** | Requisitos, restricciones, criterios de éxito | `SRS.md` |
| **Arquitectura** | Componentes propuestos, decisiones, diagrama C4 | `03-architecture.md` |
| **Registro de decisiones (ADRs)** | Decisiones técnicas y contexto | `07-decisions-log.md` |
| **Hoja de ruta** | Orden de trabajo propuesto | `08-roadmap.md` |
| **Data Model** | Entidades Firebase, reglas, consultas | `04-data-model.md` |
| **API Spec** | Interfaces, contratos, tipos | `05-api-specification.md` |
| **Firmware Design** | Diseño embebido, capas, power management | `06-firmware-design.md` |

---

## 3. Stack Tecnológico (Tech Stack)

| Capa | Tecnología | Versión | Notas / Decisión clave (ADR) |
|------|------------|---------|------------------------------|
| **Frontend** | React 18 + TypeScript + Vite | 18.x / 5.x | SPA, SSR no requerido, Vite dev server |
| **API / Cloud** | Firebase Realtime Database | Spark Plan | Realtime nativo (WebSocket), reglas seguridad, hosting |
| **Weather API** | OpenWeatherMap One Call 3.0 | Gratis (1000 calls/día) | Cache 10min, fallback graceful |
| **Firmware** | ESP-IDF v5.2 + C (C11) | 5.2 | ESP32-C3 Super Mini, FreeRTOS, components modulares |
| **Database** | Firebase Realtime DB (NoSQL) | — | Estructura plana: `/telemetry/{deviceId}/{timestamp}` |
| **Cache** | React Query + Firebase cache | — | SWR para datos en tiempo real |
| **Auth** | Firebase Anonymous Auth | — | Device: custom token (`uid=deviceId`); Frontend: anónimo |
| **File Storage** | Firebase Hosting | Gratis | Despliegue dashboard, preview en PRs |
| **Infra** | GitHub Actions + Firebase Hosting | — | CI/CD, preview deployments por PR |
| **CI/CD** | GitHub Actions | — | Lint → Typecheck → Test → Build → Deploy |
| **Observability** | Firebase Console + Structured logs | — | Logs JSON en firmware, Firebase metrics |
| **Testing** | Vitest / Unity / Playwright / k6 | — | Unit / Integration / E2E / Load |
| **Design System** | Fluent Design System (custom) | — | Tokens: color, spacing, typography, motion, elevation |
| **Grid Responsive** | 12 columnas | — | Desktop 12, Tablet 8, Mobile 4 (breakpoints 1024/768) |

---

## 4. Arquitectura en una imagen

```
┌─────────────┐     HTTPS/WSS      ┌─────────────┐
│   Browser   │ ◄────────────────► │   Firebase  │
│  (React)    │   Realtime listener│  Realtime DB│
└──────┬──────┘                    └──────┬──────┘
       │                                 │
       ▼                                 ▼
┌─────────────────────────────────────────────────┐
│              Firebase Hosting + Rules           │
│  Auth: Anónimo (frontend) + Custom (device)    │
└────────────────┬────────────────────────────────┘
                 │ HTTPS REST
       ┌─────────┼─────────┐
       ▼         ▼         ▼
┌──────────┐ ┌────────┐ ┌──────────┐
│ Weather  │ │ Device │ │  Client  │
│  API     │ │  ESP32 │ │ Dashboard│
│(OpenW.)  │ │ C3 FW  │ │  (React) │
└──────────┘ └────┬────┘ └──────────┘
                  │ I2C
            ┌─────┴─────┐
            ▼           ▼
        INA219 #1   INA219 #2
     (Solar)      (Batería)
```

---

## 5. Estructura del repositorio (Monorepo Turborepo)

```
repo-root/
├── apps/
│   ├── web/          # Frontend (React + Vite + TypeScript)
│   │   ├── src/
│   │   │   ├── app/           # App shell, providers, routing, layout
│   │   │   ├── features/      # Dominios: dashboard, historical, device-status, weather, settings
│   │   │   └── shared/        # UI (Fluent tokens), hooks, utils, types
│   │   └── package.json
│   └── firmware/        # Embedded (ESP-IDF, C)
│       ├── main/
│       └── components/   # acquisition, communication, power, diagnostics, drivers, hal, config, time
├── packages/
│   ├── design-tokens/   # Fluent tokens → Style Dictionary (CSS vars, TS, JSON)
│   ├── ui/              # Component library (Storybook, tests, a11y)
│   ├── eslint-config/
│   ├── tsconfig/
│   └── firebase-types/  # Tipos TypeScript (Telemetry, Device, Weather)
├── infra/
│   ├── firebase/        # Reglas Firebase, indexes
│   └── docker/          # Dockerfiles, compose
├── docs/                # Documentación completa (esta carpeta)
├── .github/
│   └── workflows/       # CI/CD pipelines
└── turbo.json           # Turborepo config
```

---

## 6. Comandos esenciales (Developer Experience)

```bash
# Setup inicial (una vez)
make bootstrap        # Instala deps (pnpm), genera types, configura Firebase emulator

# Desarrollo diario
make dev              # Levanta todo: web (Vite) + Firebase Emulator
make dev:web          # Solo frontend (pnpm dev en apps/web)
make dev:firmware     # Solo firmware build (idf.py build)

# Calidad de código
make lint             # ESLint + Prettier check (apps/web + packages)
make typecheck        # TypeScript --noEmit (apps/web + packages)
make test             # Unit + Integration tests (Vitest + Unity host)
make test:e2e         # Playwright E2E (apps/web)
make test:contract    # Zod schema validation + Firebase Emulator

# Base de datos / Firebase
make firebase:emu     # Levanta Firebase Emulator local
make firebase:deploy  # Despliega reglas + hosting a Firebase

# Firmware
make firmware:build   # Compila para ESP32-C3 (idf.py build)
make firmware:flash   # Flashea via USB/UART (idf.py -p PORT flash)
make firmware:monitor # Serial monitor (idf.py -p PORT monitor)

# Release
make version:patch    # Bump patch + tag + changelog
make release          # Build + push images + deploy staging/prod
```

---

## 7. Entornos y URLs

| Entorno | URL Frontend | URL Firebase | Base de datos | Propósito |
|---------|--------------|--------------|---------------|-----------|
| **Local** | `http://localhost:5173` | `http://localhost:9000` | Firebase Emulator | Desarrollo |
| **Preview (PR)** | `https://pr-{n}.web.app` | Firebase staging | Efímera (branch) | Review visual + tests |
| **Staging** | `https://solar-mon-staging.web.app` | `solar-mon-staging` | Firebase (prod-like) | QA, integración |
| **Producción** | `https://solar-mon.web.app` | `solar-mon-prod` | Firebase (Spark plan) | Usuarios reales / Demo |

---

## 8. Flujos críticos (Happy Paths)

| Flujo | Entrada | Salida | Docs relacionados |
|-------|---------|--------|-------------------|
| **Boot → Medición → Envío** | ESP32 despierta (timer) | Telemetría en Firebase | [Firmware](06-firmware-design.md#3.2), [API](05-api-specification.md#41) |
| **Firebase → Dashboard (live)** | Listener `onValue` | UI actualizada < 30s | [Architecture](03-architecture.md#5.1), [API](05-api-specification.md#42) |
| **Dashboard histórico** | Usuario selecciona rango fechas | Gráfica con datos agregados | [Data Model](04-data-model.md#8), [API](05-api-specification.md#42) |
| **Weather widget** | Cada 10min (cache) | Datos clima para correlación | [API](05-api-specification.md#6) |
| **Deep Sleep → Wake** | RTC timer (5min) | Nuevo ciclo medición | [Firmware](06-firmware-design.md#5.2) |

---

## 9. Convenciones de código (Code Conventions)

| Área | Regla | Tooling |
|------|-------|---------|
| **Git** | Conventional Commits (`feat:`, `fix:`, `chore:`) | `commitlint` + Husky |
| **Branches** | `feature/`, `fix/`, `chore/`, `release/` | GitHub branch protection |
| **PRs** | Template obligatorio: descripción, testing, screenshots | GitHub PR template |
| **Types** | `strict: true`, no `any`, `unknown` preferido | TypeScript ESLint |
| **Imports** | Absolute paths (`@/components`), orden: externos → internos → relatives | ESLint import/order |
| **Components** | Funcionales, props tipadas, feature-based | — |
| **API** | Zod schemas → OpenAPI → Types generados | `zod-to-openapi` |
| **Database** | Reglas Firebase versionadas, nunca editar aplicadas | Firebase CLI emulator |
| **Secrets** | Nunca en repo. GitHub Secrets / Firebase config público | `pre-commit` secret scan |
| **Grid** | 12-col: Desktop 12, Tablet 8, Mobile 4 | Tailwind/CSS Grid + Fluent tokens |

---

## 10. Contactos y ownership

| Área | Owner Principal | Slack / Email |
|------|-----------------|---------------|
| **Todo (proyecto personal / portfolio)** | Matías Biem | — |
| **Frontend** | Matías Biem | — |
| **Backend / Firebase** | Matías Biem | — |
| **Firmware / IoT** | Matías Biem | — |
| **DevOps / Infra** | Matías Biem | — |
| **QA / Testing** | Matías Biem | — |
| **Product / PO** | Matías Biem | — |
| **Security** | Matías Biem | — |

---

## 11. Enlaces rápidos (Bookmarks)

| Qué | Enlace |
|-----|--------|
| **Repo** | `https://github.com/Matybiem/solar-monitoring` |
| **CI/CD** | `https://github.com/Matybiem/solar-monitoring/actions` |
| **Firebase Console** | `https://console.firebase.google.com/project/solar-mon-prod` |
| **Firebase Hosting** | `https://solar-mon.web.app` |
| **Staging** | `https://solar-mon-staging.web.app` |
| **GitHub Releases** | `https://github.com/Matybiem/solar-monitoring/releases` |
| **Issues / Projects** | `https://github.com/Matybiem/solar-monitoring/issues` |

---

## 12. Preguntas frecuentes (FAQ)

| Pregunta | Respuesta corta |
|----------|-----------------|
| ¿Cómo añado un nuevo endpoint Firebase? | 1. Zod schema en `packages/firebase-types` → 2. Regla Firebase → 3. Hook en `features/*/api` → 4. Test en Emulator |
| ¿Cómo hago deploy a staging? | Push a `main` → GitHub Actions → Firebase Hosting deploy automático |
| ¿Dónde están los secrets de staging? | GitHub Environments → `staging` → Secrets (Weather API key, etc.) |
| ¿Cómo debug en firmware? | `idf.py -p PORT monitor` (UART serial) + `idf.py gdb` (JTAG) |
| ¿Cómo añado una migración DB Firebase? | Editar `infra/firebase/firestore.rules` → probar en Emulator → deploy |
| ¿Cómo actualizo firmware OTA? | (v2) No disponible MVP. Reflash via UART/Serial CLI |
| ¿Qué grid usar para nuevo componente? | 12-col: Desktop 12, Tablet 8, Mobile 4 — ver `packages/design-tokens` |

---

## Historial de versiones

| Versión | Fecha | Autor | Cambios |
|---------|-------|-------|---------|
| 0.1 | 2024-11-20 | Matías Biem | Plantilla base |
| 1.0 | 2024-11-20 | Matías Biem | Contexto Solar Monitoring v1.0 con stack, estructura repo, comandos, flujos |