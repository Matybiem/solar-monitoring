#include "power.h"
#include "hal/esp_sleep.h"
#include "esp_log.h"

static const char *TAG = "POWER";

esp_err_t power_init(void)
{
    ESP_LOGI(TAG, "Power management initialized");
    return ESP_OK;
}

esp_err_t power_enter_deep_sleep(uint32_t duration_sec)
{
    ESP_LOGI(TAG, "Entering deep sleep for %d seconds", duration_sec);

    // Configure RTC timer wakeup
    esp_err_t ret = esp_sleep_enable_timer_wakeup(duration_sec * 1000000ULL);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure timer wakeup");
        return ret;
    }

    ESP_LOGI(TAG, "Going to deep sleep now...");
    esp_deep_sleep_start();

    // Should never reach here
    return ESP_OK;
}

float power_get_battery_voltage(void)
{
    // Placeholder - would read from ADC or INA219
    return 3.87f;
}

esp_err_t power_check_battery_health(void)
{
    float voltage = power_get_battery_voltage();

    if (voltage < 3.0f) {
        ESP_LOGW(TAG, "Battery voltage critical: %.2fV", voltage);
        return ESP_ERR_INVALID_STATE;
    } else if (voltage < 3.2f) {
        ESP_LOGW(TAG, "Battery voltage low: %.2fV", voltage);
        return ESP_OK;
    }

    ESP_LOGI(TAG, "Battery voltage normal: %.2fV", voltage);
    return ESP_OK;
}
