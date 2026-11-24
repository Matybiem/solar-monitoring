#ifndef ACQUISITION_H
#define ACQUISITION_H

#include "telemetry_types.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t acquisition_init(void);
esp_err_t acquisition_read_all(telemetry_packet_t *packet);
esp_err_t acquisition_read_solar(float *voltage, float *current, float *power);
esp_err_t acquisition_read_battery(float *voltage, float *current, float *soc_est);

#ifdef __cplusplus
}
#endif

#endif // ACQUISITION_H