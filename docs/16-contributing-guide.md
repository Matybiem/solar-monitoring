# Contributing Guide / Guía de Contribución

**Estado:** v1.0 — Solar Monitoring

---

## Propósito del documento

Estándares y procesos para que **cualquiera** (empleado, contractor, open source contributor) pueda contribuir con calidad y confianza.

---

## 1. Código de Conducta

- Seguimos **[Contributor Covenant v2.1](https://www.contributor-covenant.org/version/2/1/code_of_conduct/)**
- Reportes: confidencial
- Aplicable en: GitHub (issues, PRs, discussions), Slack, eventos

---

## 2. Primeros Pasos (Setup en 10 min)

```bash
# 1. Clonar
git clone https://github.com/Matybiem/solar-monitoring.git
cd solar-monitoring

# 2. Bootstrap (instala deps, genera types, levanta Firebase Emulator)
make bootstrap

# 3. Verificar que todo funciona
make test          # Unit + Integration
make lint          # ESLint + Prettier
make typecheck     # TypeScript

# 4. Levantar entorno de desarrollo
make dev           # Web + Firebase Emulator

# 5. Abrir http://localhost:5173
```

**Requisitos previos:**
- Node.js 20+ (`.nvmrc`)
- pnpm 9+ (`corepack enable`)
- Firebase CLI (para emulator y deploy)
- Git 2.40+

---

## 3. Flujo de Trabajo (Git Workflow)

### 3.1 Branching (Trunk-based Development)
```
main ────────────────────────────────────────────────►
  │
  ├── feature/short-description  (short-lived, < 2 días)
  ├── fix/short-description
  ├── chore/short-description
  └── release/v1.2.x             (solo para hotfixes en prod)
```

**Reglas:**
- **Nunca** commit directo a `main`
- **Siempre** PR desde feature branch
- **Rebase** sobre `main` antes de PR (`git fetch origin && git rebase origin/main`)
- **Squash merge** en PR (historial limpio)

### 3.2 Commits (Conventional Commits 1.0)
```bash
# Formato
<type>(<scope>): <description>

# Tipos
feat:     Nueva funcionalidad (MINOR)
fix:      Bug fix (PATCH)
docs:     Documentación
style:    Formatting (prettier)
refactor: Refactor sin cambio de comportamiento
perf:     Mejora de rendimiento
test:     Tests
chore:    Mantenimiento (deps, config, build)
ci:       CI/CD changes
build:    Build system / deps

# Ejemplos
feat(firmware): add INA219 battery sensor driver
fix(api): handle null pointer in telemetry ingestion
docs(architecture): update C4 diagram for new worker
perf(db): add composite index for telemetry queries
```

**Herramienta:** `commitlint` + `husky` (pre-commit) — rechaza commits no conformes.

### 3.3 Pull Requests

#### Checklist obligatorio (PR Template)
- [ ] **Descripción:** Qué cambia y por qué (link a issue/ADR)
- [ ] **Testing:** Unit + Integration tests pasan; E2E si aplica
- [ ] **Docs:** README / ADR / Changelog / API spec actualizados
- [ ] **Breaking changes:** Marcados en descripción + migration guide
- [ ] **Screenshots/GIFs:** Para cambios UI (Storybook link si componente)
- [ ] **Performance:** Impacto medido (bundle size, query time, latency)
- [ ] **Security:** Sin secrets, validación inputs, authz verificado
- [ ] **A11y:** Cumple WCAG 2.1 AA (axe clean)

#### Labels automáticos (GitHub Actions)
| Label | Trigger |
|-------|---------|
| `feat` | Commit `feat:` |
| `fix` | Commit `fix:` |
| `breaking` | Commit `BREAKING CHANGE:` o `!:` |
| `docs` | Commit `docs:` |
| `performance` | Commit `perf:` |
| `security` | Commit `security:` o archivos `*security*` |
| `needs-review` | PR opened (default) |
| `approved` | 1 approval + CI green |
| `ready-to-merge` | Approved + CI green + up-to-date |

#### Reglas de Merge
- ✅ 1 approval + CI verde
- ✅ Branch up-to-date con `main` (rebase si necesario)
- ✅ No conversaciones sin resolver
- 🔀 **Squash and merge** (commit message = PR title + #PR)

---

## 4. Estándares de Código

### 4.1 TypeScript (Strict Mode)
```json
{
  "compilerOptions": {
    "strict": true,
    "noUncheckedIndexedAccess": true,
    "exactOptionalPropertyTypes": true,
    "noImplicitReturns": true,
    "noFallthroughCasesInSwitch": true,
    "noUnusedLocals": true,
    "noUnusedParameters": true
  }
}
```
- **Prohibido:** `any`, `Record<string, any>`, `@ts-ignore` (salvo excepciones documentadas)
- **Preferido:** `type` sobre `interface`, `const` assertions, `satisfies`

### 4.2 React / Frontend
- **Componentes:** Funcionales, props tipadas con `type`
- **Estado:** `useState`/`useReducer` local; `React Query` (server state)
- **Forms:** `React Hook Form` + `Zod` resolver
- **Estilos:** Tailwind CSS (utility-first) + `clsx`/`tailwind-merge`
- **Accesibilidad:** `eslint-plugin-jsx-a11y` + `axe-core` en tests
- **Grid:** 12-col: Desktop 12, Tablet 8, Mobile 4 — ver `packages/design-tokens`

### 4.3 Backend / API (Firebase)
- **Validación:** Zod schemas → Types generados (`zod-to-openapi`)
- **Errores:** RFC 9457 Problem Details
- **Logging:** Structured JSON, correlation ID

### 4.4 Firmware (ESP-IDF / C)
- **Lenguaje:** C (C11) + C++17 opcional
- **Build:** CMake + Ninja (`idf.py build/flash/monitor`)
- **Naming:** `snake_case` para archivos y funciones, `PascalCase` para structs
- **Seguridad:** No hardcodear credenciales; usar NVS encrypted

### 4.5 Base de Datos (Firebase)
- **Reglas:** Versionadas en `infra/firebase/firestore.rules`
- **Naming:** `snake_case` para keys, `camelCase` en código TS

### 4.6 Testing (Ver [Testing Strategy](12-testing-strategy.md))
- **Unit:** Lógica pura, utils, mappers, validators (`> 80%` coverage)
- **Integration:** Firebase Emulator, API endpoints, DB queries
- **Contract:** Zod schemas + TypeScript types (compile-time)
- **E2E:** Critical journeys only (`Playwright`, `5-15` tests max)

---

## 6. Herramientas de Calidad (Pre-commit / CI)

```bash
# Pre-commit (husky + lint-staged)
# Ejecuta solo en archivos staged:
- prettier --write
- eslint --fix
- tsc --noEmit (typecheck rápido)
- vitest run --changed (unit tests afectados)

# CI Pipeline (GitHub Actions)
# En cada PR:
1. lint + prettier check
2. typecheck (tsc --noEmit)
3. unit tests (vitest --coverage)
4. integration tests (Firebase Emulator)
5. contract tests (Zod schemas)
6. build (TypeScript, firmware)
7. SAST (CodeQL / Semgrep)
8. SCA (Dependabot / Trivy / Snyk)
```

---

## 7. Seguridad en Contribuciones

- **Nunca** commitear secrets (API keys, passwords, tokens, certificados)
- **Usar** `.env.example` para documentar variables requeridas
- **Escanear** localmente: `trufflehog filesystem .` / `git-secrets --scan`
- **Dependencias:** `pnpm audit` / `npm audit` antes de PR
- **Reportar vulnerabilidades:** por canal privado (no issue público)

---

## 8. Documentación

| Qué | Dónde | Cuándo |
|-----|-------|--------|
| **API Changes** | `openapi.yaml` + `05-api-specification.md` | En mismo PR |
| **Architecture Decisions** | `docs/ADR-XXX.md` | Antes de implementar |
| **Database Changes** | `04-data-model.md` + reglas Firebase | En mismo PR |
| **New Feature** | `SRS.md` + `10-project-context.md` | Antes de merge |
| **Changelog** | `09-changelog.md` (`[Unreleased]`) | En mismo PR |
| **Runbooks** | `14-deployment-runbook.md` | Si nuevo componente operativo |

---

## 9. Code Review Guide (Para Revisores)

### Qué SÍ revisar (High Value)
- [ ] **Correctness:** Lógica de negocio, edge cases, race conditions
- [ ] **Security:** AuthZ, validación, secrets, crypto, injection
- [ ] **Performance:** N+1 queries, bundle size, memory leaks, algoritmos
- [ ] **Maintainability:** Nombres claros, abstracciones adecuadas, DRY razonable
- [ ] **Testing:** Cobertura de casos críticos, tests determinísticos, nombres descriptivos
- [ ] **DX:** Error messages útiles, logs estructurados, types correctos
- [ ] **Docs:** ¿Se entiende sin preguntar? ¿ADR si decisión importante?

### Qué NO revisar (Low Value / Automatable)
- ❌ Formato (Prettier)
- ❌ Imports ordenados (ESLint)
- ❌ Variables no usadas (TypeScript)
- ❌ Estilo de comillas, punto y coma, tabs (Prettier)

### Comentarios Constructivos
```markdown
# Bueno 👍
> "En `calculateTotal` la función no maneja `quantity === 0` — deberíamos devolver 0 temprano para evitar división por cero en la línea 42."

# Malo 👎
> "Esto está mal." / "Arregla esto." / "No me gusta."
```

### Tiempos de Respuesta
- **PRs pequeños (< 300 líneas):** < 4 horas laborables
- **PRs medianos (300-800):** < 1 día laborable
- **PRs grandes (> 800):** Pedir split / < 2 días laborables

---

## 10. Versionado y Releases

- **SemVer** estricto (ver `09-changelog.md`)
- **Release Train:** `main` → `vX.Y.Z` tag → GitHub Release → Firebase Hosting deploy
- **Changelog:** Generado automáticamente (`release-drafter`) + curado manual
- **Hotfixes:** Branch `release/vX.Y.x` → cherry-pick → tag patch → deploy

---

## 11. Obteniendo Ayuda

| Canal | Para qué |
|-------|----------|
| Issues (GitHub) | Bugs, feature requests, preguntas |
| GitHub Discussions | RFCs, propuestas, Q&A persistente |
| `docs/10-project-context.md` | Onboarding, comandos, contactos |
| `make help` | Lista de comandos disponibles |

---

## 12. Licencia

Al contribuir, aceptas que tu código se licencie bajo **[MIT License](LICENSE)** (o la del proyecto).

---

## Historial de versiones

| Versión | Fecha | Autor | Cambios |
|---------|-------|-------|---------|
| 0.1 | 2024-11-20 | Matías Biem | Plantilla base |
| 1.0 | 2024-11-20 | Matías Biem | Contributing Guide Solar Monitoring v1.0 |