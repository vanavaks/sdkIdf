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

#include "nvs.h"
#include "nvs_flash.h"
#include "cmd.h"
#include "wifi_connect.h"
#include "sntp_app.h"
#include "nvs_app.h"
//extern uint8_t sntp_connected;



extern "C" void app_main(void) {
	initialize_nvs();
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	ESP_ERROR_CHECK(wifi_connect());

	register_OtaUpdate();
	sntp_start();
	ESP_LOGI("main", "trying to starting console task");
	while (sntp_getStat() == 0) {}
	ESP_LOGI("main", "starting console task");
	xTaskCreate(&cmd_task, "cmd_task", 8192, NULL, 5, NULL);
}
