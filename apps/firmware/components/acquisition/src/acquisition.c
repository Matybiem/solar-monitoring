#include "acquisition.h"
#include "ina219.h"
#include "i2c_bus.h"
#include "esp_log.h"
#include <stdbool.h>

static const char *TAG = "ACQUISITION";

static ina219_handle_t solar_sensor;
static ina219_handle_t battery_sensor;
static bool solar_sensor_present = false;
static bool battery_sensor_present = false;

esp_err_t acquisition_init(void)
{
    esp_err_t ret;

    // Initialize I2C bus
    ret = i2c_bus_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize I2C bus");
        return ret;
    }

    // Initialize INA219 solar sensor (addr 0x40) - don't fail if not present
    ret = ina219_init(&solar_sensor, I2C_NUM_0, 0x40);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "Solar INA219 sensor not found at 0x40 (continuing without it)");
        solar_sensor_present = false;
    } else {
        solar_sensor_present = true;
    }

    // Initialize INA219 battery sensor (addr 0x41) - don't fail if not present
    ret = ina219_init(&battery_sensor, I2C_NUM_0, 0x41);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "Battery INA219 sensor not found at 0x41 (continuing without it)");
        battery_sensor_present = false;
    } else {
        battery_sensor_present = true;
    }

    ESP_LOGI(TAG, "Acquisition module initialized (solar: %s, battery: %s)",
             solar_sensor_present ? "present" : "absent",
             battery_sensor_present ? "present" : "absent");
    return ESP_OK;
}

esp_err_t acquisition_read_all(telemetry_packet_t *packet)
{
    esp_err_t ret;

    // Read solar sensor
    if (solar_sensor_present) {
        ret = acquisition_read_solar(&packet->solar_voltage, &packet->solar_current, &packet->solar_power);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to read solar sensor");
            packet->solar_voltage = 0.0f;
            packet->solar_current = 0.0f;
            packet->solar_power = 0.0f;
        }
    } else {
        packet->solar_voltage = 0.0f;
        packet->solar_current = 0.0f;
        packet->solar_power = 0.0f;
    }

    // Read battery sensor
    if (battery_sensor_present) {
        ret = acquisition_read_battery(&packet->battery_voltage, &packet->battery_current, &packet->battery_soc_est);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to read battery sensor");
            packet->battery_voltage = 0.0f;
            packet->battery_current = 0.0f;
            packet->battery_soc_est = 0.0f;
        }
    } else {
        packet->battery_voltage = 0.0f;
        packet->battery_current = 0.0f;
        packet->battery_soc_est = 0.0f;
    }

    return ESP_OK;
}

esp_err_t acquisition_read_solar(float *voltage, float *current, float *power)
{
    if (!solar_sensor_present) {
        *voltage = 0.0f;
        *current = 0.0f;
        *power = 0.0f;
        return ESP_ERR_INVALID_ARG;
    }
    return ina219_read_all(&solar_sensor, voltage, current, power);
}

esp_err_t acquisition_read_battery(float *voltage, float *current, float *soc_est)
{
    if (!battery_sensor_present) {
        *voltage = 0.0f;
        *current = 0.0f;
        *soc_est = 0.0f;
        return ESP_ERR_INVALID_ARG;
    }
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
