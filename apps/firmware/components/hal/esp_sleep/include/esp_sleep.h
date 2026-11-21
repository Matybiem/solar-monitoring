#ifndef ESP_SLEEP_H
#define ESP_SLEEP_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t esp_sleep_enable_timer_wakeup(uint64_t duration_us);
void esp_deep_sleep_start(void);

#ifdef __cplusplus
}
#endif

#endif // ESP_SLEEP_H
