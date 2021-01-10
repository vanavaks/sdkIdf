/* SPIFFS filesystem example.
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "main.h"

#include "esp_spiffs.h"
#include "OTA_app.h"
#include "esp_netif.h"
#include "esp_event.h"
//#include "protocol_examples_common.h"

#include "nvs.h"
#include "nvs_flash.h"
#include "cmd.h"
#include "wifi_connect.h"
//#include "temp.h"

static void initialize_nvs()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK( nvs_flash_erase() );
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void app_main(void)
{
	initialize_nvs();
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	ESP_ERROR_CHECK(wifi_connect());

	register_OtaUpdate();

	//xTaskCreate(&ota_example_task, "ota_example_task", 8192, NULL, 5, NULL);
	xTaskCreate(&cmd_task, "cmd_task", 8192, NULL, 5, NULL);

}
