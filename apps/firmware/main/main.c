#include "main.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_timer.h"
#include "driver/rtc_io.h"
#include <string.h>
#include "acquisition.h"
#include "communication.h"
#include "power.h"
#include "diagnostics.h"
#include "config.h"
#include "time_sync.h"
#include "wifi_secrets.h"

static const char *TAG = "MAIN";

system_config_t g_config = {
    .measurement_interval_sec = 30,
    .deep_sleep_duration_sec = 300,
    .solar_sensor_addr = "0x40",
    .battery_sensor_addr = "0x41",
    .battery_voltage_low = 3.2f,
    .battery_voltage_critical = 3.0f,
    .rssi_warning = -80,
    .wifi_ssid = WIFI_SSID,
    .wifi_password = WIFI_PASSWORD
};

EventGroupHandle_t g_wifi_event_group;
QueueHandle_t g_telemetry_queue;

#define WIFI_CONNECTED_BIT    BIT0
#define SNTP_SYNCED_BIT       BIT1
#define FIREBASE_READY_BIT    BIT2

void app_main(void)
{
    ESP_LOGI(TAG, "Solar Monitoring Firmware v1.0.0 starting...");
    ESP_LOGI(TAG, "Free heap: %lu bytes", (unsigned long)esp_get_free_heap_size());

    // Initialize NVS and load configuration
    ESP_ERROR_CHECK(config_init());
    ESP_ERROR_CHECK(config_load(&g_config));

    // Initialize event group and queue
    g_wifi_event_group = xEventGroupCreate();
    g_telemetry_queue = xQueueCreate(10, sizeof(telemetry_packet_t));

    // Initialize system components
    ESP_ERROR_CHECK(system_init());

    // Main state machine
    while (1) {
        switch (g_config.measurement_interval_sec > 0 ? SYSTEM_STATE_MEASURE : SYSTEM_STATE_SLEEP) {
            case SYSTEM_STATE_MEASURE:
                ESP_LOGI(TAG, "Starting measurement cycle");
                system_measure();
                vTaskDelay(pdMS_TO_TICKS(g_config.measurement_interval_sec * 1000));
                break;

            case SYSTEM_STATE_SEND:
                // Handled by communication task via queue
                break;

            case SYSTEM_STATE_SLEEP:
                ESP_LOGI(TAG, "Entering deep sleep for %lu seconds", (unsigned long)g_config.deep_sleep_duration_sec);
                system_sleep(g_config.deep_sleep_duration_sec);
                break;

            default:
                ESP_LOGE(TAG, "Unknown system state");
                vTaskDelay(pdMS_TO_TICKS(5000));
                break;
        }
    }
}

esp_err_t system_init(void)
{
    ESP_LOGI(TAG, "Initializing system components...");

    ESP_ERROR_CHECK(acquisition_init());
    ESP_LOGI(TAG, "Acquisition initialized");

    ESP_ERROR_CHECK(communication_init(&g_config));
    ESP_LOGI(TAG, "Communication initialized");

    ESP_ERROR_CHECK(power_init());
    ESP_LOGI(TAG, "Power management initialized");

    ESP_ERROR_CHECK(diagnostics_init());
    ESP_LOGI(TAG, "Diagnostics initialized");

    ESP_LOGI(TAG, "System initialization complete");
    return ESP_OK;
}

esp_err_t system_sleep(uint32_t duration_sec)
{
    return power_enter_deep_sleep(duration_sec);
}

esp_err_t system_measure(void)
{
    telemetry_packet_t packet = {0};

    // Get timestamp
    packet.timestamp = (uint32_t)(esp_timer_get_time() / 1000ULL);

    // Read sensors
    ESP_ERROR_CHECK(acquisition_read_all(&packet));

    // Get system info
    packet.uptime_sec = (uint32_t)(esp_timer_get_time() / 1000000ULL);
    packet.free_heap_bytes = esp_get_free_heap_size();
    strcpy(packet.fw_version, "v1.0.0");
    strcpy(packet.wake_reason, "timer");

    // Send to communication queue
    if (xQueueSend(g_telemetry_queue, &packet, pdMS_TO_TICKS(1000)) != pdTRUE) {
        ESP_LOGE(TAG, "Failed to send telemetry to queue");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Measurement complete: solar=%.2fV %.2fA %.2fW, battery=%.2fV %.2fA SoC=%.1f%%",
             packet.solar_voltage, packet.solar_current, packet.solar_power,
             packet.battery_voltage, packet.battery_current, packet.battery_soc_est);

    return ESP_OK;
}
