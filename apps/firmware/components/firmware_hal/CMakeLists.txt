# HAL component - aggregates ESP HAL drivers
idf_component_register(SRCS
    "esp_http/src/esp_http.c"
    "esp_nvs/src/esp_nvs.c"
    "esp_sleep/src/esp_sleep.c"
    "esp_wifi/src/esp_wifi.c"
    INCLUDE_DIRS
    "esp_http/include"
    "esp_nvs/include"
    "esp_sleep/include"
    "esp_wifi/include"
    REQUIRES nvs_flash esp_wifi esp_timer esp_http_client esp_netif)