/* LwIP SNTP example

 This example code is in the Public Domain (or CC0 licensed, at your option.)

 Unless required by applicable law or agreed to in writing, this
 software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 CONDITIONS OF ANY KIND, either express or implied.
 */

#include <esp_console.h>
#include <esp_err.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <lwip/apps/sntp.h>
#include <sys/_stdint.h>
#include <sys/_timeval.h>
#include <cstdlib>
#include <ctime>

#include "../../../../../../ESP8266_RTOS_SDK_IDF/components/freertos/port/esp8266/include/freertos/portmacro.h"
#include "Tag.h"

static const char *TAG = "sntp_example";

Tag* sntp_server{0};
Tag* sntp_firstUpdPeriod{0};
Tag* sntp_updPeriod{0};
Tag* sntp_time{0};
Tag* sntp_zone{0};
Tag* sntp_isSync{0};

const tagProp_t sntp_prop_server = Pstr("ntpServer","ntp",SAVEBLE,"pool.ntp.org");
const tagProp_t sntp_prop_updPeriod = Pui32("ntpUpdPer","ntp",SAVEBLE,1800000);	//30 мин
const tagProp_t sntp_prop_ifrstUpdPeriod = Pui32("ntpFirstUpdPer","ntp",SAVEBLE,10000);	//10 сек
const tagProp_t sntp_prop_time = Pstr("ntpLastSyncTime","ntp",0,0);
const tagProp_t sntp_prop_timeZone = Pstr("ntpZone","ntp",SAVEBLE,"CST-2");
const tagProp_t sntp_prop_isSync = Pbool("ntpSsSynch","ntp",0,0);

uint8_t sntp_connected;

static void initialize_sntp(void) {
	ESP_LOGI(TAG, "Initializing SNTP");
	sntp_setoperatingmode(SNTP_OPMODE_POLL);
	sntp_setservername(0, sntp_server->getStr());
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
		vTaskDelay(sntp_firstUpdPeriod->getUI32() / portTICK_PERIOD_MS);
		time(&now);
		localtime_r(&now, &timeinfo);
	}
}

void sntp_task(void *arg) {
	time_t now;
	struct tm timeinfo;

	time(&now);
	localtime_r(&now, &timeinfo);
	char strftime_buf[64];

	// Is time set? If not, tm_year will be (1970 - 1900).
	if (timeinfo.tm_year < (2016 - 1900)) {
		ESP_LOGI(TAG,
				"Time is not set yet. Connecting to WiFi and getting time over NTP.");
		obtain_time();
	}

	// Set timezone to Eastern Standard Time and print local time
	// setenv("TZ", "EST5EDT,M3.2.0/2,M11.1.0", 1);
	// tzset();

	// Set timezone to Ukraine Standard Time
	setenv("TZ", sntp_zone->getStr(), 1);
	tzset();

	while (1) {
		// update 'now' variable with current time
		ESP_LOGI(TAG,"Updaiting time.");
		time(&now);
		localtime_r(&now, &timeinfo);

		ESP_LOGI(TAG,"Updaiting time.");
		strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
		if (timeinfo.tm_year < (2016 - 1900)) {
			sntp_connected = 0;
			ESP_LOGE(TAG, "The current date/time error");
		} else {
			if(sntp_connected == 0){
				strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
				ESP_LOGI(TAG, "The current date/time in Ukraine is: %s",strftime_buf);
			}
			sntp_connected = 1;
		}


		sntp_time->set(strftime_buf, sizeof(strftime_buf));
		vTaskDelay(sntp_updPeriod->getUI32() / portTICK_PERIOD_MS);
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
		//Tags definitions
	sntp_server 		= new Tag(&sntp_prop_server);
	sntp_firstUpdPeriod = new Tag(&sntp_prop_ifrstUpdPeriod);
	sntp_updPeriod 		= new Tag(&sntp_prop_updPeriod);
	sntp_time 			= new Tag(&sntp_prop_time);
	sntp_zone 			= new Tag(&sntp_prop_timeZone);
	sntp_isSync 		= new Tag(&sntp_prop_isSync);
		//console commands config
	register_getSNTP();
		//start sntp task
	xTaskCreate(sntp_task, "sntp_example_task", 2048, NULL, 4, NULL);
}
