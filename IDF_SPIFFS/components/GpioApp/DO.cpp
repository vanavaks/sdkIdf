/*
 * DO.cpp
 *
 *  Created on: 19 квіт. 2021 р.
 *      Author: Vanavaks
 */

#include "DO.h"
#include "main.h"

uint32_t DO::mask = 0;
const char* DOTag = "IO";

DO::DO(const gpio_num_t _pin, const tagProp_t* p) : Tag(p) {
	//if(prop == NULL)  {ESP_ERROR_CHECK(ESP_ERR_TAG_NULL_POINTER); return;}
	if(prop->type != TAG_BOOL) {ESP_ERROR_CHECK(ESP_ERR_TYPE); return;}
	if (pin > GPIO_NUM_MAX){
		ESP_LOGE(DOTag, "IO: name - %s, wrong pin %d", prop->KeyName, pin);
		ESP_ERROR_CHECK(ESP_ERR_IO_PIN); return;
	}
	pin = _pin;
	DO::mask |= 1 << pin;
	//Tag((tagProp_t*)p);
}

void DO::set(bool v) {
	setBL(v);
	gpio_set_level(pin, getBL());
}

bool DO::get() {
	if(gpio_get_level(pin)>0) setBL(true);
	else setBL(false);
	return getBL();
}

void DO::start() {
	gpio_config_t io_conf;

	io_conf.intr_type = GPIO_INTR_DISABLE;				//disable interrupt
	io_conf.mode = GPIO_MODE_OUTPUT;					//set as output mode
	io_conf.pin_bit_mask = mask;						//bit mask of the pins that you want to set,e.g.GPIO15/16
	io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;		//disable pull-down mode
	io_conf.pull_up_en = GPIO_PULLUP_DISABLE;			//disable pull-up mode
	gpio_config(&io_conf);								//configure GPIO with the given settings
}

