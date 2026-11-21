# Roadmap / Hoja de Ruta del Proyecto

**Estado:** v1.0 — Solar Monitoring

---

## Propósito del documento

Comunica la **visión temporal** del producto: qué se entrega, cuándo y por qué. Alinea stakeholders, equipo y expectativas.

---

## 1. Visión de producto (Product Vision)

**Solar Monitoring** es una plataforma IoT de portafolio que permite a propietarios de instalaciones solares pequeñas monitorear generación de energía, estado de batería y correlacionar con condiciones meteorológicas, todo operando primariamente con energía solar y presentado en un dashboard web responsive con diseño tipo Microsoft 365 (Fluent Design System).

**North Star Metric:** Dispositivos activos enviando telemetría cada 30s con dashboard disponible > 95% del tiempo.

---

## 2. Horizonte temporal

| Horizonte | Rango | Nivel de detalle | Cadencia de revisión |
|-----------|-------|------------------|----------------------|
| **Now** | 0-3 meses | Detallado (features, fechas, criterios) | Cada sprint |
| **Next** | 3-12 meses | Temas / Outcomes / Rough sizing | Trimestral |
| **Later** | 12+ meses | Visionario / Oportunidades / Research | Semestral |

---

## 3. Now (Próximos 3 meses) — Detallado

### Sprint / Release actual: `[v1.0 - MVP]` — Target: `2025-03-31`

| Epic / Feature | Outcome / Métrica | Estado | Owner | Estimación | Dependencias |
|----------------|-------------------|--------|-------|------------|--------------|
| Arquitectura modular (firmware capas + frontend features) | Componentes desacoplados, tests aislados | `[Done]` | Tech Lead | `8 SP` | ADRs 001, 010 |
| Firmware: adquisición INA219x2 + cálculo potencia/SoC | Lecturas estables, ±10% precisión estimada | `[In Progress]` | Desarrollador | `13 SP` | ADR-004, ADR-011 |
| Firmware: Wi-Fi + Firebase REST POST + buffer local | Telemetría recibida cada 30s ±5s | `[In Progress]` | Desarrollador | `8 SP` | ADR-002, ADR-004 |
| Frontend: Dashboard con Grid 12-col (Desktop 12, Tablet 8, Mobile 4) | Responsive verificado en 3 breakpoints | `[To Do]` | Desarrollador | `8 SP` | ADR-005, ADR-006 |
| Frontend: Lectura tiempo real desde Firebase (`onValue`) | Live indicator < 30s latencia | `[To Do]` | Desarrollador | `5 SP` | ADR-003 |
| Frontend: Gráficas tendencias generación (Recharts) | Visualización histórica | `[To Do]` | Desarrollador | `8 SP` | ADR-007 |
| Firebase Realtime Database + Reglas Seguridad | Write restringido / Read público | `[Done]` | Tech Lead | `5 SP` | ADR-008 |
| Diseño visual Fluent-like (tokens color, spacing, typo, motion) | Storybook + tokens publicados | `[Done]` | Tech Lead | `5 SP` | ADR-005 |
| CI/CD (GitHub Actions: lint, test, build, deploy preview) | Deploy automático en PR | `[Done]` | Tech Lead | `5 SP` | ADR-010 |
| Documentación completa (SRS, Arch, Data Model, API, ADRs, Glossary) | 100% docs actualizadas | `[In Progress]` | Todos | `8 SP` | — |

### Próximo Release: `[v1.1 - Hardening]` — Target: `2025-06-30`

| Epic / Feature | Outcome | Estado | Estimación |
|----------------|---------|--------|------------|
| Tests E2E críticos (Playwright) | 0 regresiones en deploy | `[Planned]` | `5 SP` |
| Observabilidad completa (logs estructurados, métricas, alertas) | MTTR < 15 min | `[Planned]` | `8 SP` |
| Rate limiting + validación de entrada | 0 abuso API / payload inválido | `[Planned]` | `3 SP` |
| Documentación API pública (OpenAPI) | DevX para integraciones | `[Planned]` | `2 SP` |
| Secure Boot + Flash Encryption (producción) | Seguridad HW activada | `[Planned]` | `5 SP` |
| Calibración SoC (procedimiento documentado) | Precisión ±10% validada | `[Planned]` | `8 SP` |

---

## 4. Next (3-12 meses) — Temas y Outcomes

| Tema | Outcome deseado | Iniciativas candidatas (rough) | T-shirt size |
|------|-----------------|--------------------------------|--------------|
| **Multi-device** | Soporte N dispositivos, dashboard multi-panel | `/devices/{deviceId}`, switching, comparativa | M |
| **OTA Firmware** | Actualización remota segura (Firebase Storage + ECDSA) | ADR-013 implementado | L |
| **Alertas (Email/Push/Telegram)** | Notificación umbral batería baja / dispositivo offline | Cloud Functions + SMTP / Telegram Bot | M |
| **Autenticación multi-usuario** | Login, roles, permisos por dispositivo | Firebase Auth + Firestore (si migra) | L |
| **Analytics / Predicción** | Tendencia generación, estimación producción próxima | ML ligero / reglas simples | L |
| **Provisioning BLE** | Configurar Wi-Fi desde móvil sin cable | ADR-014 implementado | M |
| **Home Assistant Integration** | Entidad sensor para ecosistema HA | Integración custom component | M |
| **i18n (ES/EN)** | Interfaz bilingüe | ICU messages, locale detection | S |

---

## 5. Later (12+ meses) — Visionario

| Idea / Oportunidad | Por qué importa | Validación necesaria |
|---------------------|-----------------|----------------------|
| **Edge Computing / Local-first** | Funcionamiento 100% offline, sync cuando hay conexión | POC: SQLite + Sync engine |
| **Digital Twin / 3D Visualization** | Diferenciación premium, vista 3D del sistema | Three.js / Spline, performance en mobile |
| **AI Assistant / Copilot** | Consultas lenguaje natural sobre datos históricos | RAG sobre datos, LLM integrado |
| **Hardware Partnerships** | Venta de kit / white-label | Certificación CE/FCC, BMS custom |
| **Open Source Core** | Comunidad, adopción, contribuciones | Licencia, governance, contributor experience |
| **SaaS Multi-tenant** | Modelo negocio, facturación suscripción | Stripe, RLS multi-tenant, billing |

---

## 6. Métricas de éxito por Release

| Release | North Star | Leading Indicators | Lagging Indicators |
|---------|------------|--------------------|--------------------|
| v1.0 MVP | 1 dispositivo activo, telemetría continua | 2880 lecturas/día, latencia < 30s, uptime > 90% | Demo portfolio funcional |
| v1.1 | 3 dispositivos, tests automatizados | Coverage > 70%, MTTR < 15 min, 0 regresiones | Tiempo de deploy < 10 min |
| v2.0 | Multi-device, OTA, alertas | N dispositivos, OTA éxito > 99%, alertas entregadas | ARR / adopción (si comercial) |

---

## 7. Dependencias y riesgos críticos

| Dependencia / Riesgo | Impacto si falla | Mitigación | Dueño |
|----------------------|------------------|------------|-------|
| Certificación eléctrica (BMS, protecciones) | No se puede energizar sistema | Revisión diseño antes de conectar, componentes certificados | Desarrollador |
| Radiación solar insuficiente (invierno/nublados) | Batería descarga, sistema offline | Deep sleep extendido, hibernation < 3.0V, panel dimensionado | Desarrollador |
| Wi-Fi inestable | Datos perdidos, gaps telemetría | Buffer local 10 lecturas, reintento exponencial, deep sleep | Desarrollador |
| Firebase límites Spark (1GB/10GB) | Lectura bloqueada, sobrecoste | Retención 30d, agregados diarios, monitoreo 80% | Tech Lead |
| Clima API rate limit | Widget clima sin datos | Cache 10min, fallback graceful, múltiples proveedores | Desarrollador |
| Contratación tiempo (proyecto personal) | Velocidad reducida | Priorización ruthless, MVP scope reducido | PO |

---

## 8. Formato de presentación (para stakeholders)

### Vista Kanban (Now / Next / Later)
```
┌─────────────┬─────────────┬─────────────┐
│    NOW      │    NEXT     │    LATER    │
│ (0-3 mos)   │ (3-12 mos)  │ (12+ mos)   │
├─────────────┼─────────────┼─────────────┤
│ 🟢 Firmware │ 🟡 OTA FW   │ 🔵 Edge     │
│ 🟢 Dashboard│ 🟡 Alertas  │ 🔵 AI       │
│ 🟢 Firebase │ 🟡 Multi-dev│ 🔵 3D Twin  │
│ 🟢 Grid 12-col│ 🟡 Auth  │ 🔵 OpenSrc  │
└─────────────┴─────────────┴─────────────┘
```

### Vista Timeline (Gantt simplificado)
```
Q1 2025          Q2 2025          Q3 2025          Q4 2025
├────────────────┼────────────────┼────────────────┼────────────────┤
████████████████ │                │                │                │  v1.0 MVP
                ████████████████  │                │                │  v1.1 Hardening
                                   ████████████████ │                │  v2.0 Multi-dev
                                                        ████████████████ │  v2.1 OTA
```

---

## 9. Proceso de priorización (RICE)

| Framework | Fórmula | Cuándo usar |
|-----------|---------|-------------|
| **RICE** | `(Reach × Impact × Confidence) / Effort` | Features de producto |
| **ICE** | `Impact × Confidence × Ease` | Decisiones rápidas |
| **WSJF** | `Cost of Delay / Job Duration` | Portfolio grande |

> **Regla:** Todo item en Now debe tener score calculado. Next/Later: estimación rough.

---

## Historial de versiones

| Versión | Fecha | Autor | Cambios |
|---------|-------|-------|---------|
| 0.1 | 2024-11-20 | Matías Biem | Plantilla base |
| 1.0 | 2024-11-20 | Matías Biem | Roadmap Solar Monitoring v1.0: MVP Q1 2025, Next Q2-Q4 2025 |