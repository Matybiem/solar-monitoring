#ifndef FIRMWARE_HAL_FW_HTTP_H
#define FIRMWARE_HAL_FW_HTTP_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t fw_http_init(void);
esp_err_t fw_http_post(const char *url, const char *body);

#ifdef __cplusplus
}
#endif

#endif // FIRMWARE_HAL_FW_HTTP_H