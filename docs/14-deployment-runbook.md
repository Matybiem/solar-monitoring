# Deployment & Runbook / Despliegue y Runbooks Operacionales

**Estado:** v1.0 — Solar Monitoring

---

## Propósito del documento

Centraliza **cómo se despliega, opera y recupera** el sistema. Reduce bus factor, acelera on-call, estandariza incidentes.

---

## 1. Arquitectura de Despliegue (Resumen)

| Capa | Tecnología | Estrategia |
|------|------------|------------|
| **Source Control** | GitHub (monorepo) | Trunk-based + short-lived feature branches |
| **CI** | GitHub Actions | Lint → Typecheck → Unit → Integration → Contract → SAST → Build → Test Firebase Emulator |
| **Artifacts** | GitHub Releases (firmware .bin), Firebase Hosting (web) | SemVer tags (`v1.2.3`) |
| **CD / GitOps** | Firebase Hosting (auto-deploy desde main) | Preview en PRs, manual-sync prod |
| **Infra** | Firebase (managed), GitHub Actions | State gestionado por Firebase |
| **Environments** | `preview` (PR), `staging`, `production` | Preview = efímero, Staging = shared, Prod = Firebase Spark |

---

## 2. Pipeline de Release (Release Train)

```
┌─────────────┐   ┌─────────────┐   ┌─────────────┐   ┌─────────────┐
│   Develop   │──►│   Main      │──►│  Release    │──►│  Production │
│  (feature)  │   │  (merge)    │   │  Candidate  │   │  (manual)   │
└─────────────┘   └─────────────┘   └─────────────┘   └─────────────┘
     │                 │                 │                 │
     ▼                 ▼                 ▼                 ▼
  CI Checks       CI + Firebase    CI + E2E +         Manual Approval
  (lint, test)    Emulator + Build  Load Test          + Canary Deploy
                  Docker Image     Docker Image       + Smoke Tests
                  (web)            tagged RC
```

### 2.1 Versionado (SemVer + Git Tags)
```bash
# Patch: bugfix
git tag -a v1.2.3 -m "v1.2.3" && git push origin v1.2.3

# Minor: feature
git tag -a v1.3.0 -m "v1.3.0" && git push origin v1.3.0

# Major: breaking
git tag -a v2.0.0 -m "v2.0.0" && git push origin v2.0.0

# Pre-release
git tag -a v1.3.0-rc.1 -m "v1.3.0-rc.1" && git push origin v1.3.0-rc.1
```

### 2.2 Changelog Automático
- `release-drafter` genera borrador desde PR labels (`feat`, `fix`, `docs`, `perf`, `security`)
- Mantenedor revisa → publica Release en GitHub → Firebase Hosting detecta tag → deploy

---

## 3. Runbooks por Entorno

### 3.1 Preview Environment (PR Efímero)
**Trigger:** PR opened / synchronized
**TTL:** 24 horas sin actividad → auto-destroy (Firebase Hosting preview)
**URL:** `https://pr-{number}--solar-mon.web.app`

| Acción | Comando / UI |
|--------|--------------|
| Ver logs | `gh run view --log` / Firebase Hosting dashboard |
| Re-deploy | Push nuevo commit a PR branch |
| Extender TTL | Firebase Hosting preview expira automáticamente |

### 3.2 Staging (Compartido)
**Trigger:** Merge a `main` (auto-deploy Firebase Hosting)
**Datos:** Firebase Emulator o staging project
**URL:** `https://solar-mon-staging.web.app`

| Acción | Runbook |
|--------|---------|
| Deploy manual | `firebase deploy --only hosting` (staging project) |
| Rollback | `firebase hosting:rollback` o revertir commit + push |
| Firebase Rules | `firebase deploy --only firestore:rules` |
| Test Emulator | `firebase emulators:exec "pnpm test"` |

### 3.3 Producción (Firebase Spark)
**Trigger:** Tag `vX.Y.Z` + Manual Approval en GitHub Environment `production`
**Estrategia:** Canary (Firebase Hosting no tiene canary nativo; usar preview + promote)
**URL:** `https://solar-mon.web.app`

| Paso | Acción | Verificación | Rollback si |
|------|--------|--------------|-------------|
| 1 | `gh workflow run deploy-prod.yml -f tag=v1.2.3` | Workflow green | — |
| 2 | Firebase Hosting deploy | `firebase hosting:channel:deploy live` | Error rate > 1% |
| 3 | Smoke tests automatizados | `k6 run smoke.js` | Fail |
| 4 | Promote 100% | Metrics: Firebase console, errors, business KPIs | Regression |
| 5 | Post-deploy verification | Synthetic monitoring, error budget | — |

**Rollback inmediato:** `git revert <commit>` + push → auto-deploy (< 2 min)

---

## 4. Runbooks de Incidentes Comunes

| Runbook | Trigger | Owner | SLA |
|---------|---------|-------|-----|
| `high-error-rate.md` | Alert: Firebase errors > 1% | On-call | 15 min |
| `device-offline.md` | Alert: dispositivo sin telemetría > 10min | On-call | 30 min |
| `firebase-quota.md` | Alert: Firebase storage/download > 80% | Platform | 24 horas |
| `firmware-flash-failed.md` | No se puede flashear ESP32 | Desarrollador | 1 hora |
| `cert-expiry-warning.md` | Alert: certificado expira < 30d | Platform | 24 horas |
| `weather-api-fail.md` | Widget clima sin datos | Desarrollador | 4 horas |

---

## 5. Procedimientos Operativos Estándar (SOPs)

### 5.1 Acceso a Firebase (Producción)
```bash
# 1. Autenticar con Firebase CLI
firebase login --reauth

# 2. Seleccionar proyecto
firebase use solar-mon-prod

# 3. Desplegar reglas
firebase deploy --only firestore:rules

# 4. Desplegar hosting
firebase deploy --only hosting

# 5. Verificar
firebase hosting:channel:list
```

### 5.2 Rotación de Secrets (Weather API key)
```bash
# 1. Generar nueva key en OpenWeatherMap
# 2. Actualizar en GitHub Secrets (repository settings)
# 3. Redeploy a staging + prod
# 4. Verificar widget clima funciona
# 5. Revocar key antigua (opcional)
```

### 5.3 Database Migration Segura (Firebase Realtime DB)
Firebase no tiene migraciones formales. Estrategia:
- **Campos nuevos:** Agregar opcionales, default en lectura
- **Campos renombrados:** Mantener ambos 1 release (`new ?? legacy`)
- **Estructura mayor:** Nueva ruta paralela (`/telemetry_v2/`) + Cloud Function backfill + switch atómico

### 5.4 Certificate Renewal (Firebase Hosting)
Firebase Hosting gestiona certificados SSL automáticamente (Let's Encrypt). Verificación:
```bash
firebase hosting:channel:list
# Verificar certificado válido en navegador (icono candado)
```

---

## 6. Observabilidad para Operaciones

### 6.1 Dashboards (Firebase Console)
| Dashboard | Uso | Link |
|-----------|-----|------|
| **Realtime DB** | Conexiones, ancho de banda, almacenamiento | Firebase Console > Build > Realtime Database |
| **Hosting** | Despliegues, tráfico, errores | Firebase Console > Hosting |
| **Authentication** | Usuarios anónimos, tokens | Firebase Console > Build > Authentication |

### 6.2 Alertas Críticas
| Alerta | Condición | Severidad | Runbook |
|--------|-----------|-----------|---------|
| `Device Offline` | Sin telemetría > 10min | Warning | `device-offline.md` |
| `Firebase Quota` | Storage/download > 80% | Warning | `firebase-quota.md` |
| `High Error Rate` | Firebase errors > 1% | Warning | `high-error-rate.md` |

---

## 7. Backup & Disaster Recovery

| Componente | RPO | RTO | Método | Verificación |
|------------|-----|-----|--------|--------------|
| **Firebase Realtime DB** | < 5 min | < 15 min | Firebase managed backup (daily) | Monthly restore test |
| **Firebase Hosting** | 0 | < 5 min | Git history + redeploy | Deploy from tag |
| **Firmware binarios** | Cada release | < 1 hora | GitHub Releases (artifacts firmados) | SHA256 verify |
| **Secrets** | 0 | < 5 min | GitHub Secrets (managed) | N/A |

---

## 8. Capacity Planning & Scaling

| Recurso | Métrica Trigger | Acción | Automatización |
|---------|-----------------|--------|----------------|
| **Firebase Connections** | `> 80` concurrentes | Upgrade plan / optimizar listeners | ⚠️ Manual |
| **Firebase Storage** | `> 800 MB` | Limpieza > 30d + Cloud Function | ✅ Automático (CF) |
| **Firebase Download** | `> 8 GB/mes` | Compresión payload / reducir frecuencia | ⚠️ Manual |
| **Weather API calls** | `> 800/día` | Cache 10min → 15min, reducir ubicación | ⚠️ Manual |

---

## 9. Contactos de Escalación (On-Call)

| Nivel | Rol | Contacto | Horario | Backup |
|-------|-----|----------|---------|--------|
| **L1** | On-Call Engineer (Desarrollador único) | Matías Biem | 24/7 rotación semanal | — |
| **L2** | Tech Lead / Platform | Matías Biem | Business hours | — |
| **L3** | Engineering Manager | Matías Biem | Business hours | — |
| **Security** | Security Engineer | Matías Biem | 24/7 | — |
| **Firebase Support** | Firebase Support (Spark plan limitado) | Firebase Console | Business hours | — |

---

## 10. Checklist de Release (Definition of Done para Deploy)

- [ ] CI Pipeline verde en `main` (lint, typecheck, test, build, scans)
- [ ] Changelog actualizado (`[Unreleased]` → `[vX.Y.Z]`)
- [ ] Tag Git creado y firmado (`git tag -s`)
- [ ] Docker images (si aplica) + firmware `.bin` en GitHub Releases
- [ ] SBOM generado (futuro)
- [ ] Firebase Rules desplegadas y testeadas en Emulator
- [ ] Staging deployado y smoke tests pasan
- [ ] Feature flags configurados (off por defecto)
- [ ] Dashboards/Alertas actualizados para nuevas métricas
- [ ] Aprobación manual en GitHub Environment `production`
- [ ] Canary/deploy iniciado + monitoreo activo
- [ ] Post-deploy verification (synthetic + business KPIs)
- [ ] Rollback plan confirmado y probado en staging
- [ ] Comunicación a stakeholders (Slack `#releases`)

---

## Historial de versiones

| Versión | Fecha | Autor | Cambios |
|---------|-------|-------|---------|
| 0.1 | 2024-11-20 | Matías Biem | Plantilla base |
| 1.0 | 2024-11-20 | Matías Biem | Deployment Runbook Solar Monitoring v1.0: Firebase Hosting, GitHub Actions, runbooks, SOPs, DR |