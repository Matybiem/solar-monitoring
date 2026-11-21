# Acquisition component CMakeLists.txt
idf_component_register(SRCS "src/acquisition.c"
                     "src/solar_sensor.c"
                     "src/battery_sensor.c"
                     INCLUDE_DIRS "include"
                     REQUIRES drivers/ina219 drivers/i2c_bus hal/esp_nvs time
                     PRIV_REQUIRES esp_timer)
