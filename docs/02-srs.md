# Software Requirements Specification (SRS) / Especificación de Requisitos de Software

**Estado:** plantilla — adaptar a cada proyecto.

---

## Propósito del documento

Define **qué** debe hacer el sistema (funcional y no-funcional), sin entrar en **cómo** se implementa. Es la base para arquitectura, diseño, pruebas y validación. Basado en IEEE 830 / ISO/IEC 25010.

---

## 1. Introducción

### 1.1 Propósito del sistema
> Descripción breve (2-3 frases) del sistema, su dominio y valor principal.

### 1.2 Alcance del documento
- Qué subsistemas/módulos cubre esta SRS
- Qué queda fuera (referenciar otros docs si aplica)

### 1.3 Definiciones, acrónimos y abreviaturas
| Término | Definición |
|---------|------------|
| `[API]` | Application Programming Interface |
| `[MVP]` | Minimum Viable Product |

### 1.4 Referencias
- [Project Charter](01-project-charter.md)
- [Architecture](03-architecture.md)
- `[Estándares aplicables, ej: OWASP Top 10, WCAG 2.1]`

---

## 2. Descripción general del sistema

### 2.1 Perspectiva del producto
> Independiente / parte de suite / reemplazo de sistema legacy / integración con X.

### 2.2 Funciones principales (resumen)
| ID | Función | Descripción breve |
|----|---------|-------------------|
| F-01 | `[Autenticación]` | Login, registro, JWT, refresh tokens |
| F-02 | `[Dashboard]` | Visualización de métricas en tiempo real |

### 2.3 Características de los usuarios
| Perfil | Descripción | Nivel técnico |
|--------|-------------|---------------|
| Admin | Gestión completa | Alto |
| Usuario final | Consulta datos | Bajo |

### 2.4 Restricciones generales
- Navegadores soportados: `[Chrome 120+, Firefox 115+, Safari 16+]`
- Dispositivos: `[Desktop, Tablet, Mobile responsive]`
- Idiomas: `[ES, EN]`

---

## 3. Requisitos funcionales (RF)

### Formato estándar por requisito:

#### RF-[XXX]: [Título corto]
- **Descripción:** Narrativa clara de la funcionalidad.
- **Actor(es):** `[Quién la usa]`
- **Precondiciones:** `[Estado previo necesario]`
- **Postcondiciones:** `[Estado resultante]`
- **Flujo principal:**
  1. `[Paso 1]`
  2. `[Paso 2]`
- **Flujos alternativos / errores:**
  - A1: `[Condición]` → `[Acción]`
- **Reglas de negocio:** `[RN-01, RN-02]`
- **Prioridad:** `[Must / Should / Could / Won't]` (MoSCoW)
- **Trazabilidad:** `[Caso de uso / User Story ID]`

---

### 3.1 Módulo: Autenticación y Autorización
> Copiar bloque RF-[XXX] para cada requisito.

| ID | Título | Prioridad |
|----|--------|-----------|
| RF-001 | Registro de usuario con email/password | Must |
| RF-002 | Login con JWT + refresh token | Must |
| RF-003 | Recuperación de contraseña | Should |
| RF-004 | Roles y permisos (RBAC) | Must |

---

### 3.2 Módulo: [Nombre del módulo principal]
| ID | Título | Prioridad |
|----|--------|-----------|
| RF-100 | `[Funcionalidad core 1]` | Must |
| RF-101 | `[Funcionalidad core 2]` | Must |

---

## 4. Requisitos no-funcionales (RNF)

Basados en **ISO/IEC 25010**. Cada RNF debe ser **verificable** (medible).

| ID | Categoría (ISO 25010) | Requisito | Métrica / Criterio de aceptación |
|----|------------------------|-----------|----------------------------------|
| RNF-01 | **Rendimiento** | Tiempo de respuesta p95 | `< 500ms` en API reads, `< 2s` en writes |
| RNF-02 | **Rendimiento** | Throughput | `> 1000 req/s` en endpoint crítico |
| RNF-03 | **Disponibilidad** | Uptime mensual | `99.9%` (excluye mantenimiento planificado) |
| RNF-04 | **Seguridad** | Autenticación | `MFA opcional`, `bcrypt/argon2`, `HTTPS only` |
| RNF-05 | **Seguridad** | Autorización | `RBAC` verificado en cada endpoint |
| RNF-06 | **Seguridad** | Protección de datos | `Cifrado en tránsito (TLS 1.3) y reposo (AES-256)` |
| RNF-07 | **Usabilidad** | Accesibilidad | `WCAG 2.1 AA` |
| RNF-08 | **Usabilidad** | Responsive | `Mobile-first, breakpoints 320/768/1024/1440` |
| RNF-09 | **Mantenibilidad** | Cobertura de tests | `> 80%` unit, `> 60%` integration |
| RNF-10 | **Mantenibilidad** | Deuda técnica | `SonarQube Quality Gate passed` |
| RNF-11 | **Portabilidad** | Despliegue | `Docker multi-arch, k8s Helm chart` |
| RNF-12 | **Escalabilidad** | Horizontal | `Stateless services, cache distribuido` |

---

## 5. Requisitos de interfaz

### 5.1 Interfaces de usuario (UI)
- Referencia: `[Design System / Figma link]`
- Pantallas principales: `[Lista con mockups IDs]`

### 5.2 Interfaces de software (APIs)
- **REST API:** `[OpenAPI/Swagger spec link]`
- **WebSockets / Server-Sent Events:** `[Eventos en tiempo real]`
- **Integraciones externas:** `[Stripe, Auth0, Maps, etc.]`

### 5.3 Interfaz de hardware (si aplica)
- `[Dispositivos IoT, sensores, protocolo MQTT/HTTP]`

### 5.4 Interfaz de base de datos
- Esquema lógico: ver [Data Model](04-data-model.md)

---

## 6. Casos de uso / User Stories (resumen)

| ID | Título | Actor | RFs relacionados |
|----|--------|-------|------------------|
| UC-01 | Registrarse | Visitante | RF-001 |
| UC-02 | Ver dashboard | Usuario autenticado | RF-100, RF-101 |

---

## 7. Matriz de trazabilidad (resumen)

| Requisito | Caso de uso | Test case | Componente arquitectura |
|-----------|-------------|-----------|-------------------------|
| RF-001 | UC-01 | TC-AUTH-001 | Auth Service |
| RNF-04 | — | TC-SEC-001 | API Gateway + Auth |

---

## 8. Criterios de aceptación del MVP

| # | Criterio | Método de verificación |
|---|----------|------------------------|
| 1 | Usuario puede registrarse y loguearse | Test E2E + manual |
| 2 | Dashboard carga < 2s p95 | k6 load test |
| 3 | 0 vulnerabilidades críticas/altas | SAST/DAST en CI |

---

## Historial de versiones

| Versión | Fecha | Autor | Cambios |
|---------|-------|-------|---------|
| 0.1 | YYYY-MM-DD | `[Autor]` | Plantilla base |
| 1.0 | YYYY-MM-DD | `[Autor]` | Aprobado para desarrollo |