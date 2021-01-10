/* LwIP SNTP example

 This example code is in the Public Domain (or CC0 licensed, at your option.)

 Unless required by applicable law or agreed to in writing, this
 software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 CONDITIONS OF ANY KIND, either express or implied.
 */
#include "main.h"

#include "driver/gpio.h"
#include "lwip/apps/sntp.h"
#include "esp_console.h"

static const char *TAG = "sntp_example";

uint8_t sntp_connected;

static void initialize_sntp(void) {
	ESP_LOGI(TAG, "Initializing SNTP");
	sntp_setoperatingmode(SNTP_OPMODE_POLL);
	sntp_setservername(0, "pool.ntp.org");
	sntp_init();
}

static void obtain_time(void) {
	initialize_sntp();

	// wait for time to be set
	time_t now = 0;
	struct tm timeinfo = { 0 };
	int retry = 0;
	const int retry_count = 10;

	while (timeinfo.tm_year < (2016 - 1900) && ++retry < retry_count) {
		ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry,
				retry_count);
		vTaskDelay(2000 / portTICK_PERIOD_MS);
		time(&now);
		localtime_r(&now, &timeinfo);
	}
}

void sntp_example_task(void *arg) {
	time_t now;
	struct tm timeinfo;

	time(&now);
	localtime_r(&now, &timeinfo);

	// Is time set? If not, tm_year will be (1970 - 1900).
	if (timeinfo.tm_year < (2016 - 1900)) {
		ESP_LOGI(TAG,
				"Time is not set yet. Connecting to WiFi and getting time over NTP.");
		obtain_time();
	}

	// Set timezone to Eastern Standard Time and print local time
	// setenv("TZ", "EST5EDT,M3.2.0/2,M11.1.0", 1);
	// tzset();

	// Set timezone to China Standard Time
	setenv("TZ", "CST-8", 1);
	tzset();

	while (1) {
		// update 'now' variable with current time
		time(&now);
		localtime_r(&now, &timeinfo);

		if (timeinfo.tm_year < (2016 - 1900)) {
			sntp_connected = 0;
			ESP_LOGE(TAG, "The current date/time error");
		} else {
			if(sntp_connected == 0){
				char strftime_buf[64];
				strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
				ESP_LOGI(TAG, "The current date/time in Shanghai is: %s",strftime_buf);
			}
			sntp_connected = 1;
		}

		//ESP_LOGI(TAG, "Free heap size: %d\n", esp_get_free_heap_size());
		vTaskDelay(1000 / portTICK_RATE_MS);
	}
}

static int sntp_get(int argc, char **argv) {
	time_t now;
	struct tm timeinfo;
	char strftime_buf[64];

	time(&now);
	localtime_r(&now, &timeinfo);

	strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
	ESP_LOGI(TAG, "SNTP %s connected", (sntp_connected > 0) ? " " : "don't");
	ESP_LOGI(TAG, "The current date/time in Shanghai is: %s", strftime_buf);
	return 0;
}

void register_getSNTP() {
	const esp_console_cmd_t cmd = { .command = "get_sntp", .help =
			"return status and current time", .hint = NULL, .func = &sntp_get };
	ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}

uint8_t sntp_getStat(){
	return sntp_connected;
}



void sntp_start(){
	register_getSNTP();
	xTaskCreate(sntp_example_task, "sntp_example_task", 2048, NULL, 4, NULL);
}
