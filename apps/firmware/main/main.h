#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "driver/i2c.h"

// System states
typedef enum {
    SYSTEM_STATE_INIT,
    SYSTEM_STATE_MEASURE,
    SYSTEM_STATE_SEND,
    SYSTEM_STATE_SLEEP,
    SYSTEM_STATE_ERROR
} system_state_t;

// Telemetry packet structure
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

// Configuration structure
typedef struct {
    uint32_t measurement_interval_sec;
    uint32_t deep_sleep_duration_sec;
    char solar_sensor_addr[8];
    char battery_sensor_addr[8];
    float battery_voltage_low;
    float battery_voltage_critical;
    int8_t rssi_warning;
    char wifi_ssid[32];
} system_config_t;

// External declarations
extern system_config_t g_config;
extern EventGroupHandle_t g_wifi_event_group;
extern QueueHandle_t g_telemetry_queue;
extern const char *TAG;

// Function declarations
void app_main(void);
esp_err_t system_init(void);
esp_err_t system_measure(void);
esp_err_t system_send(const telemetry_packet_t *packet);
esp_err_t system_sleep(uint32_t duration_sec);

#endif // MAIN_H
