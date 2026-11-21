# API Specification / Especificación de API

**Estado:** v1.0 — Solar Monitoring MVP

---

## Propósito del documento

Define la **interfaz contractual** entre firmware (device), frontend (dashboard), Firebase Realtime Database, y servicios externos (Weather API). Fuente de verdad: **OpenAPI 3.1** (archivo `openapi.yaml` versionado en repo). Este doc resume decisiones, patrones y guías.

---

## 1. Principios de diseño API

| Principio | Descripción |
|-----------|-------------|
| **Firebase-first** | MVP usa Firebase Realtime Database como "API" nativa (WebSocket + REST). No backend propio. |
| **Device ↔ Firebase** | HTTPS REST (Firebase HTTP API) o Firebase C++ SDK en ESP32. Payload JSON. |
| **Frontend ↔ Firebase** | Firebase Web SDK (Realtime listeners `onValue`, queries `orderByKey`). |
| **OpenAPI para Weather** | Weather API externa documentada con OpenAPI 3.1 para type-safety. |
| **Versionado implícito** | `schema_version` en payload telemetría. Cambios breaking = v2 path. |
| **Idempotencia** | Device envía timestamp único → Firebase key = timestamp → upsert natural. |
| **Observabilidad** | `Request-ID` (correlation) en headers Firebase REST. Logs estructurados. |

---

## 2. Estructura Base URLs

| Entorno | Firebase Project | Database URL | Hosting URL |
|---------|------------------|--------------|-------------|
| **Local (Emulator)** | `demo-solar-mon` | `http://localhost:9000` | `http://localhost:5000` |
| **Staging** | `solar-mon-staging` | `https://solar-mon-staging-default-rtdb.firebaseio.com` | `https://solar-mon-staging.web.app` |
| **Producción** | `solar-mon-prod` | `https://solar-mon-prod-default-rtdb.firebaseio.com` | `https://solar-mon.web.app` |

**Weather API:** `https://api.openweathermap.org/data/3.0/onecall` (OpenWeatherMap One Call 3.0)

---

## 3. Autenticación y Autorización

### 3.1 Esquema MVP
| Actor | Método | Detalles |
|-------|--------|----------|
| **Device (ESP32)** | Anonymous Auth + Custom Token | Token mintado con `uid = deviceId`, claims `{ deviceId }`. Renovación cada 24h o al boot. |
| **Frontend (Dashboard)** | Anonymous Auth | `signInAnonymously()` al cargar. Solo lectura pública. |
| **Admin / Cloud Functions** | Service Account / Admin SDK | `auth.token.admin == true` en rules. |

### 3.2 Claims estándar (Custom Token Device)
```json
{
  "uid": "solar-mon-a1b2c3",
  "claims": {
    "deviceId": "solar-mon-a1b2c3",
    "type": "device"
  },
  "iat": 1700000000,
  "exp": 1700086400
}
```

### 3.3 Permisos (Firebase Rules - ver Data Model)
| Recurso | Lectura | Escritura |
|---------|---------|-----------|
| `/telemetry/{deviceId}/**` | Público (`true`) | `auth.uid == deviceId` |
| `/devices/{deviceId}` | Público | `auth.uid == deviceId \|\| auth.token.admin` |
| `/weather_cache/**` | Público | `auth.token.admin` |
| `/aggregated/**` | Público | `auth.token.admin` |

---

## 4. Endpoints / Interfaces Firebase Realtime Database

> Firebase Realtime Database no es REST tradicional; se accede vía **WebSocket (SDK)** o **REST HTTP**. Documentamos ambos patrones.

### 4.1 Device → Firebase (Telemetría)

#### REST HTTP (usado por firmware ESP32)
```
POST https://{projectId}-default-rtdb.firebaseio.com/telemetry/{deviceId}/{timestamp}.json?auth={ID_TOKEN}
```

**Request Body:**
```json
{
  "schema_version": 1,
  "deviceId": "solar-mon-a1b2c3",
  "timestamp": 1700000000000,
  "solar_voltage": 18.45,
  "solar_current": 2.31,
  "solar_power": 42.62,
  "battery_voltage": 3.87,
  "battery_current": 1.15,
  "battery_soc_est": 87.5,
  "rssi": -67,
  "fw_version": "v1.2.3",
  "uptime_sec": 3600,
  "free_heap_bytes": 185000,
  "wake_reason": "timer"
}
```

**Response (200 OK):**
```json
{ "name": "1700000000000" }
```

**Errores:**
| Código | Causa | Acción Device |
|--------|-------|---------------|
| 401 | Token expirado/inválido | Renovar token (re-auth anónimo + custom) |
| 403 | deviceId mismatch | Verificar uid en token |
| 429 | Rate limit Firebase | Backoff exponencial + deep sleep |
| 5xx | Error servidor Firebase | Buffer local + reintento próximo ciclo |

#### Firebase C++ SDK (alternativa)
```cpp
// Firebase Realtime Database C++ SDK
firebase::database::DatabaseReference ref = db.GetReference("telemetry").Child(deviceId).Child(timestamp);
ref.SetValue(json_payload);
```

---

### 4.2 Frontend ↔ Firebase (Dashboard)

#### Tiempo Real (Listener) — Última medición
```typescript
// apps/web/src/features/dashboard/api/telemetry.ts
import { ref, onValue } from 'firebase/database';
import { database } from '@/shared/lib/firebase';

export function subscribeLatestTelemetry(deviceId: string, callback: (data: Telemetry) => void) {
  const latestRef = ref(database, `telemetry/${deviceId}/latest`);
  return onValue(latestRef, (snapshot) => {
    if (snapshot.exists()) {
      callback(snapshot.val() as Telemetry);
    }
  });
}
```

**Estructura en DB:**
```
/telemetry/{deviceId}/latest  →  { ...Telemetry }  (actualizado en cada write device)
```

#### Histórica (Query por rango)
```typescript
export async function fetchHistoricalTelemetry(
  deviceId: string,
  startMs: number,
  endMs: number,
  limit = 1000
): Promise<Telemetry[]> {
  const telemetryRef = ref(database, `telemetry/${deviceId}`);
  const queryRef = query(
    telemetryRef,
    orderByKey(),
    startAt(startMs.toString()),
    endAt(endMs.toString()),
    limitToFirst(limit)
  );
  const snapshot = await get(queryRef);
  const data: Telemetry[] = [];
  snapshot.forEach((child) => data.push({ ...child.val(), timestamp: Number(child.key) }));
  return data;
}
```

#### Estado Dispositivo
```typescript
export function subscribeDeviceStatus(deviceId: string, callback: (data: Device) => void) {
  const deviceRef = ref(database, `devices/${deviceId}`);
  return onValue(deviceRef, (snapshot) => {
    if (snapshot.exists()) callback(snapshot.val() as Device);
  });
}
```

---

## 5. Esquemas de Datos (TypeScript Types)

> Generados desde `packages/firebase-types` o definidos manualmente sincronizados con Data Model.

```typescript
// packages/firebase-types/src/telemetry.ts
export interface Telemetry {
  schema_version: 1;
  deviceId: string;
  timestamp: number;           // epoch ms UTC
  solar_voltage: number;       // V
  solar_current: number;       // A (+ generación)
  solar_power: number;         // W
  battery_voltage: number;     // V
  battery_current: number;     // A (+ carga, - descarga)
  battery_soc_est: number;     // % (0-100, 1 decimal)
  rssi: number;                // dBm
  fw_version: string;          // SemVer
  uptime_sec?: number;
  free_heap_bytes?: number;
  wake_reason?: 'timer' | 'external' | 'touch' | 'ulp';
}

export interface Device {
  deviceId: string;
  name: string;
  location?: string;
  fw_version: string;
  hw_version?: string;
  status: 'online' | 'offline' | 'degraded' | 'maintenance';
  last_seen: number;
  battery_health?: 'good' | 'fair' | 'poor' | 'critical';
  config: DeviceConfig;
  created_at: number;
  updated_at: number;
}

export interface DeviceConfig {
  measurement_interval_sec: number;
  deep_sleep_duration_sec: number;
  solar_sensor_addr: string;
  battery_sensor_addr: string;
  soc_estimation_method: 'hybrid_ocv_coulomb';
  soc_calibration: {
    voltage_full: number;
    voltage_empty: number;
    capacity_mah: number;
  };
  thresholds: {
    battery_voltage_low: number;
    battery_voltage_critical: number;
    solar_power_min: number;
    rssi_warning: number;
  };
  wifi: {
    ssid: string;
    reconnect_max_attempts: number;
    reconnect_backoff_base_sec: number;
  };
}

// packages/firebase-types/src/weather.ts
export interface WeatherCache {
  locationKey: string;     // "lat,lon" 2 decimales
  data: WeatherData;
  fetched_at: number;
  expires_at: number;
}

export interface WeatherData {
  current: {
    temp: number;
    humidity: number;
    clouds: number;
    uvi: number;
    wind_speed: number;
    weather: Array<{ main: string; description: string; icon: string }>;
  };
  hourly: Array<{ dt: number; temp: number; clouds: number; uvi: number }>;
  daily: Array<{ dt: number; temp: { min: number; max: number }; clouds: number; uvi: number }>;
}
```

---

## 6. Weather API (OpenWeatherMap One Call 3.0)

### 6.1 Endpoint
```
GET https://api.openweathermap.org/data/3.0/onecall
  ?lat={lat}&lon={lon}
  &exclude=minutely,alerts
  &units=metric
  &lang=es
  &appid={API_KEY}
```

### 6.2 Parámetros
| Parámetro | Requerido | Descripción |
|-----------|-----------|-------------|
| `lat`, `lon` | Sí | Coordenadas ubicación instalación (2 decimales) |
| `exclude` | No | `minutely,alerts` para reducir payload |
| `units` | Sí | `metric` (Celsius, m/s, mm) |
| `lang` | No | `es` para descripciones en español |
| `appid` | Sí | API Key (env variable `VITE_WEATHER_API_KEY`) |

### 6.3 Response (subconjunto)
```json
{
  "lat": -34.60,
  "lon": -58.38,
  "timezone": "America/Argentina/Buenos_Aires",
  "current": {
    "dt": 1700000000,
    "temp": 22.5,
    "feels_like": 22.1,
    "humidity": 65,
    "clouds": 20,
    "uvi": 5.2,
    "wind_speed": 3.1,
    "weather": [{ "id": 800, "main": "Clear", "description": "cielo despejado", "icon": "01d" }]
  },
  "hourly": [ { "dt": 1700003600, "temp": 21.8, "clouds": 15, "uvi": 4.8 } ],
  "daily": [ { "dt": 1700000000, "temp": { "min": 15.2, "max": 26.8 }, "clouds": 10, "uvi": 6.1 } ]
}
```

### 6.4 Cache Strategy (Frontend)
- **TTL:** 10 minutos (600 segundos)
- **Storage:** Firebase `/weather_cache/{locationKey}` + React Query cache
- **Fallback:** Si API falla → leer cache aunque expirado → mostrar `stale` badge

---

## 7. Formato de Respuesta Estándar (Firebase REST)

### 7.1 Éxito
| Operación | Response |
|-----------|----------|
| `POST` (write) | `{ "name": "{key}" }` |
| `GET` (read) | JSON object / array |
| `PUT` (update) | JSON object actualizado |
| `DELETE` | `null` (200 OK con body null) |

### 7.2 Error (Firebase REST)
```json
{
  "error": "Permission denied",
  "code": 403
}
```

| Código HTTP | Firebase Error | Significado |
|-------------|----------------|-------------|
| 400 | Invalid data | Payload malformado / validación falló |
| 401 | Unauthorized | Token inválido / expirado |
| 403 | Permission denied | Rules rechazaron operación |
| 404 | Not found | Path no existe |
| 429 | Rate limit | Demasiadas requests (Firebase limits) |
| 500 | Internal error | Error servidor Firebase |

---

## 8. Versionado y Ciclo de Vida

| Cambio | Estrategia |
|--------|------------|
| **Campo nuevo opcional** | Agregar en firmware + frontend con default. `schema_version` no cambia. |
| **Campo renombrado** | Mantener ambos 1 release. Frontend: `new ?? legacy`. `schema_version` +1. |
| **Estructura breaking** | Nueva ruta `/telemetry_v2/` + Cloud Function backfill + switch atómico. |
| **Weather API v4** | Nueva ruta `/weather_cache_v2/` + migración gradual. |

---

## 9. Rate Limiting y Cuotas (Firebase Spark Plan)

| Límite | Valor | Acción al exceder |
|--------|-------|-------------------|
| Conexiones simultáneas | 100 | Rechaza nuevas (device: backoff; frontend: retry) |
| Escrituras/seg | ~1000 (burst) | 429 → exponential backoff |
| Lecturas/seg | ~10000 | 429 → cache local |
| Almacenamiento | 1 GB | Limpieza automática 30d |
| Descarga/mes | 10 GB | Monitoring + alerta 80% |

---

## 10. Testing de Contratos

| Tipo | Herramienta | Qué valida |
|------|-------------|------------|
| **Firebase Emulator** | `firebase emulators:start` | Rules, triggers, functions localmente |
| **Integration Tests** | Vitest + Firebase Emulator | Device write → Frontend read E2E |
| **Schema Validation** | Zod schemas en firmware/frontend | Payload shape en runtime |
| **Type Safety** | TypeScript + `packages/firebase-types` | Compile-time contract |

---

## 11. Checklist de Revisión de Cambios API

- [ ] `openapi.yaml` actualizado (Weather API)
- [ ] `packages/firebase-types` actualizado (Telemetry, Device, Weather)
- [ ] Firebase Rules actualizadas y testeadas en Emulator
- [ ] Firmware actualizado para nuevo `schema_version`
- [ ] Frontend actualizado (hooks, types, componentes)
- [ ] Tests de integración pasan (Emulator)
- [ ] Changelog actualizado (`09-changelog.md`)
- [ ] Documentación relacionada: Data Model, Architecture, SRS

---

## Historial de versiones

| Versión | Fecha | Autor | Cambios |
|---------|-------|-------|---------|
| 0.1 | 2024-11-20 | Matías Biem | Plantilla base |
| 1.0 | 2024-11-20 | Matías Biem | API Spec Solar Monitoring v1.0: Firebase Realtime DB patterns, Device/Frontend interfaces, Weather API, Types |