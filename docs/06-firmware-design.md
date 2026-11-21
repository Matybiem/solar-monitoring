# Firmware Design / Diseño de Firmware (IoT / Embedded)

**Estado:** v1.0 — Solar Monitoring (ESP32-C3 Super Mini)

---

## Propósito del documento

Define la arquitectura, interfaces, protocolos y restricciones del **software embebido** que corre en el ESP32-C3 Super Mini. Incluye: arranque, adquisición sensores, comunicación, gestión de energía, OTA (futuro), seguridad y testing. Arquitectura **modular por capas** para mantenibilidad.

---

## 1. Hardware objetivo (Target Hardware)

| Componente | Especificación | Notas |
|------------|----------------|-------|
| **MCU** | ESP32-C3 Super Mini (ESP32-C3FN4) | RISC-V 32-bit 160MHz, 400KB SRAM, 4MB Flash, Wi-Fi 2.4GHz + BLE 5 |
| **Flash** | 4 MB (W25Q32) | Particiones: `app` (1.3MB), `ota_0` (1.3MB), `nvs` (24KB), `otadata` (8KB), `spiffs` (512KB) |
| **RAM** | 400 KB SRAM | ~300KB usable para app (resto sistema) |
| **Conectividad** | Wi-Fi 2.4GHz (802.11 b/g/n) | Antena PCB integrada. BLE 5 para provisioning futuro. |
| **Sensores** | 2× INA219 (TI) | I2C addr 0x40 (solar), 0x41 (batería). Shunt 0.1Ω 1%. Rango ±3.2A / ±26V |
| **Alimentación** | Panel solar → Charge Controller (TP4056/CN3791) → 18650 ×2 en paralelo → LDO 3.3V (AMS1117) → ESP32 | **Protección batería:** BMS 2S o protection board (overcharge, over-discharge, overcurrent, short) |
| **Seguridad HW** | Flash Encryption (AES-XTS) + Secure Boot (RSA-3072) | eFuses quemados en producción. JTAG/UART debug deshabilitado. |

---

## 2. Entorno de desarrollo

| Herramienta | Versión | Propósito |
|-------------|---------|-----------|
| **Framework** | ESP-IDF v5.2+ | Oficial Espressif, CMake, components, OTA nativo |
| **Lenguaje** | C (C11) + C++17 (opcional drivers) | Compatible ESP-IDF. Rust `esp-rs` evaluado para v2. |
| **Build** | CMake + Ninja | `idf.py build/flash/monitor` |
| **Debug** | ESP-Prog (JTAG) + OpenOCD + GDB | Breakpoints, watchpoints, RTOS awareness |
| **Test en host** | Unity + CMock (C) / GoogleTest (C++) | Unit tests off-target (compilados nativo Linux/macOS) |
| **CI** | GitHub Actions + `esp-idf-action` | Build + unit tests + artifact `.bin` firmado |
| **Static Analysis** | `cppcheck` + `clang-tidy` + `esp-idf` check | En CI pipeline |

---

## 3. Arquitectura de firmware (Modular por Capas)

```
┌─────────────────────────────────────────────────────────────┐
│                    APPLICATION LAYER                        │  ← main.c: state machine, task creation, watchdog
├─────────────────────────────────────────────────────────────┤
│  ┌─────────────┐ ┌─────────────┐ ┌─────────────┐ ┌────────┐ │
│  │ Acquisition │ │Communication│ │   Power     │ │Diagnost.│ │  ← Services (business logic, reutilizables)
│  │  Service    │ │  Service    │ │  Service    │ │ Service│ │
│  └──────┬──────┘ └──────┬──────┘ └──────┬──────┘ └────┬───┘ │
├─────────┼──────────────┼──────────────┼────────────┼──────┤
│  ┌──────▼──────┐ ┌─────▼─────┐ ┌───────▼──────┐ ┌─────▼───┐ │
│  │   Drivers   │ │   HAL     │ │   Config    │ │  Time   │ │  ← Drivers/HAL/Utils (portable, testeable)
│  │  (INA219,   │ │ (Wi-Fi,   │ │   (NVS)     │ │ (SNTP)  │ │
│  │   I2C, GPIO)│ │  Sleep,   │ │             │ │         │ │
│  │             │ │  NVS)     │ │             │ │         │ │
│  └─────────────┘ └───────────┘ └─────────────┘ └─────────┘ │
├─────────────────────────────────────────────────────────────┤
│                    FREERTOS KERNEL                          │  ← Tasks, Queues, Semaphores, Timers
├─────────────────────────────────────────────────────────────┤
│                    HARDWARE (ESP32-C3 + INA219x2)           │
└─────────────────────────────────────────────────────────────┘
```

### 3.1 Estructura de carpetas (Modular)
```
apps/firmware/
├── CMakeLists.txt
├── main/
│   ├── CMakeLists.txt
│   ├── main.c                 # app_main(), init, task creation
│   └── system_state.c/.h      # State machine: INIT → MEASURE → SEND → SLEEP
├── components/                # Componentes ESP-IDF (reutilizables)
│   ├── acquisition/           # Servicio adquisición sensores
│   │   ├── include/acquisition.h
│   │   ├── src/acquisition.c
│   │   ├── src/solar_sensor.c      # INA219 #1 lógica
│   │   ├── src/battery_sensor.c    # INA219 #2 lógica + SoC estimation
│   │   └── test/                   # Unity tests (mock I2C)
│   ├── communication/         # Servicio comunicación Firebase
│   │   ├── include/communication.h
│   │   ├── src/communication.c
│   │   ├── src/firebase_client.c   # HTTP client + JSON + retry
│   │   ├── src/buffer.c            # Circular buffer (10 lecturas)
│   │   └── test/
│   ├── power/                 # Servicio gestión energía
│   │   ├── include/power.h
│   │   ├── src/power.c
│   │   ├── src/deep_sleep.c        # Deep sleep config, wake reasons
│   │   ├── src/battery_monitor.c   # Voltaje batería → health
│   │   └── test/
│   ├── diagnostics/           # Servicio diagnóstico/logging
│   │   ├── include/diagnostics.h
│   │   ├── src/diagnostics.c
│   │   ├── src/logging.c           # Structured logging (JSON)
│   │   ├── src/health_check.c      # Self-test sensores, Wi-Fi, heap
│   │   └── test/
│   ├── drivers/               # Drivers hardware (portables)
│   │   ├── ina219/             # Driver INA219 genérico
│   │   │   ├── include/ina219.h
│   │   │   ├── src/ina219.c
│   │   │   └── test/             # Mock I2C, test unitario
│   │   ├── i2c_bus/            # Bus I2C compartido + mutex
│   │   └── gpio/               # LED status, botones
│   ├── hal/                   # Hardware Abstraction Layer (ESP-IDF specific)
│   │   ├── esp_wifi/           # Wi-Fi init, connect, event loop
│   │   ├── esp_sleep/          # Deep/light sleep, wake sources
│   │   ├── esp_nvs/            # NVS encrypted: config, creds, calibration
│   │   └── esp_http/           # HTTP client wrapper (mbedTLS)
│   ├── config/                # Configuración persistente (NVS)
│   │   ├── include/config.h
│   │   ├── src/config.c
│   │   └── src/defaults.c        # Valores por defecto
│   └── time/                  # Sincronización tiempo
│       ├── include/time_sync.h
│       ├── src/sntp_client.c     # SNTP (pool.ntp.org)
│       └── src/rtc.c             # RTC backup si SNTP falla
├── test/                      # Integration tests (target)
│   ├── unity_test_runner.c
│   └── test_*.c
└── partitions.csv             # Tabla particiones flash
```

---

## 3.2 Tareas / Hilos (FreeRTOS Tasks)

| Tarea | Prioridad | Stack | Trigger / Periodicidad | Descripción |
|-------|-----------|-------|------------------------|-------------|
| `main_task` / `app_main` | — | 4KB | Boot | Inicialización: NVS, Wi-Fi, SNTP, I2C, sensores. Crea tareas. |
| `acquisition_task` | 5 (normal) | 4KB | Cada 30s (timer) | Lee INA219x2 → calcula potencia/SoC → empuja a `telemetry_queue` |
| `communication_task` | 6 (high) | 8KB | Event: `telemetry_queue` + cada 30s | Wi-Fi connect → SNTP sync → Firebase POST → flush buffer |
| `power_task` | 4 (low) | 2KB | Idle / Event | Monitorea batería, decide deep sleep depth, feed watchdog |
| `diagnostics_task` | 3 (low) | 2KB | Cada 5min | Health check: heap, Wi-Fi RSSI, sensor status, uptime |
| `watchdog_task` | 25 (highest) | 2KB | Cada 10s | Feed HW watchdog (TIMG) + SW watchdog (task monitoring) |

### 3.3 Comunicación inter-tareas
- **`telemetry_queue`** (QueueHandle_t, tamaño 10): `acquisition_task` → `communication_task`. Item: `telemetry_packet_t` (struct completa + metadata).
- **`wifi_event_group`** (EventGroupHandle_t): Bits `WIFI_CONNECTED`, `SNTP_SYNCED`, `FIREBASE_READY`. Sincronización boot y reconexión.
- **`config_mutex`** (SemaphoreHandle_t): Acceso exclusivo a NVS (config, credenciales, calibración).
- **`i2c_mutex`** (SemaphoreHandle_t): Bus I2C compartido entre sensores.

---

## 4. Conectividad y Protocolos

### 4.1 Wi-Fi / Red
| Parámetro | Valor |
|-----------|-------|
| Modo | Station (STA) únicamente |
| Seguridad | WPA2-PSK / WPA3-SAE (configurable en NVS) |
| Provisioning | **Bluetooth LE (futuro)** / **Serial CLI (dev)** / **Hardcoded NVS (MVP)** |
| Reconexión | Exponential backoff: 10s, 30s, 60s, 120s, 300s (max). Máx 5 intentos → deep sleep 5min → retry. |
| Watchdog de red | Ping a `8.8.8.8` cada 60s si conectado. Si 3 fallos → reiniciar Wi-Fi stack. |
| Power Save | `WIFI_PS_MIN_MODEM` (modem sleep automático entre beacons) |

### 4.2 Protocolo Device → Cloud (Firebase)
| Opción | Decisión | Justificación |
|--------|----------|---------------|
| **HTTPS REST (Firebase HTTP API)** | **Elegida** | Simplicidad MVP, sin broker MQTT, Firebase nativo, WebSocket para frontend |
| MQTT 3.1.1 / 5 | Descartada | Requiere broker propio, complejidad adicional |
| CoAP | Descartada | Menos tooling, Firebase no nativo |
| WebSocket directo | Descartada | Firebase REST suficiente para 1 request/30s |

#### Estructura Request (Firebase REST)
```
POST https://{projectId}-default-rtdb.firebaseio.com/telemetry/{deviceId}/{timestamp}.json?auth={ID_TOKEN}
Content-Type: application/json
```

**Payload JSON** (ver [API Specification](05-api-specification.md#41)):
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

### 4.3 Serialización
| Formato | Uso | Librería |
|---------|-----|----------|
| **JSON** | Firebase REST, config NVS, logs | `cJSON` (incluido en ESP-IDF) |
| **Binario (CBOR)** | Futuro: buffer local comprimido | `tinycbor` (opcional) |

---

## 5. Gestión de Energía (Power Management)

### 5.1 Estados de sueño (ESP32-C3)
| Estado | Corriente típica | Wakeup sources | Latencia wake | Uso |
|--------|------------------|----------------|---------------|-----|
| **Active** (RF on) | 80–120 mA | — | — | Medición + envío (30s) |
| **Modem Sleep** | 15–25 mA | Wi-Fi beacon, Timer | < 1 ms | Entre mediciones si Wi-Fi on |
| **Light Sleep** | 0.5–1.5 mA | Timer, GPIO, Wi-Fi | ~1 ms | Si intervalo < 1 min |
| **Deep Sleep** | **10–20 µA** | **Timer (RTC), EXT0/1 (RTC GPIO)** | **~100 ms** | **Por defecto: 5 min** |
| **Hibernation** | < 5 µA | Timer only | ~1 s | Batería crítica (< 3.2V) |

### 5.2 Estrategia Duty-Cycling (MVP)
```
┌─────────────────────────────────────────────────────────────┐
│                    WAKE UP (Timer / RTC)                    │
├─────────────────────────────────────────────────────────────┤
│  1. Boot / Wake reason check                                │
│  2. Inicializar periféricos (I2C, Wi-Fi radio)              │
│  3. acquisition_task: leer INA219x2 (3 mediciones c/u,     │
│     media móvil) → calcular potencia, SoC                   │
│  4. communication_task:                                     │
│     a) Conectar Wi-Fi (reintentos + backoff)               │
│     b) SNTP sync (timeout 10s)                              │
│     c) POST telemetría a Firebase                           │
│     d) Flush buffer local si existe                         │
│  5. diagnostics_task: health check, log structured          │
│  6. power_task: evaluar batería → decidir sleep depth       │
│  7. Configurar RTC timer (300s = 5 min)                     │
│  8. ENTER DEEP SLEEP                                        │
└─────────────────────────────────────────────────────────────┘
```

**Parámetros configurables (NVS):**
- `measurement_interval_sec`: 30 (fijo MVP, configurable v2)
- `deep_sleep_duration_sec`: 300 (5 min), ajustable 60–3600
- `battery_voltage_critical`: 3.0V → hibernation mode
- `battery_voltage_low`: 3.2V → light sleep + intervalo extendido

### 5.3 Solar / Battery Management
- **Carga:** Panel solar → Charge Controller (TP4056/CN3791) → 18650 2P (5200mAh nominal)
- **Protecciones HW (obligatorias):**
  - BMS 2S: Overcharge (4.25V/cell), Over-discharge (2.5V/cell), Overcurrent (3A), Short circuit, Temp
  - Fusible rearmable (PPTC) en línea + diodo Schottky anti-reverso
- **Fuel Gauge (estimación SW):** Hibridación OCV (voltaje reposo) + Coulomb Counting (integración corriente). Ver `acquisition/battery_sensor.c`.

---

## 6. Over-the-Air Updates (OTA) — Futuro (v2)

> **MVP: Sin OTA.** Firmware actualizado via Serial/UART (desarrollo) o JTAG (producción inicial).
> **Plan v2:** OTA segura vía Firebase Storage + HTTPS.

### 6.1 Arquitectura OTA (v2)
```
Firebase Storage (firmware.bin firmado) 
    │
    ▼ HTTPS Download (streaming, chunked)
ESP32 OTA Partition (ota_0) 
    │
    ▼ Verify Signature (ECDSA P-256) + SHA256
Bootloader: Mark ota_0 as pending → Reboot
    │
    ▼ App v2 boots → Health check (30s) → Mark valid / Rollback
```

### 6.2 Particiones Flash (4MB ESP32-C3)
| Partición | Tamaño | Offset | Descripción |
|-----------|--------|--------|-------------|
| `nvs` | 24 KB | 0x9000 | Config, Wi-Fi creds, calibración, keys (encrypted) |
| `otadata` | 8 KB | 0xF000 | OTA metadata (boot selector) |
| `app` (factory) | 1.3 MB | 0x10000 | Firmware actual (running) |
| `ota_0` | 1.3 MB | 0x150000 | Firmware nuevo (staging) |
| `spiffs` | 512 KB | 0x290000 | Filesystem: certs CA, calibración JSON, logs persistentes |

### 6.3 Seguridad OTA
- **Firma:** ECDSA P-256 (private key en HSM / GitHub Actions secret). Public key en bootloader.
- **Verificación:** Antes de escribir en `ota_0` → verify signature + hash.
- **Rollback automático:** Si app no reporta `health_ok` en 60s → bootloader revierte a `app`.
- **Anti-rollback:** eFuse `secure_version` + `app_version` en imagen.

---

## 7. Seguridad Embebida

| Capa | Medida | Implementación |
|------|--------|----------------|
| **Secure Boot** | Bootloader + App firmados (ECDSA P-256) | `CONFIG_SECURE_BOOT_V2_ENABLED=y`, clave en eFuse |
| **Flash Encryption** | AES-XTS 256, clave en eFuse | `CONFIG_SECURE_FLASH_ENC_ENABLED=y` |
| **Device Identity** | MAC address derivado → `deviceId` | `esp_read_mac()` → `solar-mon-{last3bytes}` |
| **TLS** | mTLS con Firebase (CA pública) | `mbedTLS` + certificado CA Firebase embebido |
| **Secrets Storage** | NVS Encrypted Partition | `nvs_flash_init_partition("nvs_encrypted")` |
| **JTAG / UART Debug** | Deshabilitado en producción | `CONFIG_ESP_DEBUG_OCDAWARE=n`, eFuse `DISABLE_JTAG` |
| **Rollback Protection** | Anti-rollback eFuse | `CONFIG_BOOTLOADER_APP_ROLLBACK_PROTECTION=y` |

---

## 8. Observabilidad y Diagnóstico

### 8.1 Structured Logging (JSON via UART / Buffer)
```json
{
  "ts": 1700000000000,
  "level": "INFO",
  "module": "acquisition",
  "event": "measurement_complete",
  "data": {
    "solar_v": 18.45,
    "solar_i": 2.31,
    "bat_v": 3.87,
    "bat_i": 1.15,
    "soc_est": 87.5
  },
  "fw_version": "v1.2.3",
  "uptime_sec": 3600,
  "free_heap": 185000
}
```
**Niveles:** `ERROR`, `WARN`, `INFO`, `DEBUG` (compile-time `CONFIG_LOG_DEFAULT_LEVEL`).

### 8.2 Métricas (Push a Firebase / Local)
| Métrica | Frecuencia | Destino |
|---------|------------|---------|
| `free_heap_bytes` | Cada ciclo | Telemetría + Log |
| `wifi_rssi_dbm` | Cada ciclo | Telemetría |
| `measurement_duration_ms` | Cada ciclo | Log (diagnóstico) |
| `firebase_latency_ms` | Cada envío | Log |
| `deep_sleep_duration_sec` | Cada wake | Log |
| `wake_reason` | Cada wake | Telemetría + Log |

### 8.3 Health Checks (Self-test)
```c
// Ejecutado en diagnostics_task cada 5min
typedef struct {
  bool i2c_bus_ok;
  bool ina219_solar_ok;
  bool ina219_battery_ok;
  bool wifi_connected;
  bool sntp_synced;
  bool firebase_reachable;
  uint32_t free_heap_bytes;
  uint32_t min_free_heap_bytes;
  int8_t rssi_dbm;
} health_status_t;
```
**Umbrales alerta:** heap < 20KB, RSSI < -85 dBm, sensor error > 3 consecutivos.

### 8.4 Crash Dump / Core Dump
- **Config:** `CONFIG_ESP_COREDUMP_ENABLE_TO_FLASH=y`, partición `coredump` 64KB.
- **Análisis:** `idf.py coredump-info` + GDB. Auto-upload en próximo boot (v2).

---

## 9. Testing Strategy

| Nivel | Herramienta | Cobertura objetivo | Qué testea |
|-------|-------------|-------------------|------------|
| **Unit (Host/Linux)** | Unity + CMock | > 80% lógica pura | `acquisition/`, `power/`, `config/`, `time/`, `drivers/ina219/` (mock I2C) |
| **Unit (Target)** | Unity en ESP32 | Drivers HW reales | `drivers/ina219/` con I2C real, `hal/esp_nvs/` |
| **Integration (Target)** | Unity + custom test app | Flujos críticos | `acquisition → communication → buffer → deep sleep` cycle |
| **Hardware-in-Loop (HIL)** | Custom fixture + PC host | E2E: sensores → Firebase → Dashboard | Sensor simulator (INA219 emulator) + Wi-Fi attenuator + Firebase Emulator |
| **OTA Testing** | CI: build → sign → flash device test → verify rollback | Cada release candidate | Requiere HW real en CI (self-hosted runner) |
| **Stress / Soak** | 72h+ duty-cycling, reconexiones, buffer overflow | Pre-release | Power cycling, Wi-Fi jammer, batería variable |

---

## 10. Configuración y Calibración (NVS Encrypted)

| Parámetro | Namespace NVS | Clave | Modificación |
|-----------|---------------|-------|--------------|
| Wi-Fi SSID | `wifi` | `ssid` | Provisioning BLE / Serial CLI |
| Wi-Fi Password | `wifi` | `pass` | Provisioning BLE / Serial CLI |
| Firebase Project ID | `firebase` | `project_id` | Provisioning / OTA |
| Device ID | `device` | `device_id` | Factory (MAC derivado) |
| Calibración INA219 Solar | `calib` | `solar_shunt_mv`, `solar_cal` | Serial CLI / OTA |
| Calibración INA219 Batería | `calib` | `bat_shunt_mv`, `bat_cal` | Serial CLI / OTA |
| SoC Calibración | `calib` | `v_full`, `v_empty`, `cap_mah` | Serial CLI (procedimiento calibración) |
| Intervalos | `config` | `measure_sec`, `sleep_sec` | OTA / Remote config (v2) |
| Umbrales | `config` | `bat_low`, `bat_crit`, `rssi_warn` | OTA / Remote config (v2) |

---

## 11. Checklist de Release (Definition of Done)

- [ ] Build reproducible (`idf.py build` en Docker `espressif/idf:v5.2`)
- [ ] Unit tests pass (Host + Target) en CI
- [ ] Integration tests pass en HW real (HIL)
- [ ] Flash encryption + Secure Boot habilitados (production build `idf.py -D CONFIG_SECURE_BOOT=y -D CONFIG_SECURE_FLASH_ENC=y build`)
- [ ] JTAG/UART debug deshabilitado (eFuse quemados)
- [ ] Binario firmado (ECDSA P-256) + checksums SHA256 publicados en GitHub Release
- [ ] `partitions.csv` validado (tamaños, offsets)
- [ ] Release notes + changelog + migration guide (si breaking)
- [ ] Rollback plan documentado (reflash factory via UART)

---

## Historial de versiones

| Versión | Fecha | Autor | Cambios |
|---------|-------|-------|---------|
| 0.1 | 2024-11-20 | Matías Biem | Plantilla base |
| 1.0 | 2024-11-20 | Matías Biem | Firmware Design Solar Monitoring v1.0: Modular layers, FreeRTOS tasks, Deep Sleep duty-cycling, Firebase REST, NVS encrypted, OTA v2 plan, Security, Testing |