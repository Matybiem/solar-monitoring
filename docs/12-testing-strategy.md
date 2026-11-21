# Testing Strategy / Estrategia de Pruebas

**Estado:** v1.0 — Solar Monitoring

---

## Propósito del documento

Define **qué, cómo, cuándo y quién** testa. Alinea al equipo en cobertura, herramientas, criterios de calidad y gates de CI/CD. Basado en **Testing Trophy** (Kent C. Dodds) y **Test Pyramid** adaptado.

---

## 1. Principios de testing

| Principio | Descripción |
|-----------|-------------|
| **Test behavior, not implementation** | Probar *qué* hace, no *cómo* lo hace |
| **Fast feedback** | Unit < 30s, Integration < 2m, E2E < 5m — CI total < 10 min |
| **Deterministic** | Mismos inputs → mismos outputs. Cero flakiness tolerado |
| **Isolation** | Tests no comparten estado (Firebase Emulator, DB, cache, tiempo, random) |
| **Production-like** | Test environment ≈ production (Firebase Emulator, mismos contenedores) |
| **Shift Left** | Seguridad, performance, accesibilidad en CI |
| **Contract First** | API contracts (Zod schemas, Firebase Rules) validados antes que implementation tests |

---

## 2. Niveles de testing (Testing Trophy)

```
                    ┌─────────────┐
                    │   E2E       │  ← Pocos (5-15), críticos
                    │   (Playwright)│     Happy paths + regresiones
                    └──────┬──────┘
                           │
              ┌────────────┼────────────┐
              ▼            ▼            ▼
        ┌──────────┐ ┌──────────┐ ┌──────────┐
        │Integration│ │Integration│ │Integration│  ← Mayor ROI
        │  (API)   │ │  (DB)    │ │ (Messaging)│  Tests de contratos + flujos
        │ Firebase │ │ Emulator │ │ (Buffer)  │
        └──────────┘ └──────────┘ └──────────┘
              ▲            ▲            ▲
              └────────────┼────────────┘
                           │
              ┌────────────┼────────────┐
              ▼            ▼            ▼
        ┌──────────┐ ┌──────────┐ ┌──────────┐
        │  Unit    │ │  Unit    │ │  Unit    │  ← Muchos, rápidos, aislados
        │ (Logic)  │ │(Components)│ │(Utils)   │     Lógica pura, edge cases
        │  Vitest  │ │  RTL     │ │  Unity   │
        └──────────┘ └──────────┘ └──────────┘
```

### 2.1 Unit Tests (Base de la pirámide)

| Qué | Herramienta | Cobertura objetivo | Qué NO testear |
|-----|-------------|-------------------|----------------|
| Funciones puras, utils, validators, mappers, domain logic | `Vitest` (frontend) / `Unity` (firmware host) | `> 80%` líneas | Implementación interna, wrappers triviales |
| Componentes UI (lógica, no render) | `React Testing Library` + `Vitest` | `> 70%` | Snapshot testing (frágil), CSS, layout visual |
| Drivers INA219 (mock I2C) | `Unity` + `CMock` | `> 80%` | Hardware real (solo integration target) |
| Lógica SoC estimation | `Unity` host | `> 80%` | Sensores reales |

### 2.2 Integration Tests (Centro del valor)

| Qué | Herramienta | Cobertura objetivo | Entorno |
|-----|-------------|-------------------|---------|
| **Firebase Emulator** (reglas, triggers, functions) | `Firebase Emulator Suite` + `Vitest` | Todas las reglas + queries | Firebase Emulator local |
| **API Endpoints** (device → Firebase) | `Vitest` + `supertest` + Emulator | Todos los endpoints + errores | Firebase Emulator |
| **Messaging / Buffer** (circular buffer, retry) | `Unity` target + mock Wi-Fi | Flujos async críticos | ESP32 target / HIL |
| **External Integrations** (Weather API) | **Mock servers** (`MSW`, `WireMock`) | Contratos + error handling | No llamar a APIs reales en CI |

### 2.3 Contract Tests

| Qué | Herramienta | Qué valida |
|-----|-------------|------------|
| **Provider** (Firebase) cumple reglas de seguridad | `Firebase Emulator` + `Vitest` | Reglas `.read`/`.write`, validación payload |
| **Consumer** (Frontend) usa contrato | `Zod` schemas + TypeScript | Tipos sincronizados con payload |
| **Firmware ↔ API** | `Zod` en C (manual) + JSON schema | Payload shape, campos requeridos |

### 2.4 E2E Tests (Punta del iceberg)

| Qué | Herramienta | Cuántos | Qué testear |
|-----|-------------|---------|-------------|
| **Critical User Journeys** | `Playwright` (Chrome/Firefox/WebKit) | `5-15` max | Happy paths + 1-2 error flows críticos |
| **Visual Regression** | `Playwright` + `pixelmatch` | Componentes clave | Design System, páginas públicas |
| **Accessibility (a11y)** | `axe-core` en Playwright | Todas las páginas | WCAG 2.1 AA automático |
| **Responsive (Grid 12-col)** | `Playwright` (3 breakpoints) | Dashboard principal | Desktop 12, Tablet 8, Mobile 4 |

### 2.5 Non-Functional Tests (En CI/CD)

| Tipo | Herramienta | Cuándo | Gates |
|------|-------------|--------|-------|
| **Load / Stress** | `k6` | Nightly + Pre-release | p95 < 500ms, error rate < 0.1% @ 100 VU |
| **Security (SAST)** | `CodeQL` / `Semgrep` | Every PR | 0 Critical/High |
| **Security (DAST)** | `OWASP ZAP` | Nightly staging | 0 Critical/High |
| **Dependency Scan** | `Dependabot` / `Snyk` / `Trivy` | Every PR + Daily | 0 Critical vulns |
| **Performance (Frontend)** | `Lighthouse CI` | Every PR | Performance score > 90 |

---

## 3. Test Data Management

| Estrategia | Cuándo usar | Herramienta |
|------------|-------------|-------------|
| **Factories / Builders** | Unit/Integration — datos tipados, variados | `@faker-js/faker`, manual builders |
| **Fixtures / Snapshots** | E2E — estado conocido reproducible | JSON fixtures + Firebase Emulator seed |
| **Firebase Emulator Data** | Integration — aislamiento total | `firebase emulators:exec` con seed data |
| **Ephemeral Emulator per test** | Integration — aislamiento total | Emulator iniciado/detenido por test suite |

---

## 4. CI/CD Gates (Quality Gates)

| Stage | Checks requeridos | Bloquea merge si falla |
|-------|-------------------|------------------------|
| **Pre-commit** (local) | `lint`, `typecheck`, `unit:changed` | Sí (Husky) |
| **PR Pipeline** | `lint`, `typecheck`, `unit`, `integration`, `contract`, `sast`, `dep-scan` | Sí (GitHub Branch Protection) |
| **Nightly / Main** | `e2e`, `load`, `dast`, `full-scan`, `dependency-review` | Alerta (no bloquea main) |
| **Release Candidate** | Todo lo anterior + `smoke` en staging | Sí (manual approval) |

---

## 5. Test Organization (Estructura de carpetas)

```
apps/web/
├── src/
│   ├── features/
│   │   └── dashboard/
│   │       ├── components/
│   │       │   ├── MetricCard.test.tsx        ← Unit (RTL + Vitest)
│   │       │   └── DashboardGrid.test.tsx     ← Unit + Visual regression
│   │       ├── hooks/
│   │       │   └── useTelemetryRealtime.test.ts
│   │       └── api/
│   │           └── firebase.integration.test.ts  ← Integration (Emulator)
├── tests/
│   ├── e2e/
│   │   ├── dashboard.spec.ts                  ← E2E (Playwright)
│   │   ├── responsive.spec.ts                 ← E2E: Grid 12-col breakpoints
│   │   └── a11y.spec.ts                       ← E2E: axe-core
│   └── contract/
│       └── telemetry.schema.test.ts           ← Contract (Zod)
├── test-utils/
│   ├── firebase-emulator.ts
│   └── factories.ts
└── vitest.config.ts

apps/firmware/
├── components/
│   ├── acquisition/
│   │   └── test/                          # Unity unit tests (mock I2C)
│   ├── drivers/
│   │   └── ina219/
│   │       └── test/                      # Unity unit tests
│   └── power/
│       └── test/                          # Unity unit tests
└── test/                                  # Integration (target)
    ├── unity_test_runner.c
    └── test_full_cycle.c                  # measure → send → deep sleep
```

---

## 6. Métricas y Reportes

| Métrica | Target | Herramienta |
|---------|--------|-------------|
| **Code Coverage** (lines/branches/functions) | `> 80% / > 70% / > 80%` | `vitest --coverage` / `lcov` |
| **Mutation Testing Score** | `> 60%` | `Stryker` (JS/TS) |
| **Test Execution Time** | Unit < 30s, Integration < 2m, E2E < 5m | CI dashboard |
| **Flaky Test Rate** | `0%` (auto-quarantine si > 1 flake/semana) | Custom + GitHub Actions |
| **Defect Escape Rate** | `< 5%` bugs en prod detectados por usuarios | Issues + release tracking |

---

## 7. Responsabilidades (RACI)

| Actividad | Dev | QA/SDET | Tech Lead | EM |
|-----------|-----|---------|-----------|----|
| Escribir unit tests | R | C | A | I |
| Escribir integration tests | R | C | A | I |
| Escribir E2E tests | C | R | A | I |
| Mantener test infra (Emulator) | C | R | A | I |
| Definir coverage targets | C | C | R | A |
| Investigar flaky tests | R | C | A | I |
| Load testing | C | R | A | I |

---

## 8. Herramientas recomendadas por stack

| Stack | Unit | Integration | E2E | Contract | Load | SAST |
|-------|------|-------------|-----|----------|------|------|
| **Node/TS (React/Vite)** | Vitest | Vitest + Firebase Emulator | Playwright | Zod schemas | k6 | CodeQL / Semgrep |
| **C (ESP-IDF)** | Unity + CMock | Unity (target) + HIL | — | JSON schema manual | — | cppcheck / clang-tidy |
| **Python (scripts)** | pytest | pytest + Emulator | — | Zod-Python | locust | bandit |

---

## Historial de versiones

| Versión | Fecha | Autor | Cambios |
|---------|-------|-------|---------|
| 0.1 | 2024-11-20 | Matías Biem | Plantilla base |
| 1.0 | 2024-11-20 | Matías Biem | Testing Strategy Solar Monitoring v1.0: Testing Trophy, Firebase Emulator, Playwright, Unity, CI gates |