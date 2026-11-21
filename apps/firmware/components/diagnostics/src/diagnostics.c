#include "diagnostics.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_heap_caps.h"

static const char *TAG = "DIAGNOSTICS";

esp_err_t diagnostics_init(void)
{
    ESP_LOGI(TAG, "Diagnostics module initialized");
    return ESP_OK;
}

esp_err_t diagnostics_health_check(void)
{
    ESP_LOGI(TAG, "=== System Health Check ===");
    ESP_LOGI(TAG, "Free heap: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "Min free heap: %d bytes", heap_caps_get_minimum_free_size(MALLOC_CAP_DEFAULT));
    ESP_LOGI(TAG, "Chip ID: %d", esp_chip_info()->chip_id);
    ESP_LOGI(TAG, "MAC address: " MACSTR, MAC2STR(esp_chip_info()->mac));
    ESP_LOGI(TAG, "==========================");
    return ESP_OK;
}
