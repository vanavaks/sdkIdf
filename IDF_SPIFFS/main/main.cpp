/* SPIFFS filesystem example.
 This example code is in the Public Domain (or CC0 licensed, at your option.)

 Unless required by applicable law or agreed to in writing, this
 software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 CONDITIONS OF ANY KIND, either express or implied.
 */

#include <cmd.h>
#include <esp_event.h>
#include <esp_netif.h>
#include <http.h>
#include <nvs_app.h>
#include <OTA_app.h>
#include <sntp_app.h>
#include <sys/_stdint.h>
#include <Tag.h>
#include <TagNVS.h>
#include <wifi_connect.h>
#include "main.h"

//extern uint8_t sntp_connected;



extern "C" void app_main(void) {
	initialize_nvs();
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	ESP_ERROR_CHECK(wifi_connect());

#define Pstr(_name, _cat, _savebl, _def) {.name = (_name), .category = (_cat), .type = TAG_STR, .saveble = (_savebl), .val = { .asstr = (_def) }}

	const char* C_ssid= "ssid";
	const char* C_cat = "main";

	struct st{
		union{
			uint8_t u8;
			uint32_t u32;
		};
	};

	const st stt = {.u8 = 25};
	ESP_LOGI("main", "starting console task %d", stt.u8);

	const val_t vt = {.asstr = "mode"};
	const tagProp_t tpt = {"vanavaks","WIFI",TAG_STR,SAVEBLE,vt};

	const tagProp_t tpt1 = {.name = C_ssid, .category = C_cat, .type = TAG_STR, .saveble = SAVEBLE, .val = {.asstr = "asdf"}};
	const tagProp_t tpt2 = Pstr("ssid","WIFI",SAVEBLE,"vanavaks");

	TagNVS tag2(&tpt2); // = new TagNVS(tpt2);
	TagNVS tag3(&tpt1);
/*
const tagProp_t param[] = {
		Pstr(C_ssid, C_cat, TAG_STR, true, C_ssid)
};

const tagProp_t param12[] = {
		{.name = C_ssid, .category = C_cat, .type = TAG_STR, .saveble = true, .val = { .asstr = C_ssid}}
};
*/

//const val_t tval = {.asstr = "kjyk"};

//const tagProp_t  param1 = {.name = C_ssid, .category = C_cat, .type = TAG_STR, .saveble = true, .val = {.v {v.asstr = "asd"}}};

//const tagProp_t param1[] = {C_ssid, C_ssid, TAG_STR, true, C_ssid}
	/*const tagProp_t t1 = {
			.name = C_ssid,
			.category = C_cat,
			.type = TAG_STR,
			.saveble = true,
			.val = { .asbool = (def) }
	};
	.minVal = { .asstr = (const char*)"vanavaks"},
			.maxVal = { .asstr = (const char*)"vanavaks"}
	};*/

	/*const tagProp_t t1: name = "ssid", category = "main", {};
				.,
				.,
				.type = TAG_STR,
				.saveble = true,
				.defVal = { .asstr = "vanavaks"},
				.minVal = { .asstr = "vanavaks"},
				.maxVal = { .asstr = "vanavaks"},
	};*/
	//Tag* T1 = new TagNVS(param1);
	//Tag* T2 = new TagNVS(param12);
	//Tag* T3 = new TagNVS(param);
	register_OtaUpdate();
	sntp_start();
	ESP_LOGI("main", "trying to starting console task");
	while (sntp_getStat() == 0) {}
	ESP_LOGI("main", "starting console task");
	http_startWebServer();
	xTaskCreate(&cmd_task, "cmd_task", 8192, NULL, 5, NULL);

}
