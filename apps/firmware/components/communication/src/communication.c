#include "communication.h"
#include "hal/esp_wifi.h"
#include "hal/esp_http.h"
#include "esp_log.h"
#include "cJSON.h"

static const char *TAG = "COMMUNICATION";

static char firebase_url[256];
static bool wifi_connected = false;

esp_err_t communication_init(void)
{
    esp_err_t ret;

    // Initialize Wi-Fi
    ret = esp_wifi_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize Wi-Fi");
        return ret;
    }

    // Connect to Wi-Fi
    ret = esp_wifi_connect();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to connect to Wi-Fi");
        return ret;
    }

    // Initialize HTTP client
    ret = esp_http_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize HTTP client");
        return ret;
    }

    // Build Firebase URL
    snprintf(firebase_url, sizeof(firebase_url),
             "https://solar-mon-prod-default-rtdb.firebaseio.com/telemetry");

    ESP_LOGI(TAG, "Communication module initialized");
    return ESP_OK;
}

esp_err_t communication_send_telemetry(const telemetry_packet_t *packet)
{
    if (!wifi_connected) {
        ESP_LOGW(TAG, "Wi-Fi not connected, buffering telemetry locally");
        return ESP_ERR_INVALID_STATE;
    }

    // Create JSON payload
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        ESP_LOGE(TAG, "Failed to create JSON object");
        return ESP_ERR_NO_MEM;
    }

    cJSON_AddNumberToObject(root, "schema_version", 1);
    cJSON_AddStringToObject(root, "deviceId", "solar-mon-prod");
    cJSON_AddNumberToObject(root, "timestamp", packet->timestamp);
    cJSON_AddNumberToObject(root, "solar_voltage", packet->solar_voltage);
    cJSON_AddNumberToObject(root, "solar_current", packet->solar_current);
    cJSON_AddNumberToObject(root, "solar_power", packet->solar_power);
    cJSON_AddNumberToObject(root, "battery_voltage", packet->battery_voltage);
    cJSON_AddNumberToObject(root, "battery_current", packet->battery_current);
    cJSON_AddNumberToObject(root, "battery_soc_est", packet->battery_soc_est);
    cJSON_AddNumberToObject(root, "rssi", packet->rssi);
    cJSON_AddStringToObject(root, "fw_version", packet->fw_version);
    cJSON_AddNumberToObject(root, "uptime_sec", packet->uptime_sec);
    cJSON_AddNumberToObject(root, "free_heap_bytes", packet->free_heap_bytes);
    cJSON_AddStringToObject(root, "wake_reason", packet->wake_reason);

    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    if (!json_str) {
        ESP_LOGE(TAG, "Failed to serialize JSON");
        return ESP_ERR_NO_MEM;
    }

    // Build URL with timestamp
    char url[512];
    snprintf(url, sizeof(url), "%s/%lu.json", firebase_url, packet->timestamp);

    // Send HTTP POST
    esp_err_t ret = esp_http_post(url, json_str);
    cJSON_free(json_str);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to send telemetry to Firebase");
        return ret;
    }

    ESP_LOGI(TAG, "Telemetry sent successfully");
    return ESP_OK;
}
