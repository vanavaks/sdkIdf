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
#include <wifi_connect.h>
#include "main.h"
#include "tlg.h"

const char* tagMain = "Main";

extern "C" void app_main(void) {

	ESP_LOGI(tagMain, "Nvs_flash_init starting from main");

	ESP_LOGI(tagMain, "Tags initialization");
	Tag::begin();		//initialize_nvs();



	ESP_ERROR_CHECK(esp_netif_init());

	ESP_LOGI(tagMain, "Event loop initialization");
	ESP_ERROR_CHECK(esp_event_loop_create_default());

	ESP_LOGI(tagMain, "Starting telegram task");
	xTaskCreate(&tlg_task, "tlg_task", 20000, NULL, 5, NULL);

	ESP_LOGI(tagMain, "Coneecting to wifi");
	ESP_ERROR_CHECK(wifi_begin());

	//-----------register_OtaUpdate();

	//ESP_LOGI(tagMain, "Connecting to SNTP server");
	//sntp_start();

	//ESP_LOGI(tagMain, "Starting web server");
	//http_startWebServer();



	ESP_LOGI(tagMain, "Starting console task");
	//xTaskCreate(&cmd_task, "cmd_task", 8000, NULL, 5, NULL);
}


#if 0
//extern uint8_t sntp_connected;


Tag* tag2;
Tag*	 tag3;
//#define Pstr(_name, _cat, _savebl, _def) {.name = (_name), .category = (_cat), .type = TAG_STR, .saveble = (_savebl), .val = { .asstr = (char*)(_def) }}


	const char* C_ssid= "test";
	const char* C_cat = "Net";
	//const char*
	struct st{
		union{
			uint8_t u8;
			uint32_t u32;
		};
	};
	const tagProp_t tpt2 = Pstr("hren","Main",SAVEBLE,"jaba");
	tag2 = new Tag(&tpt2);


	const st stt = {.u8 = 25};
	ESP_LOGI("main", "starting console task %d", stt.u8);

	//const val_t vt = {.asstr = "mode"};
	//const tagProp_t tpt = {"vanavaks","WIFI",TAG_STR,SAVEBLE,vt};
#if 0
	const tagProp_t tpt1 = {.KeyName = C_ssid, .category = C_cat, .type = TAG_STR, .saveble = SAVEBLE, .val = {.asstr = (char *)"asdf"}};

	 // = new TagNVS(tpt2);
	 tag3 = new Tag(&tpt1);

	//const tagProp_t tagI = {.name = "IntTag", .category = C_cat, .type = TAG_UI32, .saveble = SAVEBLE, .val = {.asi32 = 6513}};
	//Tag tagInt(&tagI);

	Tag::printAll();

	tag2->set("Tag2");
	tag3->set("Tag3");
	//tagInt.set(564835);

	Tag::printAll();
	char* rv;
	uint32_t int32;

	rv = tag2->getStr();
	//int32 = tagInt.getUI32();
	ESP_LOGI("main", "tag2=%s", rv);
	//ESP_LOGI("main", "tagInt=%d", int32);
#endif
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


#endif
