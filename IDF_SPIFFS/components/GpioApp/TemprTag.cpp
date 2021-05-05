 /*
 * TemprTag.cpp
 *
 *  Created on: 21 квіт. 2021 р.
 *      Author: Vanavaks
 */

#include "TemprTag.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <esp_err.h>

gpio_num_t 	TemprTag :: temprPin = GPIO_NUM_0;
TemprTag* 	TemprTag :: arr[TEMPR_TAG_MAX_NUM];
uint8_t 	TemprTag :: arrHead = 0;

TemprTag::TemprTag(ds18x20_addr_t _addr, const tagProp_t* p) : Tag(p){
	//if(p == NULL) {ESP_ERROR_CHECK(ESP_ERR_TAG_NULL_POINTER); return;}
	if(_addr == 0) {ESP_ERROR_CHECK(ESP_ERR_TEMPR_ADR); return;}
	adr = _addr;
}

void TemprTag::init(const gpio_num_t _pin) {
	temprPin = _pin;
	gpio_set_pull_mode(temprPin, GPIO_PULLUP_ONLY);

	xTaskCreate(update, TEMPR_TAG, 256, NULL, 5, NULL);
}

void TemprTag::update(void *pvParameter) {
	esp_err_t res;
	float val;
					/* tempr scaning main cycle */
	while(1){
					/* reading all sensors */
		for(int i=0; i<arrHead; i++){
					/* reading and saving all sensors */
			res = ds18x20_measure_and_read(TemprTag::temprPin, arr[i]->adr, &val);
			if(res != ESP_OK){
				ESP_LOGE(TEMPR_TAG, "Could not read from sensor %08x%08x: %d (%s)", (uint32_t)(arr[i]->adr >> 32), (uint32_t)arr[i]->adr, (uint32_t)res, esp_err_to_name(res));
			}else{
				arr[i]->set(val);
			}
		}
		vTaskDelay(pdMS_TO_TICKS(TEMPRTAG_SCAN_TIME));
	}
}

TemprTag::~TemprTag() {
	// TODO Auto-generated destructor stub
}

void TemprTag::arrAdd() {
	if(arrHead >= TEMPR_TAG_MAX_NUM) {ESP_ERROR_CHECK(ESP_ERR_TEMPR_ARR); return;}
	TemprTag::arr[TemprTag::arrHead] = this;
	arrHead++;
}

TemprTag* TemprTag::arrGet(uint8_t n) {
	if(n >= TEMPR_TAG_MAX_NUM) {ESP_ERROR_CHECK(ESP_ERR_TEMPR_ARR); return NULL;}
	return arr[n];
}

void TemprTag::arrInit() {
	arrHead = 0;
	for(int i=0;i<TEMPR_TAG_MAX_NUM; i++){
		arr[i] = NULL;
	}
}

