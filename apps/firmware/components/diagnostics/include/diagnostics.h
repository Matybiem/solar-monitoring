#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t diagnostics_init(void);
esp_err_t diagnostics_health_check(void);

#ifdef __cplusplus
}
#endif

#endif // DIAGNOSTICS_H
