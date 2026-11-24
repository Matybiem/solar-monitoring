#include "diagnostics.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_heap_caps.h"
#include "esp_chip_info.h"
#include "esp_mac.h"

static const char *TAG = "DIAGNOSTICS";

esp_err_t diagnostics_init(void)
{
    ESP_LOGI(TAG, "Diagnostics module initialized");
    return ESP_OK;
}

esp_err_t diagnostics_health_check(void)
{
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    ESP_LOGI(TAG, "=== System Health Check ===");
    ESP_LOGI(TAG, "Free heap: %lu bytes", (unsigned long)esp_get_free_heap_size());
    ESP_LOGI(TAG, "Min free heap: %lu bytes", (unsigned long)heap_caps_get_minimum_free_size(MALLOC_CAP_DEFAULT));
    ESP_LOGI(TAG, "Chip model: %d", (int)chip_info.model);
    ESP_LOGI(TAG, "Chip features: 0x%08x", (unsigned int)chip_info.features);
    ESP_LOGI(TAG, "Chip revision: %d", (int)chip_info.revision);
    ESP_LOGI(TAG, "Chip cores: %d", (int)chip_info.cores);
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    ESP_LOGI(TAG, "MAC address: " MACSTR, MAC2STR(mac));
    ESP_LOGI(TAG, "==========================");
    return ESP_OK;
}