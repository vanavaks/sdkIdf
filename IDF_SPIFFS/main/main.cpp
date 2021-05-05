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
#include "driver/gpio.h"
#include "IO_app.cpp"

const char* tagMain = "Main";
void gpio_init();

extern "C" void app_main(void) {

	//ESP_LOGI(tagMain, "Nvs_flash_init starting from main");

	ESP_LOGI(tagMain, "Tags initialization");
	Tag::begin();		//initialize_nvs();



	ESP_ERROR_CHECK(esp_netif_init());

	ESP_LOGI(tagMain, "Event loop initialization");
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	ESP_ERROR_CHECK(esp_event_loop_init(nullptr, nullptr));



	ESP_LOGI(tagMain, "Coneecting to wifi");
	ESP_ERROR_CHECK(wifi_begin());

	//-----------register_OtaUpdate();

	ESP_LOGI(tagMain, "Connecting to SNTP server");
	sntp_start();




	ESP_LOGI(tagMain, "Starting web server");
	http_startWebServer();

	vTaskDelay(2000/portTICK_PERIOD_MS);

	ESP_LOGI(tagMain, "Starting telegram task");

	xTaskCreate(&tlg_task, "tlg_task", 16000, NULL, 6, NULL);
	tlgInit();


	ESP_LOGI(tagMain, "Starting console task");
	xTaskCreate(&cmd_task, "cmd_task", 2000, NULL, 5, NULL);

	//gpio_init();
	IOapp_Init();

}


#define GPIO_OUTPUT_IO_0    GPIO_NUM_2
#define GPIO_OUTPUT_IO_1    GPIO_NUM_16
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_0) | (1ULL<<GPIO_OUTPUT_IO_1))
#define GPIO_INPUT_IO_0     GPIO_NUM_0
#define GPIO_INPUT_IO_1     GPIO_NUM_5
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_IO_0) | (1ULL<<GPIO_INPUT_IO_1))

const char* tagGpio = "GPIO";
static xQueueHandle gpio_evt_queue = NULL;

static void gpio_isr_handler(void *arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void gpio_task_example(void *arg)
{
	gpio_num_t io_num;

    for (;;) {
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            ESP_LOGI(tagGpio, "GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
        }
    }
}

void gpio_init(){
	gpio_config_t io_conf;
	//disable interrupt
	io_conf.intr_type = GPIO_INTR_DISABLE;
	//set as output mode
	io_conf.mode = GPIO_MODE_OUTPUT;
	//bit mask of the pins that you want to set,e.g.GPIO15/16
	io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
	//disable pull-down mode
	io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
	//disable pull-up mode
	io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
	//configure GPIO with the given settings
	gpio_config(&io_conf);

	//interrupt of rising edge
	io_conf.intr_type = GPIO_INTR_POSEDGE;
	//bit mask of the pins, use GPIO4/5 here
	io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
	//set as input mode
	io_conf.mode = GPIO_MODE_INPUT;
	//enable pull-up mode
	io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
	gpio_config(&io_conf);

	//change gpio intrrupt type for one pin
	gpio_set_intr_type(GPIO_INPUT_IO_0, GPIO_INTR_ANYEDGE);

	//create a queue to handle gpio event from isr
	gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
	//start gpio task
	xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 7, NULL);

	//install gpio isr service
	gpio_install_isr_service(0);
	//hook isr handler for specific gpio pin
	gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void *) GPIO_INPUT_IO_0);
	//hook isr handler for specific gpio pin
	gpio_isr_handler_add(GPIO_INPUT_IO_1, gpio_isr_handler, (void *) GPIO_INPUT_IO_1);

	//remove isr handler for gpio number.
	gpio_isr_handler_remove(GPIO_INPUT_IO_0);
	//hook isr handler for specific gpio pin again
	gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void *) GPIO_INPUT_IO_0);

	int cnt = 0;

	    while (1) {
	        ESP_LOGI(tagGpio, "cnt: %d\n", cnt++);
	        vTaskDelay(1000 / portTICK_RATE_MS);
	        gpio_set_level(GPIO_OUTPUT_IO_0, cnt % 2);
	        gpio_set_level(GPIO_OUTPUT_IO_1, cnt % 2);
	    }
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
