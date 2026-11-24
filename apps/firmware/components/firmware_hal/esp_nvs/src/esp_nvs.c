#include "fw_nvs.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"

static const char *TAG = "FW_NVS";

esp_err_t fw_nvs_init(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    return ret;
}

esp_err_t fw_nvs_set_str(const char *key, const char *value)
{
    nvs_handle_t handle;
    esp_err_t ret = nvs_open("solar_config", NVS_READWRITE, &handle);
    if (ret != ESP_OK) return ret;

    ret = nvs_set_str(handle, key, value);
    if (ret == ESP_OK) ret = nvs_commit(handle);

    nvs_close(handle);
    return ret;
}

esp_err_t fw_nvs_get_str(const char *key, char *value, size_t *max_len)
{
    nvs_handle_t handle;
    esp_err_t ret = nvs_open("solar_config", NVS_READONLY, &handle);
    if (ret != ESP_OK) return ret;

    ret = nvs_get_str(handle, key, value, max_len);

    nvs_close(handle);
    return ret;
}