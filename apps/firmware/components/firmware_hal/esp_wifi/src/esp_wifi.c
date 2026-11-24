#include "fw_wifi.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include <string.h>
#include <stdbool.h>

static const char *TAG = "FW_WIFI";
static bool s_initialized = false;

esp_err_t fw_wifi_init(void)
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

esp_err_t fw_wifi_connect(const char *ssid, const char *password)
{
    if (!s_initialized) return ESP_ERR_INVALID_STATE;

    esp_err_t ret = esp_wifi_set_mode(WIFI_MODE_STA);
    if (ret != ESP_OK) return ret;

    esp_wifi_start();

    wifi_config_t wifi_config = {
        .sta = {
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    strncpy((char *)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid));
    strncpy((char *)wifi_config.sta.password, password, sizeof(wifi_config.sta.password));

    ret = esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    if (ret != ESP_OK) return ret;

    ret = esp_wifi_connect();
    if (ret != ESP_OK) return ret;

    ESP_LOGI(TAG, "Wi-Fi connecting to SSID: %s", ssid);
    return ESP_OK;
}