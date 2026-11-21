#include "esp_wifi.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

static const char *TAG = "ESP_WIFI";

static bool s_initialized = false;

esp_err_t esp_wifi_init(void)
{
    if (s_initialized) return ESP_OK;

    esp_err_t ret = esp_netif_init();
    if (ret != ESP_OK) return ret;

    ret = esp_event_loop_create_default();
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE) return ret;

    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ret = esp_wifi_init(&cfg);
    if (ret != ESP_OK) return ret;

    s_initialized = true;
    ESP_LOGI(TAG, "Wi-Fi initialized");
    return ESP_OK;
}

esp_err_t esp_wifi_connect(void)
{
    if (!s_initialized) return ESP_ERR_INVALID_STATE;

    // Note: SSID and password would come from NVS config
    // For now, just start Wi-Fi in station mode
    esp_err_t ret = esp_wifi_set_mode(WIFI_MODE_STA);
    if (ret != ESP_OK) return ret;

    ret = esp_wifi_start();
    if (ret != ESP_OK) return ret;

    ESP_LOGI(TAG, "Wi-Fi started in station mode");
    return ESP_OK;
}
