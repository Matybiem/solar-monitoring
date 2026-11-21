# Solar Monitoring

Proyecto de monitoreo de generación solar y baterías mediante un dispositivo IoT y un panel web.

## Estado del Proyecto

**v1.0 - MVP en desarrollo** — Arquitectura definida, documentación completa, estructura de código lista para implementar.

## Documentación

| Documento | Descripción | Ruta |
|-----------|-------------|------|
| SRS | Especificación de requisitos de software | `docs/SRS.md` |
| Project Charter | Objetivos, alcance, stakeholders | `docs/01-project-charter.md` |
| Architecture | Arquitectura C4, componentes, decisiones | `docs/03-architecture.md` |
| Data Model | Entidades Firebase, reglas, consultas | `docs/04-data-model.md` |
| API Specification | Interfaces, contratos, tipos | `docs/05-api-specification.md` |
| Firmware Design | Diseño embebido, power management, OTA | `docs/06-firmware-design.md` |
| Decisions Log | ADRs (Architecture Decision Records) | `docs/07-decisions-log.md` |
| Roadmap | Hoja de ruta de releases | `docs/08-roadmap.md` |
| Changelog | Historial de cambios | `docs/09-changelog.md` |
| Project Context | Contexto, stack, comandos, FAQ | `docs/10-project-context.md` |
| Glossary | Términos y definiciones | `docs/11-glossary.md` |
| Testing Strategy | Estrategia de pruebas | `docs/12-testing-strategy.md` |
| Security & Threat Model | Modelo de amenazas, controles | `docs/13-security-threat-model.md` |
| Deployment & Runbook | Despliegue, operaciones, incidentes | `docs/14-deployment-runbook.md` |
| UI/UX Design System | Diseño Fluent, componentes, tokens | `docs/15-ui-ux-design-system.md` |
| Contributing Guide | Guía de contribución | `docs/16-contributing-guide.md` |

## Estructura del Repositorio (Monorepo Turborepo)

```
solar-monitoring/
├── apps/
│   ├── web/                 # Frontend (React 18 + TypeScript + Vite)
│   │   ├── src/
│   │   │   ├── app/           # App shell, providers, routing
│   │   │   ├── features/      # Dominios: dashboard, historical, device-status, weather, settings
│   │   │   └── shared/        # UI (Fluent tokens), hooks, utils, types, lib
│   │   └── package.json
│   └── firmware/            # Embedded (ESP-IDF v5.2, C)
│       ├── main/
│       └── components/        # acquisition, communication, power, diagnostics, drivers, hal, config, time
├── packages/
│   ├── design-tokens/       # Fluent Design tokens (Style Dictionary)
│   ├── ui/                  # Component library (Storybook)
│   ├── eslint-config/
│   ├── tsconfig/
│   └── firebase-types/      # Tipos TypeScript compartidos
├── infra/
│   ├── firebase/            # Reglas Firebase, indexes
│   └── docker/              # Dockerfile, docker-compose
├── docs/                    # Documentación completa
├── .github/workflows/       # CI/CD pipelines
├── package.json             # Root (Turborepo)
├── turbo.json               # Turborepo config
└── README.md
```

## Stack Tecnológico

| Capa | Tecnología | Versión |
|------|------------|---------|
| Frontend | React 18 + TypeScript + Vite | 18.x / 5.x |
| Cloud | Firebase Realtime Database (Spark) | Gratis |
| Weather API | OpenWeatherMap One Call 3.0 | Gratis (1000 calls/día) |
| Firmware | ESP-IDF v5.2 + C (C11) | 5.2 |
| MCU | ESP32-C3 Super Mini | RISC-V, Wi-Fi 2.4GHz |
| Sensores | 2× INA219 (I2C) | Voltaje, corriente |
| Batería | 18650 2P | 5200 mAh nominal |
| Diseño | Fluent Design System | Custom tokens |
| Grid | 12 columnas | Desktop 12, Tablet 8, Mobile 4 |

## Comandos de Desarrollo

```bash
# 1. Instalar dependencias (requiere Node.js 20+ y pnpm 9+)
pnpm install

# 2. Levantar entorno de desarrollo
pnpm dev

# 3. Ejecutar pruebas
pnpm test              # Unit + Integration
pnpm test:e2e          # Playwright E2E
pnpm test:contract     # Contract tests

# 4. Calidad de código
pnpm lint              # ESLint
pnpm typecheck         # TypeScript
pnpm fmt               # Prettier

# 5. Build
pnpm build             # Build todas las apps

# 6. Firmware (requiere ESP-IDF)
cd apps/firmware
idf.py build           # Compilar firmware
idf.py -p /dev/ttyUSB0 flash  # Flashear dispositivo
idf.py -p /dev/ttyUSB0 monitor  # Monitor serie
```

## Decisiones Arquitectónicas Clave (ADR)

- **ADR-001:** Arquitectura modular por capas (firmware + frontend)
- **ADR-002:** Firebase Realtime Database como backend MVP
- **ADR-003:** Sincronización tiempo real vía Firebase listeners
- **ADR-004:** Deep Sleep duty-cycling (30s activo / 5min sleep)
- **ADR-005:** Fluent Design System (estilo Microsoft 365)
- **ADR-006:** Grid responsive 12-col: Desktop 12, Tablet 8, Mobile 4
- **ADR-007:** Estructura Firebase plana por deviceId + timestamp
- **ADR-008:** Seguridad MVP: Auth anónimo + reglas write-restringidas
- **ADR-009:** Testing Trophy: Vitest + Unity + Playwright
- **ADR-010:** Monorepo Turborepo

## Grid Responsive (12 columnas)

El sistema utiliza un grid de 12 columnas basado en Fluent Design:

- **Desktop (≥1024px):** 12 columnas — layout completo con sidebar
- **Tablet (768-1023px):** 8 columnas — sidebar colapsable
- **Mobile (<768px):** 4 columnas — stack vertical

12 es el mínimo común múltiplo de 2, 3 y 4, lo que permite layouts flexibles (mitad, tercio, cuarto).

## Contribución

Consulta [CONTRIBUTING.md](docs/16-contributing-guide.md) para el flujo de trabajo, estándares de código y proceso de PR.

## Licencia

MIT License
