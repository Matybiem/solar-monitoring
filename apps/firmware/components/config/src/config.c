#include "config.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"

static const char *TAG = "CONFIG";
#define CONFIG_NAMESPACE "solar_config"

esp_err_t config_init(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "NVS initialized");
    return ESP_OK;
}

esp_err_t config_load(system_config_t *config)
{
    nvs_handle_t handle;
    esp_err_t ret = nvs_open(CONFIG_NAMESPACE, NVS_READONLY, &handle);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "No existing config found, using defaults");
        return ret;
    }

    size_t len = sizeof(config->wifi_ssid);
    nvs_get_str(handle, "wifi_ssid", config->wifi_ssid, &len);
    nvs_get_u32(handle, "measure_interval", &config->measurement_interval_sec);
    nvs_get_u32(handle, "deep_sleep_sec", &config->deep_sleep_duration_sec);

    nvs_close(handle);
    ESP_LOGI(TAG, "Configuration loaded from NVS");
    return ESP_OK;
}

esp_err_t config_save(const system_config_t *config)
{
    nvs_handle_t handle;
    esp_err_t ret = nvs_open(CONFIG_NAMESPACE, NVS_READWRITE, &handle);
    if (ret != ESP_OK) return ret;

    nvs_set_str(handle, "wifi_ssid", config->wifi_ssid);
    nvs_set_u32(handle, "measure_interval", config->measurement_interval_sec);
    nvs_set_u32(handle, "deep_sleep_sec", config->deep_sleep_duration_sec);

    ret = nvs_commit(handle);
    nvs_close(handle);
    return ret;
}
