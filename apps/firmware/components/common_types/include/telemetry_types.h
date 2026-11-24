#ifndef TELEMETRY_TYPES_H
#define TELEMETRY_TYPES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t timestamp;
    float solar_voltage;
    float solar_current;
    float solar_power;
    float battery_voltage;
    float battery_current;
    float battery_soc_est;
    int8_t rssi;
    char fw_version[16];
    uint32_t uptime_sec;
    uint32_t free_heap_bytes;
    char wake_reason[16];
} telemetry_packet_t;

typedef struct {
    uint32_t measurement_interval_sec;
    uint32_t deep_sleep_duration_sec;
    char solar_sensor_addr[8];
    char battery_sensor_addr[8];
    float battery_voltage_low;
    float battery_voltage_critical;
    int8_t rssi_warning;
    char wifi_ssid[32];
    char wifi_password[64];
} system_config_t;

typedef enum {
    SYSTEM_STATE_INIT,
    SYSTEM_STATE_MEASURE,
    SYSTEM_STATE_SEND,
    SYSTEM_STATE_SLEEP,
    SYSTEM_STATE_ERROR
} system_state_t;

#ifdef __cplusplus
}
#endif

#endif // TELEMETRY_TYPES_H