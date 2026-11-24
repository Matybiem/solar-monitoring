# Acquisition component CMakeLists.txt
idf_component_register(SRCS "src/acquisition.c"
                     INCLUDE_DIRS "include"
                     REQUIRES drivers firmware_hal time common_types driver
                     PRIV_REQUIRES esp_timer)