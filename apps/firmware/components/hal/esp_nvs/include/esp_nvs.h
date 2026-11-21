#ifndef ESP_NVS_H
#define ESP_NVS_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t esp_nvs_init(void);
esp_err_t esp_nvs_set_str(const char *key, const char *value);
esp_err_t esp_nvs_get_str(const char *key, char *value, size_t *max_len);

#ifdef __cplusplus
}
#endif

#endif // ESP_NVS_H
