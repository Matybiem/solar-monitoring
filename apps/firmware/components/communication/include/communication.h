#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t communication_init(void);
esp_err_t communication_send_telemetry(const telemetry_packet_t *packet);

#ifdef __cplusplus
}
#endif

#endif // COMMUNICATION_H
