#ifndef FIRMWARE_HAL_ESP_WIFI_H
#define FIRMWARE_HAL_ESP_WIFI_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t fw_wifi_init(void);
esp_err_t fw_wifi_connect(const char *ssid, const char *password);

#ifdef __cplusplus
}
#endif

#endif // FIRMWARE_HAL_ESP_WIFI_H