#ifndef ESP_HTTP_H
#define ESP_HTTP_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t esp_http_init(void);
esp_err_t esp_http_post(const char *url, const char *body);

#ifdef __cplusplus
}
#endif

#endif // ESP_HTTP_H
