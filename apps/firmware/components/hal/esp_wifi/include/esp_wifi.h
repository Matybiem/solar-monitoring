#ifndef ESP_WIFI_H
#define ESP_WIFI_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t esp_wifi_init(void);
esp_err_t esp_wifi_connect(void);

#ifdef __cplusplus
}
#endif

#endif // ESP_WIFI_H
