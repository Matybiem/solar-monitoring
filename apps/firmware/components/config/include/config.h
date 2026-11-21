#ifndef CONFIG_H
#define CONFIG_H

#include "nvs_flash.h"
#include "esp_err.h"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t config_init(void);
esp_err_t config_load(system_config_t *config);
esp_err_t config_save(const system_config_t *config);

#ifdef __cplusplus
}
#endif

#endif // CONFIG_H
