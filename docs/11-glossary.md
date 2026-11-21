# Glossary / Glosario del Proyecto

**Estado:** v1.0 — Solar Monitoring

---

## Propósito del documento

Elimina ambigüedad: **todos** (dev, PO, QA, stakeholders) usan las mismas definiciones. Evita "creo que X significa Y" en reuniones y código.

---

## Convenciones

| Campo | Descripción |
|-------|-------------|
| **Término** | Palabra o frase exacta (case-sensitive si aplica) |
| **Definición** | Clara, concisa, accionable. Una frase ideal. |
| **Contexto** | Dominio/módulo donde aplica (ej: `Auth`, `Firmware`, `IoT`) |
| **Sinónimos / No usar** | Términos equivalentes y confusiones comunes |
| **Relacionado** | Otros términos del glosario |

---

## Glosario maestro (orden alfabético)

### A

| Término | Definición | Contexto | Sinónimos / No usar | Relacionado |
|---------|------------|----------|---------------------|-------------|
| **ADR** | Architecture Decision Record — documento que registra una decisión de arquitectura y su contexto | Documentación | `Registro de decisiones` | `07-decisions-log.md` |
| **API Gateway** | Punto de entrada único para clientes (no aplicable MVP, usamos Firebase directo) | Infra | `Edge`, `Proxy` | `Kong`, `Ingress` |
| **AuthZ / Authorization** | Decisión de qué puede hacer un usuario autenticado | Auth | `Permisos`, `RBAC` | `AuthN`, `Role`, `Permission` |
| **AuthN / Authentication** | Verificación de identidad (credenciales, tokens, MFA) | Auth | `Login`, `Identificación` | `AuthZ`, `JWT`, `Session` |

### B

| Término | Definición | Contexto | Sinónimos / No usar | Relacionado |
|---------|------------|----------|---------------------|-------------|
| **BMS** | Battery Management System — protección de batería (sobrecarga, descarga profunda, sobrecorriente, cortocircuito) | Hardware | `Protección batería` | `18650`, `Charge Controller` |
| **Buffer Circular** | Almacén temporal en RAM/Flash para lecturas sin conexión (máx 10 lecturas) | Firmware | `Cola`, `Queue` | `Deep Sleep`, `Reconexión` |
| **Charge Controller** | Dispositivo que regula carga de batería desde panel solar | Hardware | `Regulador carga` | `Panel Solar`, `18650`, `BMS` |

### C

| Término | Definición | Contexto | Sinónimos / No usar | Relacionado |
|---------|------------|----------|---------------------|-------------|
| **CI / Continuous Integration** | Build + test automático en cada push/PR | DevOps | `Pipeline`, `Build` | `CD`, `GitHub Actions` |
| **CD / Continuous Deployment** | Deploy automático a staging/prod tras CI exitoso | DevOps | `Deploy`, `Release` | `CI`, `Firebase Hosting` |
| **Cloud Function** | Función serverless ejecutada en Firebase/Google Cloud | Cloud | `Función serverless` | `AggregatedDaily`, `Limpieza` |
| **Coulomb Counting** | Método estimación SoC integrando corriente medida a lo largo del tiempo | Firmware / Batería | `Integración corriente` | `OCV`, `SoC` |
| **CSP** | Content Security Policy — cabecera de seguridad para prevenir XSS | Seguridad Web | `Cabecera CSP` | `Helmet`, `XSS` |

### D

| Término | Definición | Contexto | Sinónimos / No usar | Relacionado |
|---------|------------|----------|---------------------|-------------|
| **Deep Sleep** | Modo ultra-bajo consumo del ESP32 (10-20µA), wake por RTC timer o GPIO | Firmware | `Sueño profundo` | `Light Sleep`, `Duty Cycling` |
| **Device ID** | Identificador único del dispositivo: `solar-mon-{MAC_suffix}` | Firmware / DB | `ID dispositivo`, `UID` | `Telemetry`, `Firebase Rules` |
| **DLQ / Dead Letter Queue** | Cola para mensajes fallidos tras reintentos agotados | Messaging | `Dead Queue` | `Buffer Circular`, `Reintentos` |
| **Duty Cycling** | Ciclo activo/sleep para ahorro energía (30s activo / 5min sleep) | Firmware / Energía | `Ciclo activo/sleep` | `Deep Sleep`, `Power Management` |

### E

| Término | Definición | Contexto | Sinónimos / No usar | Relacionado |
|---------|------------|----------|---------------------|-------------|
| **E2E Tests** | Pruebas end-to-end que simulan usuario real | Testing | `E2E`, `Funcionales` | `Playwright`, `Happy Paths` |
| **ESP32-C3** | Microcontrolador RISC-V con Wi-Fi 2.4GHz + BLE 5, 400KB SRAM, 4MB Flash | Hardware | `MCU`, `ESP32` | `INA219`, `Deep Sleep`, `Firmware` |
| **Elevation** | Profundidad visual en UI (sombras, capas) en Fluent Design | UI / UX | `Sombras`, `Z-index` | `Design Tokens`, `Material` |

### F

| Término | Definición | Contexto | Sinónimos / No usar | Relacionado |
|---------|------------|----------|---------------------|-------------|
| **Feature Flag** | Toggle runtime para habilitar/deshabilitar funcionalidad sin deploy | Release | `Feature Toggle`, `Kill Switch` | `LaunchDarkly`, `Unleash` |
| **Fluent Design** | Sistema de diseño Microsoft: luz, profundidad, movimiento, material, escala | UI / UX | `Microsoft 365 style`, `Fluent` | `Design Tokens`, `Grid 12-col` |
| **Firebase Realtime DB** | Base de datos NoSQL en tiempo real de Google (WebSocket nativo) | Cloud / DB | `Firebase`, `RTDB` | `Realtime Listener`, `Reglas Seguridad` |
| **Firmware** | Software embebido en ESP32-C3; se actualiza vía Serial/UART (MVP) o OTA (v2) | Firmware | `Embedded SW`, `Binary` | `ESP-IDF`, `OTA`, `Deep Sleep` |

### G

| Término | Definición | Contexto | Sinónimos / No usar | Relacionado |
|---------|------------|----------|---------------------|-------------|
| **GitOps** | Operaciones de infra/app declarativas vía Git | DevOps | `Git-based Ops` | `ArgoCD`, `Firebase Hosting` |
| **Grid 12-col** | Sistema de rejilla de 12 columnas: Desktop 12, Tablet 8, Mobile 4 | UI / UX | `Rejilla 12`, `Grid System` | `Breakpoints`, `Responsive`, `Fluent Design` |

### I

| Término | Definición | Contexto | Sinónimos / No usar | Relacionado |
|---------|------------|----------|---------------------|-------------|
| **INA219** | Sensor I2C de monitorización de corriente y voltaje (TI) | Hardware | `Sensor corriente`, `Shunt` | `ESP32-C3`, `Telemetry`, `I2C` |
| **IoT** | Internet of Things — dispositivos conectados que intercambian datos | General | `Internet de las cosas` | `ESP32-C3`, `Telemetry`, `Firmware` |
| **I2C** | Protocolo de comunicación serial sincrónica (2 cables: SDA, SCL) | Hardware | `Bus I2C` | `INA219`, `ESP32-C3` |

### L

| Término | Definición | Contexto | Sinónimos / No usar | Relacionado |
|---------|------------|----------|---------------------|-------------|
| **Light Sleep** | Modo bajo consumo ESP32 (0.5-1.5mA), wake rápido | Firmware | `Sueño ligero` | `Deep Sleep`, `Duty Cycling` |
| **Liveness** | Indica si dispositivo/sistema está vivo (heartbeat) | Observability | `Heartbeat`, `Ping` | `RSSI`, `Última actualización` |

### M

| Término | Definición | Contexto | Sinónimos / No usar | Relacionado |
|---------|------------|----------|---------------------|-------------|
| **Material** | Concepto de diseño en Fluent: capas con profundidad y textura | UI / UX | `Diseño material` | `Elevation`, `Profundidad` |
| **Monorepo** | Un repositorio con múltiples apps/packages compartiendo tooling | Repo Strategy | `Mono-repo` | `Turborepo`, `apps/`, `packages/` |
| **MTTR** | Mean Time To Recovery — tiempo medio para recuperar tras incidente | SRE | `Tiempo de recuperación` | `MTBF`, `MTTD`, `Observabilidad` |

### N

| Término | Definición | Contexto | Sinónimos / No usar | Relacionado |
|---------|------------|----------|---------------------|-------------|
| **NVS** | Non-Volatile Storage — almacenamiento persistente enFlash ESP32 (encriptado) | Firmware | `Almacenamiento NVS` | `Credenciales`, `Configuración`, `Calibración` |

### O

| Término | Definición | Contexto | Sinónimos / No usar | Relacionado |
|---------|------------|----------|---------------------|-------------|
| **OCV / Open Circuit Voltage** | Voltaje de batería en reposo (sin carga/descarga) — base para estimación SoC | Firmware / Batería | `Voltaje reposo` | `Coulomb Counting`, `SoC` |
| **OTA / Over-The-Air** | Actualización remota de firmware sin conexión física (v2) | Firmware | `Actualización remota`, `FOTA` | `Bootloader`, `Firma ECDSA`, `Rollback` |

### P

| Término | Definición | Contexto | Sinónimos / No usar | Relacionado |
|---------|------------|----------|---------------------|-------------|
| **Panel Solar** | Dispositivo que convierte radiación solar en energía eléctrica | Hardware | `Placa solar`, `Módulo PV` | `Charge Controller`, `INA219 #1` |
| **Provisioning** | Proceso de registrar y configurar dispositivo por primera vez (Wi-Fi, creds) | Firmware / IoT | `Onboarding`, `Configuración inicial` | `BLE (v2)`, `Serial CLI (MVP)` |
| **Push Notification** | Notificación enviada a dispositivo móvil (excluido MVP) | Features | `Notificación push` | `Alerta`, `Telegram` |

### R

| Término | Definición | Contexto | Sinónimos / No usar | Relacionado |
|---------|------------|----------|---------------------|-------------|
| **RSSI** | Received Signal Strength Indicator — intensidad señal Wi-Fi (dBm, negativo) | Firmware / Red | `Intensidad señal`, `Señal Wi-Fi` | `Telemetry`, `Conectividad` |
| **SoC / State of Charge** | Estado de carga estimado de batería (0-100%) | Firmware / Batería | `Estado carga`, `Nivel batería` | `OCV`, `Coulomb Counting`, `INA219 #2` |
| **SRS** | Software Requirements Specification — especificación de requisitos de software | Documentación | `Especificación requisitos` | `SRS.md` |
| **Turborepo** | Monorepo build system para JavaScript/TypeScript | DevOps | `Monorepo tool` | `apps/`, `packages/`, `CI/CD` |

### S

| Término | Definición | Contexto | Sinónimos / No usar | Relacionado |
|---------|------------|----------|---------------------|-------------|
| **SaaS** | Software as a Service — software hosteado centralmente, multi-tenant | Business | `Software en la nube` | `Multi-tenancy`, `Billing` |
| **Spark Plan** | Plan gratuito de Firebase (1GB DB, 10GB DL/mes, 10GB Hosting) | Cloud | `Plan gratuito Firebase` | `Firebase Realtime DB` |
| **Telemetry** | Datos de medición y estado transmitidos remotamente (voltaje, corriente, potencia, SoC, RSSI) | IoT / Firmware | `Datos`, `Mediciones`, `Readings` | `Device`, `Firebase`, `Dashboard` |
| **Timeseries DB** | Base de datos optimizada para datos temporales (no usada MVP, Firebase plano) | Database | `TSDB` | `Telemetry`, `AggregatedDaily` |

### U

| Término | Definición | Contexto | Sinónimos / No usar | Relacionado |
|---------|------------|----------|---------------------|-------------|
| **UART** | Universal Asynchronous Receiver-Transmitter — comunicación serial para debug/flash | Firmware | `Serial`, `USB-Serial` | `Flash`, `Debug`, `Provisioning MVP` |
| **Uptime** | Tiempo que el dispositivo/systema ha estado operativo sin interrupción | Observability | `Tiempo activo` | `Deep Sleep`, `Heartbeat` |

### V

| Término | Definición | Contexto | Sinónimos / No usar | Relacionado |
|---------|------------|----------|---------------------|-------------|
| **Vite** | Build tool y dev server rápido para frontend (React) | Frontend | `Bundler`, `Dev server` | `React`, `TypeScript` |
| **Voltage** | Diferencia de potencial eléctrico (Voltios) | Hardware / Telemetry | `Voltaje`, `Tensión` | `INA219`, `Potencia (W = V × I)` |

### W

| Término | Definición | Contexto | Sinónimos / No usar | Relacionado |
|---------|------------|----------|---------------------|-------------|
| **Webhook** | Callback HTTP POST ante evento (async, fire-and-forget) | Integration | `Callback`, `HTTP Callback` | `Evento`, `Alerta` |
| **Wi-Fi 2.4GHz** | Banda de conectividad inalámbrica usada por ESP32-C3 | Hardware / Red | `Wi-Fi`, `Inalámbrico` | `ESP32-C3`, `RSSI`, `Reconexión` |

---

## Cómo mantenerlo

1. **Nuevo término** → PR con entrada en sección alfabética
2. **Cambio de definición** → PR + notificar en canal del proyecto
3. **Revisión trimestral** → Owner: Tech Lead + PO
4. **En código:** Usar términos exactos del glosario en variables, tipos, comentarios

---

## Historial de versiones

| Versión | Fecha | Autor | Cambios |
|---------|-------|-------|---------|
| 0.1 | 2024-11-20 | Matías Biem | Plantilla base |
| 1.0 | 2024-11-20 | Matías Biem | Glosario Solar Monitoring v1.0 con 50+ términos del dominio |