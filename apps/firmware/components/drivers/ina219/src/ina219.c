#include "ina219.h"
#include "esp_log.h"
#include "driver/i2c.h"

static const char *TAG = "INA219";

// INA219 Registers
#define INA219_REG_CONFIG        0x00
#define INA219_REG_SHUNTVOLTAGE  0x01
#define INA219_REG_BUSVOLTAGE    0x02
#define INA219_REG_POWER         0x03
#define INA219_REG_CURRENT       0x04
#define INA219_REG_CALIBRATION   0x05

esp_err_t ina219_init(ina219_handle_t *handle, i2c_port_t port, uint8_t addr)
{
    handle->i2c_port = port;
    handle->addr = addr;
    handle->shunt_resistor_ohms = 0.1f;
    handle->max_expected_amps = 3.2f;

    esp_err_t ret = ina219_calibrate(handle, 0.1f, 3.2f);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to calibrate INA219 at 0x%02X", addr);
        return ret;
    }

    ESP_LOGI(TAG, "INA219 initialized at 0x%02X", addr);
    return ESP_OK;
}

esp_err_t ina219_calibrate(ina219_handle_t *handle, float shunt_ohms, float max_amps)
{
    handle->shunt_resistor_ohms = shunt_ohms;
    handle->max_expected_amps = max_amps;

    // Calculate LSB values
    float min_lsb = max_amps / 32767.0f;
    handle->current_lsb = (min_lsb * 10.0f) > 0.0001f ? (int)(min_lsb * 100000.0f + 0.5f) / 100000.0f : 0.0001f;
    handle->power_lsb = handle->current_lsb * 20.0f;

    // Calculate calibration value
    uint16_t cal_value = (uint16_t)(0.04096f / (handle->current_lsb * handle->shunt_resistor_ohms));

    // Write calibration register
    uint8_t write_buf[3];
    write_buf[0] = INA219_REG_CALIBRATION;
    write_buf[1] = (cal_value >> 8) & 0xFF;
    write_buf[2] = cal_value & 0xFF;

    esp_err_t ret = i2c_master_write_to_device(handle->i2c_port, handle->addr, write_buf, 3, pdMS_TO_TICKS(100));
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to write calibration register");
        return ret;
    }

    handle->calibration_value = cal_value;
    return ESP_OK;
}

esp_err_t ina219_read_voltage(ina219_handle_t *handle, float *voltage)
{
    uint8_t reg = INA219_REG_BUSVOLTAGE;
    uint8_t data[2];

    esp_err_t ret = i2c_master_write_read_device(handle->i2c_port, handle->addr, &reg, 1, data, 2, pdMS_TO_TICKS(100));
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read bus voltage");
        return ret;
    }

    uint16_t raw = ((uint16_t)data[0] << 8) | data[1];
    raw >>= 3; // Bus voltage register is right-aligned 3 bits
    *voltage = raw * 0.004f; // 4mV per bit

    return ESP_OK;
}

esp_err_t ina219_read_current(ina219_handle_t *handle, float *current)
{
    uint8_t reg = INA219_REG_CURRENT;
    uint8_t data[2];

    esp_err_t ret = i2c_master_write_read_device(handle->i2c_port, handle->addr, &reg, 1, data, 2, pdMS_TO_TICKS(100));
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read current");
        return ret;
    }

    int16_t raw = (int16_t)(((uint16_t)data[0] << 8) | data[1]);
    *current = raw * handle->current_lsb;

    return ESP_OK;
}

esp_err_t ina219_read_all(ina219_handle_t *handle, float *voltage, float *current, float *power)
{
    esp_err_t ret;

    if (voltage) {
        ret = ina219_read_voltage(handle, voltage);
        if (ret != ESP_OK) return ret;
    }

    if (current) {
        ret = ina219_read_current(handle, current);
        if (ret != ESP_OK) return ret;
    }

    if (power && voltage && current) {
        *power = (*voltage) * (*current);
    }

    return ESP_OK;
}
