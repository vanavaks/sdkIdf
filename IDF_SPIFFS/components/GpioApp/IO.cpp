/*
 * IO.cpp
 *
 *  Created on: 14 квіт. 2021 р.
 *      Author: Vanavaks
 */

#include "IO.h"
#include "main.h"
#include "DI.h"
#include "DO.h"
#include <IO.h>
const char* IOTag = "IO";

uint32_t _IO::inMask = 0;
uint32_t _IO::outMask = 0;

//uint32_t DI::mask = 0;
//uint32_t DO::mask = 0;
_IO::_IO()
{
	// TODO Auto-generated constructor stub

}

/*IO::IO(io_t type, Tag* tg, gpio_num_t n) {
	Tag()
}*/

/*IO::IO(const io_t type, const tagProp_t* tagProp, const gpio_num_t n) {

	Tag(tagProp);

	switch (type){
	case IO_DI:
		IO::inMask |= 1 << n;
		break;
	case IO_DO:
		IO::outMask |= 1 << n;
		break;
	default:
		ESP_LOGE(IOTag, "Tag: name - %s, wrong type %d", tagProp->KeyName, type);
		break;
	}
}*/

_IO::_IO(const ioProp_t *prop){
	if(prop == NULL)  ESP_ERROR_CHECK(ESP_ERR_TAG_NULL_POINTER);prp = prop;

	if (prp->pin > GPIO_NUM_MAX){
		ESP_LOGE(
		IOTag, "IO: name - %s, wrong pin %d", prp->tagProp.KeyName, prp->pin);}

	switch (prp->type){
	case IO_DI:
		if(prp->tagProp.type != TAG_BOOL) {
			goto typeErr;
		}
		_IO::inMask |= 1 << prp->pin;
		break;
	case IO_DO:
		if(prp->tagProp.type != TAG_UI32) {
			goto typeErr;
		}
		_IO::outMask |= 1 << prp->pin;
		break;
	default:
		goto typeErr;
		break;
	}
	Tag(&(prp->tagProp));

	typeErr: ESP_LOGE(
		IOTag, "Tag: name - %s, wrong type", prop->tagProp.KeyName); return;
}

void _IO::start() {

gpio_config_t io_conf;
/****config DO*****/

io_conf.intr_type = GPIO_INTR_DISABLE;				//disable interrupt
io_conf.mode = GPIO_MODE_OUTPUT;					//set as output mode
io_conf.pin_bit_mask = outMask;	//bit mask of the pins that you want to set,e.g.GPIO15/16
io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;		//disable pull-down mode
io_conf.pull_up_en = GPIO_PULLUP_DISABLE;			//disable pull-up mode
gpio_config(&io_conf);					//configure GPIO with the given settings

io_conf.intr_type = GPIO_INTR_DISABLE;
io_conf.pin_bit_mask = inMask; 			//bit mask of the pins, use GPIO4/5 here
io_conf.mode = GPIO_MODE_INPUT; 					//set as input mode
io_conf.pull_up_en = GPIO_PULLUP_ENABLE; 			//enable pull-up mode
gpio_config(&io_conf);

}

void _IO::setD(bool v) {
if (prp->type == IO_DO || prp->type == IO_DI) {
	setBL(v);
	if (prp->type == IO_DO) {
		gpio_set_level(prp->pin, getBL());
	}
} else
	ESP_ERROR_CHECK(ESP_ERR_TYPE);
}

bool _IO::getD() {
if (gpio_get_level(prp->pin) > 0)
	setBL(true);
else
	setBL(false);
return getBL();
}

_IO::~_IO() {
// TODO Auto-generated destructor stub
}

/*=============================== DI ==================================*/
#if 0
DI::DI(const gpio_num_t _pin, const tagProp_t* p) {
if(prop == NULL) {ESP_ERROR_CHECK(ESP_ERR_TAG_NULL_POINTER); return;}
if(prop->type != TAG_BOOL) {ESP_ERROR_CHECK(ESP_ERR_TYPE); return;}
if (pin > GPIO_NUM_MAX) {
	ESP_LOGE(IOTag, "IO: name - %s, wrong pin %d", prop->KeyName, _pin);
	ESP_ERROR_CHECK(ESP_ERR_IO_PIN); return;
}

pin = _pin;

DI::mask |= 1 << pin;
TagBool((tagProp_t*)p);
}

bool DI::get() {
if(gpio_get_level(pin)>0) setBL(true);
else setBL(false);
return getBL();
}

void DI::start() {
gpio_config_t io_conf;

io_conf.intr_type = GPIO_INTR_DISABLE;
io_conf.pin_bit_mask = mask; 			//bit mask of the pins, use GPIO4/5 here
io_conf.mode = GPIO_MODE_INPUT;//set as input mode
io_conf.pull_up_en = GPIO_PULLUP_ENABLE;//enable pull-up mode
gpio_config(&io_conf);
}

/*============================== DO ===================================*/

DO::DO(const gpio_num_t _pin, const tagProp_t* p) {
if(prop == NULL) {ESP_ERROR_CHECK(ESP_ERR_TAG_NULL_POINTER); return;}
if(prop->type != TAG_BOOL) {ESP_ERROR_CHECK(ESP_ERR_TYPE); return;}
if (pin > GPIO_NUM_MAX) {
	ESP_LOGE(IOTag, "IO: name - %s, wrong pin %d", prop->KeyName, pin);
	ESP_ERROR_CHECK(ESP_ERR_IO_PIN); return;
}
pin = _pin;
DO::mask |= 1 << pin;
TagBool((tagProp_t*)p);
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
io_conf.mode = GPIO_MODE_OUTPUT;//set as output mode
io_conf.pin_bit_mask = mask;//bit mask of the pins that you want to set,e.g.GPIO15/16
io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;//disable pull-down mode
io_conf.pull_up_en = GPIO_PULLUP_DISABLE;//disable pull-up mode
gpio_config(&io_conf);//configure GPIO with the given settings
}
#endif

IO::IO() {
// TODO Auto-generated constructor stub

}

IO::~IO() {
// TODO Auto-generated destructor stub
}

