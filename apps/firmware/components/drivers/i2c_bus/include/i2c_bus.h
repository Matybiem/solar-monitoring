#ifndef I2C_BUS_H
#define I2C_BUS_H

#include "driver/i2c.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t i2c_bus_init(void);

#ifdef __cplusplus
}
#endif

#endif // I2C_BUS_H
