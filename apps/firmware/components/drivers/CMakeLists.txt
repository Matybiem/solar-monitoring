# Drivers component - aggregates I2C bus and INA219 sensor drivers
idf_component_register(SRCS
    "i2c_bus/src/i2c_bus.c"
    "ina219/src/ina219.c"
    INCLUDE_DIRS
    "i2c_bus/include"
    "ina219/include"
    PRIV_REQUIRES driver esp_timer)