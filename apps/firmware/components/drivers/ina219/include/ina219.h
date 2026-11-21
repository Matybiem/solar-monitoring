#ifndef INA219_H
#define INA219_H

#include "driver/i2c.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    i2c_port_t i2c_port;
    uint8_t addr;
    float shunt_resistor_ohms;
    float max_expected_amps;
    float current_lsb;
    float power_lsb;
    float calibration_value;
} ina219_handle_t;

esp_err_t ina219_init(ina219_handle_t *handle, i2c_port_t port, uint8_t addr);
esp_err_t ina219_read_voltage(ina219_handle_t *handle, float *voltage);
esp_err_t ina219_read_current(ina219_handle_t *handle, float *current);
esp_err_t ina219_read_all(ina219_handle_t *handle, float *voltage, float *current, float *power);
esp_err_t ina219_calibrate(ina219_handle_t *handle, float shunt_ohms, float max_amps);

#ifdef __cplusplus
}
#endif

#endif // INA219_H
