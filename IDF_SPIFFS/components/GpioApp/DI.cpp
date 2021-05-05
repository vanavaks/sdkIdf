/*
 * DI.cpp
 *
 *  Created on: 19 квіт. 2021 р.
 *      Author: Vanavaks
 */

#include "DI.h"
#include "main.h"

uint32_t DI::mask = 0;
const char* DITag = "IO";



DI::DI(const gpio_num_t _pin, const tagProp_t* p):Tag(p){
	//if(p == NULL)  {ESP_ERROR_CHECK(ESP_ERR_TAG_NULL_POINTER); return;}


	if(prop->type != TAG_BOOL) {ESP_ERROR_CHECK(ESP_ERR_TYPE); return;}
	if (pin > GPIO_NUM_MAX){
		ESP_LOGE(DITag, "IO: name - %s, wrong pin %d", prop->KeyName, _pin);
		ESP_ERROR_CHECK(ESP_ERR_IO_PIN); return;
	}

	pin = _pin;

	DI::mask |= 1 << pin;
	//Tag((tagProp_t*)p);

}

DI::DI(const gpio_num_t _pin, const tagProp_t* p, bool inv) : DI(_pin, p){
	trueVal = !inv;
}

bool DI::get() {
	if(gpio_get_level(pin) == trueVal) setBL(true);
	else setBL(false);
	return getBL();
}

void DI::start() {
	gpio_config_t io_conf;

	io_conf.intr_type = GPIO_INTR_DISABLE;
	io_conf.pin_bit_mask = mask; 						//bit mask of the pins, use GPIO4/5 here
	io_conf.mode = GPIO_MODE_INPUT; 					//set as input mode
	io_conf.pull_up_en = GPIO_PULLUP_ENABLE; 			//enable pull-up mode
	gpio_config(&io_conf);
}
