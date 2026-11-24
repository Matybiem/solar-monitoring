# Reglas del Proyecto / Project Rules

**Propósito:** Definir estándares y restricciones permanentes sobre cómo trabajar en este proyecto Solar Monitoring. Una rule es un contexto normativo ("qué estándares respetar siempre o al tocar ciertos archivos"), a diferencia de una skill que es un procedimiento bajo demanda.

---

## 1. Rama de Trabajo (Branch)

**Nunca trabajes directamente sobre `main`.**

- Siempre verifica en qué rama te encuentras antes de iniciar cualquier cambio:
  ```bash
  git branch --show-current
  ```
- Si estás en `main`, cambia a una rama feature antes de modificar código:
  ```bash
  git checkout -b feature/<nombre-correlativo>
  ```
- Las ramas `main` y `release/*` son de solo lectura para desarrolladores ordinarios. Cualquier cambio debe realizarse a través de PRs contra `main` con aprobación correspondiente.

**Antes de cambiar de rama:**
- Preguntar con permiso del administrador (obligatorio que lo haga el administrador) o confirmar dos veces en ventana emergente.
- Verificar que no haya cambios sin commit (`git status`, `git stash` si es necesario).

---

## 2. Modificaciones a la Base de Datos (Firebase Realtime DB)

**Siempre pedir permiso antes de modificar la estructura de la Firebase Realtime Database.**

- **Qué está permitido:** Agregar campos nuevos (opcionales) o campos de solo lectura.
- **Qué requiere permiso administrativo:** Renombrar campos, mover rutas principales, cambiar tipos de datos, modificar reglas de seguridad.
- **Procedimiento:** Esperar confirmación explícita del administrador (el usuario dueño del proyecto) antes de ejecutar `firebase deploy --only firestore:rules` o editar `infra/firebase/firestore.rules` directamente.

**Antes de modificar la base de datos:**
- Preguntar con permiso del administrador (obligatorio que lo haga el administrador) o confirmar dos veces en ventana emergente.
- Hacer backup del dato afectado (`firebase database:get /`) si es una modificación mayor.
- Documentar el cambio en `07-decisions-log.md` y `09-changelog.md`.

---

## 3. Convenios de Commits

- Mensajes en tiempo presente (`add telemetry send`, not `added`).
- Prefijo de tipo obligatorio: `feat:`, `fix:`, `docs:`, `style:`, `refactor:`, `perf:`, `test:`.
- Máximo 72 caracteres en la línea de asunto.
- Cuerpo opcional con motivación y cambio de comportamiento.

---

## 4. Estándares de Código

- Formatoador: `pnpm format` (Prettier configurado en `.vscode/settings.json`).
- Linter: `pnpm lint` (ESLint + reglas propias).
- Typecheck: `pnpm typecheck` (TypeScript strict mode).
- Nunca commitear secrets ni credenciales de WiFi (`wifi_secrets.h` está en `.gitignore`).

---

## 5. Flujo de Despliegue

- Los cambios a `main` déclencian GitHub Actions → CI → `firebase deploy` a `staging`.
- Los tags `vX.Y.Z` déclencian deploy a `production` tras aprobación manual en `GitHub Environment production`.
- Rollback: `git revert <commit>` + push → auto-deploy Firebase (< 2 min).

---

## 6. Convención de Nombrado de Ramas

Las ramas se nombran con el formato `tipo/<descripcion-en-kebab-case>`:

| Tipo | Uso | Ejemplo |
|------|-----|---------|
| `feature/` | Nueva funcionalidad | `feature/firmware-wifi`, `feature/firebase-dashboard` |
| `fix/` | Corrección de bug | `fix/telemetry-queue-overflow` |
| `docs/` | Documentación | `docs/deployment-runbook` |
| `refactor/` | Refactor sin cambios de comportamiento | `refactor/acquisition-init` |
| `perf/` | Mejoras de rendimiento | `perf/main-loop-delay` |
| `test/` | Agregar tests | `test/acquisition-unit` |
| `chore/` | Mantenimiento / build / CI | `chore/esp-idf-config` |

**Regla clave:** nunca trabajes directamente sobre `main`. Crea siempre una rama `feature/` (o el tipo correspondiente) antes de modificar código.

---

### Estado actual de la rama

- **Rama activa:** `feature/firmware-wifi`
- **Último commit:** `4892039` — `docs: add telemetry visualization options and project rules/skills`
- **Push:** `origin/feature/firmware-wifi` (trackeando)
- **PR abierto en:** https://github.com/Matybiem/solar-monitoring/pull/new/feature/firmware-wifi

### Consulta antes de proceder

Cualquier operación que involucre los puntos **1** o **2** de esta regla requiere confirmación del administrador (el usuario dueño del proyecto). No proceder sin respuesta afirmativa explícita.