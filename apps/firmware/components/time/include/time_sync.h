#ifndef TIME_SYNC_H
#define TIME_SYNC_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t time_sync_init(void);
uint32_t time_get_timestamp_ms(void);

#ifdef __cplusplus
}
#endif

#endif // TIME_SYNC_H
