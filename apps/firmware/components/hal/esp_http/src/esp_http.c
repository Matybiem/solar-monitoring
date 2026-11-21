#include "esp_http.h"
#include "esp_http_client.h"
#include "esp_log.h"

static const char *TAG = "ESP_HTTP";

esp_err_t esp_http_init(void)
{
    ESP_LOGI(TAG, "HTTP client initialized");
    return ESP_OK;
}

esp_err_t esp_http_post(const char *url, const char *body)
{
    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_POST,
        .timeout_ms = 10000,
        .buffer_size = 2048
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (!client) {
        ESP_LOGE(TAG, "Failed to initialize HTTP client");
        return ESP_ERR_NO_MEM;
    }

    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, body, strlen(body));

    esp_err_t ret = esp_http_client_perform(client);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "HTTP POST failed: %s", esp_err_to_name(ret));
    } else {
        int status = esp_http_client_get_status_code(client);
        ESP_LOGI(TAG, "HTTP POST response: %d", status);
    }

    esp_http_client_cleanup(client);
    return ret;
}
