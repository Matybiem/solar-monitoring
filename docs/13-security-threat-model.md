# Security Requirements & Threat Model / Requisitos de Seguridad y Modelo de Amenazas

**Estado:** v1.0 — Solar Monitoring

---

## Propósito del documento

Identifica **activos, amenazas, vulnerabilidades y controles** desde el diseño (Shift Left). Sirve como base para revisiones de arquitectura, code review enfocado en seguridad, pentesting scope, y compliance.

---

## 1. Activos y Clasificación de Datos

| Activo | Tipo | Clasificación | Regulación | Dueño |
|--------|------|---------------|------------|-------|
| Credenciales dispositivo (Wi-Fi, Firebase config) | Auth | **Crítico / Restringido** | — | Desarrollador |
| Telemetría (voltaje, corriente, potencia, SoC, RSSI) | IoT Data | **Interno** | — | Desarrollador |
| Datos de dispositivo (nombre, ubicación, versión FW) | Device Metadata | **Interno** | — | Desarrollador |
| Claves API (Weather API) | Infra | **Crítico / Restringido** | — | Desarrollador |
| Código fuente | IP | **Confidencial** | — | Desarrollador |
| Logs de diagnóstico | Audit | **Interno** | — | Desarrollador |

---

## 2. Modelo de Amenazas (STRIDE por componente)

| Componente / Trust Boundary | Spoofing | Tampering | Repudiation | Info Disclosure | DoS | Elevation of Privilege |
|----------------------------|----------|-----------|-------------|-----------------|-----|------------------------|
| **Internet → Firebase** | ✅ Anonymous Auth + Custom Token | ✅ Reglas Firebase write-restringidas | ✅ Access logs Firebase | ✅ Reglas read restringidas si futuro auth | ✅ Rate limit Firebase (1000 writes/s) | ✅ `auth.uid == deviceId` |
| **Firebase → Frontend** | ✅ Anonymous Auth | ✅ Reglas read (público MVP) | ✅ Firebase access logs | ⚠️ Datos públicos por diseño (MVP) | ✅ Conexiones simultáneas (100) | N/A (lectura pública) |
| **Device → Firebase (REST)** | ✅ Custom Token (`uid=deviceId`) | ✅ Validación payload schema | ✅ Logs estructurados dispositivo | ✅ TLS 1.3, credenciales en NVS encriptado | ✅ Rate limit por dispositivo | ✅ Custom token claims limitados |
| **Frontend → Weather API** | ✅ API Key en env var | ✅ Respuesta validada (Zod) | ✅ Logs de consultas | ✅ No envía PII a Weather API | ✅ Rate limit API (1000/día) | N/A |
| **Firestore/Rules** | ✅ Firebase Auth | ✅ `.validate` en reglas | ✅ Firebase audit logs | ✅ Reglas declarativas | ✅ Cuotas Spark plan | ✅ `auth.token.admin` para admin |

---

## 3. Controles de Seguridad por Capa (Defense in Depth)

### 3.1 Aplicación / Firmware

| Control | Implementación | Verificación |
|---------|----------------|--------------|
| **AuthN Device** | Firebase Anonymous Auth + Custom Token mintado con `uid=deviceId` | Test Emulator, revisión código token minting |
| **AuthZ** | Reglas Firebase: `write: auth.uid == deviceId`, `read: true` (MVP) | Emulator tests, reglas versionadas |
| **Validación de entrada** | Zod schemas (frontend), validación manual en C (firmware) | Unit tests, schema tests |
| **Crypto at rest** | NVS Encrypted Partition (ESP32) para credenciales | Revisión código, `nvs_flash_init_partition` |
| **Crypto in transit** | TLS 1.3 (Firebase + Weather API), mbedTLS | SSL Labs, revisión configuración |
| **JTAG/UART debug** | Deshabilitado en producción (eFuse) | eFuse quemados, build production |
| **Secure Boot** | Bootloader + App firmados (ECDSA P-256) | Build production flags |
| **Flash Encryption** | AES-XTS 256, clave en eFuse | Build production flags |

### 3.2 Infraestructura / Cloud

| Área | Control | Herramienta / Verificación |
|------|---------|----------------------------|
| **Network** | Firebase gestionado (Google infra) | Firebase Console, status page |
| **Compute** | Firebase Hosting (CDN edge) | Firebase Console |
| **Secrets** | GitHub Secrets (Weather API key), NVS encrypted (device) | `git-secrets` scan en CI, `trufflehog` |
| **Monitoring** | Firebase Console metrics, structured logs dispositivo | Dashboard Firebase, alertas programadas |

### 3.3 Supply Chain (SLSA L1 target)

| Control | Implementación |
|---------|----------------|
| **Provenance** | GitHub Actions provenance (auto-generado) |
| **SBOM** | `syft` genera SBOM en cada build (futuro) |
| **Dependency Policy** | Dependabot + `pnpm audit` en CI |
| **Build Integrity** | Pinned versions, `package-lock.json` / `pnpm-lock.yaml` |

---

## 4. Matriz de Riesgos (Top 10 del proyecto)

| ID | Amenaza (STRIDE) | Activo | Likelihood | Impact | Risk Score | Control Actual | Residual | Dueño | Acción |
|----|------------------|--------|------------|--------|------------|----------------|----------|-------|--------|
| R-01 | Device impersonation (Spoofing) | Telemetría | Media | Alto | **Alto** | Custom Token `uid=deviceId` | Medio | Desarrollador | Rotar tokens, revisar claims |
| R-02 | Credentials leak (Info Disclosure) | Wi-Fi/Firebase config | Media | Crítico | **Alto** | NVS encrypted, no hardcodeado | Bajo | Desarrollador | `git-secrets` CI, rotación periódica |
| R-03 | Unauthorized write (Tampering) | Firebase DB | Media | Alto | **Alto** | Reglas `auth.uid == deviceId` | Bajo | Desarrollador | Emulator tests, reglas versionadas |
| R-04 | Public data exposure (Info Disclosure) | Telemetría (read público MVP) | Alta | Bajo | **Medio** | Datos no son PII, read público intencional | Bajo | Desarrollador | Revisar si futuro requiere auth |
| R-05 | DoS device/battery (DoS) | Dispositivo | Media | Alto | **Alto** | Deep sleep, backoff exponencial, buffer local | Medio | Desarrollador | Monitoreo uptime, alerta offline |
| R-06 | Supply chain attack (Tampering) | Build artifacts | Baja | Crítico | **Medio** | Pinned deps, Dependabot, GitHub provenance | Bajo | Desarrollador | SBOM, signed artifacts (v2) |
| R-07 | Insecure debug (EoP) | ESP32 JTAG/UART | Baja | Crítico | **Medio** | eFuse JTAG deshabilitado, build production | Bajo | Desarrollador | Verificar eFuse quemados en prod |
| R-08 | Weather API key leak (Info Disclosure) | API Key | Media | Medio | **Medio** | GitHub Secrets, env var (no en repo) | Bajo | Desarrollador | `git-secrets`, rotación si expuesta |
| R-09 | Firebase quota exhaustion (DoS) | Disponibilidad | Baja | Medio | **Medio** | Monitoreo 80%, retención 30d | Bajo | Desarrollador | Alertas Firebase, plan Blaze si escala |
| R-10 | XSS in dashboard (Tampering) | Frontend | Baja | Alto | **Medio** | React auto-escape, CSP estricta | Bajo | Desarrollador | Lighthouse CI, axe-core, DOMPurify si HTML user |

---

## 5. Incident Response (Respuesta a Incidentes)

| Fase | Acción | Herramienta / Runbook | SLA |
|------|--------|----------------------|-----|
| **Detect** | Alertas: Firebase Console, PagerDuty (opcional), logs dispositivo | `runbook-security-incident.md` | `< 15 min` |
| **Triage** | Severidad (SEV-1/2/3), asignar Incident Commander | `incident-commander-checklist.md` | `< 30 min` |
| **Contain** | Revocar tokens, bloquear IPs, feature flag kill-switch, escalar | `containment-playbooks/` | `< 1 hora` (SEV-1) |
| **Eradicate** | Rotar credenciales, patch vulnerabilidad, redeploy | `eradication-playbooks/` | `< 4 horas` (SEV-1) |
| **Recover** | Restore desde Firebase backup, smoke tests, monitor | `recovery-playbooks/` | `< 24 horas` (SEV-1) |
| **Postmortem** | Blameless, timeline, root cause (5 Whys), action items | `postmortem-template.md` | `< 5 días hábiles` |

---

## 6. Compliance & Privacy

| Requisito | Aplicabilidad | Implementación | Evidencia |
|-----------|---------------|----------------|-----------|
| **GDPR** | Si hay usuarios UE | DPIA, DSR automation, datos no son PII (telemetría anónima) | DPIA report |
| **CCPA/CPRA** | Si hay usuarios CA | Do Not Sell, opt-out, data inventory | Privacy policy |
| **SOC 2** | No (proyecto personal) | N/A | N/A |

---

## 7. Security Testing Program

| Tipo | Frecuencia | Scope | Herramienta |
|------|------------|-------|-------------|
| **SAST** | Every PR | 100% code | CodeQL, Semgrep |
| **SCA / Dependency Scan** | Every PR + Daily | All manifests | Dependabot, Trivy, Snyk |
| **DAST** | Nightly (Staging) | Running app | OWASP ZAP |
| **Firebase Rules Test** | Every PR | Rules + Emulator | Firebase Emulator + Vitest |
| **Penetration Test** | Annual + Major changes | Full scope | External firm (futuro) |
| **Bug Bounty** | Continuous | Public attack surface | No (proyecto personal) |

---

## 8. Security Training & Culture

| Actividad | Audiencia | Frecuencia |
|-----------|-----------|------------|
| Secure Code Review checklist | Dev | Onboarding + Quarterly |
| OWASP Top 10 workshop | Dev | Annual |
| Threat Modeling session | Tech Lead | Per major feature |
| Incident Response drill | Dev | Quarterly (tabletop) |

---

## Historial de versiones

| Versión | Fecha | Autor | Cambios |
|---------|-------|-------|---------|
| 0.1 | 2024-11-20 | Matías Biem | Plantilla base |
| 1.0 | 2024-11-20 | Matías Biem | Security Threat Model Solar Monitoring v1.0: STRIDE, riesgos, controles, incident response |