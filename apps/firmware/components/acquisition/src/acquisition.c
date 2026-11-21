#include "acquisition.h"
#include "drivers/ina219/ina219.h"
#include "esp_log.h"

static const char *TAG = "ACQUISITION";

static ina219_handle_t solar_sensor;
static ina219_handle_t battery_sensor;

esp_err_t acquisition_init(void)
{
    esp_err_t ret;

    // Initialize I2C bus
    ret = i2c_bus_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize I2C bus");
        return ret;
    }

    // Initialize INA219 solar sensor (addr 0x40)
    ret = ina219_init(&solar_sensor, I2C_NUM_0, 0x40);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize solar INA219 sensor");
        return ret;
    }

    // Initialize INA219 battery sensor (addr 0x41)
    ret = ina219_init(&battery_sensor, I2C_NUM_0, 0x41);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize battery INA219 sensor");
        return ret;
    }

    ESP_LOGI(TAG, "Acquisition module initialized");
    return ESP_OK;
}

esp_err_t acquisition_read_all(telemetry_packet_t *packet)
{
    esp_err_t ret;

    // Read solar sensor
    ret = acquisition_read_solar(&packet->solar_voltage, &packet->solar_current, &packet->solar_power);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read solar sensor");
        packet->solar_voltage = 0.0f;
        packet->solar_current = 0.0f;
        packet->solar_power = 0.0f;
    }

    // Read battery sensor
    ret = acquisition_read_battery(&packet->battery_voltage, &packet->battery_current, &packet->battery_soc_est);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read battery sensor");
        packet->battery_voltage = 0.0f;
        packet->battery_current = 0.0f;
        packet->battery_soc_est = 0.0f;
    }

    return ESP_OK;
}

esp_err_t acquisition_read_solar(float *voltage, float *current, float *power)
{
    return ina219_read_all(&solar_sensor, voltage, current, power);
}

esp_err_t acquisition_read_battery(float *voltage, float *current, float *soc_est)
{
    esp_err_t ret = ina219_read_all(&battery_sensor, voltage, current, NULL);
    if (ret != ESP_OK) return ret;

    // Simple SoC estimation based on voltage (placeholder for hybrid OCV+Coulomb)
    // 18650: 4.2V = 100%, 3.0V = 0%
    if (*voltage >= 4.2f) {
        *soc_est = 100.0f;
    } else if (*voltage <= 3.0f) {
        *soc_est = 0.0f;
    } else {
        *soc_est = ((*voltage - 3.0f) / (4.2f - 3.0f)) * 100.0f;
    }

    return ESP_OK;
}
