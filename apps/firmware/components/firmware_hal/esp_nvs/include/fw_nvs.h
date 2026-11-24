#ifndef FIRMWARE_HAL_FW_NVS_H
#define FIRMWARE_HAL_FW_NVS_H

#include "esp_err.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t fw_nvs_init(void);
esp_err_t fw_nvs_set_str(const char *key, const char *value);
esp_err_t fw_nvs_get_str(const char *key, char *value, size_t *max_len);

#ifdef __cplusplus
}
#endif

#endif // FIRMWARE_HAL_FW_NVS_H