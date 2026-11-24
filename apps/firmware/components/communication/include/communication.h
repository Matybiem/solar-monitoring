#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "telemetry_types.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t communication_init(const system_config_t *config);
esp_err_t communication_send_telemetry(const telemetry_packet_t *packet);

#ifdef __cplusplus
}
#endif

#endif // COMMUNICATION_H