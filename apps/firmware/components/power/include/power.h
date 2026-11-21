#ifndef POWER_H
#define POWER_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t power_init(void);
esp_err_t power_enter_deep_sleep(uint32_t duration_sec);
float power_get_battery_voltage(void);
esp_err_t power_check_battery_health(void);

#ifdef __cplusplus
}
#endif

#endif // POWER_H
