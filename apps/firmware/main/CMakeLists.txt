# Main component CMakeLists.txt
idf_component_register(SRCS "main.c"
                     INCLUDE_DIRS "." ".."
                     REQUIRES esp_wifi nvs_flash driver
                     PRIV_REQUIRES acquisition communication power diagnostics config time common_types)
