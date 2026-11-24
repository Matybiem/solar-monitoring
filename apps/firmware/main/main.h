#ifndef MAIN_H
#define MAIN_H

#include "telemetry_types.h"
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "driver/i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

extern system_config_t g_config;
extern EventGroupHandle_t g_wifi_event_group;
extern QueueHandle_t g_telemetry_queue;

void app_main(void);
esp_err_t system_init(void);
esp_err_t system_measure(void);
esp_err_t system_send(const telemetry_packet_t *packet);
esp_err_t system_sleep(uint32_t duration_sec);

#ifdef __cplusplus
}
#endif

#endif // MAIN_H