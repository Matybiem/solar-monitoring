#include "esp_sleep.h"
#include "esp_sleep.h"
#include "esp_log.h"

static const char *TAG = "ESP_SLEEP";

esp_err_t esp_sleep_enable_timer_wakeup(uint64_t duration_us)
{
    esp_err_t ret = esp_sleep_enable_timer_wakeup(duration_us);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to enable timer wakeup");
        return ret;
    }
    ESP_LOGI(TAG, "Timer wakeup enabled for %llu us", duration_us);
    return ESP_OK;
}

void esp_deep_sleep_start(void)
{
    ESP_LOGI(TAG, "Entering deep sleep...");
    esp_deep_sleep_start();
}
