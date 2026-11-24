#ifndef FIRMWARE_HAL_ESP_SLEEP_H
#define FIRMWARE_HAL_ESP_SLEEP_H

#include "esp_err.h"
#include <stdint.h>
#include "esp_sleep.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t fw_sleep_enable_timer_wakeup(uint64_t duration_us);
void fw_deep_sleep_start(void);

#ifdef __cplusplus
}
#endif

#endif // FIRMWARE_HAL_ESP_SLEEP_H