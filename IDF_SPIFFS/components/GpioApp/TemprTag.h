/*
 * TemprTag.h
 *
 *  Created on: 21 квіт. 2021 р.
 *      Author: Vanavaks
 */

#ifndef COMPONENTS_GPIOAPP_TEMPRTAG_H_
#define COMPONENTS_GPIOAPP_TEMPRTAG_H_

#include "Tag.h"
#include "driver/gpio.h"
#include "ds18x20.h"

#define TEMPR_TAG "tempTag"
#define PTEMPR(_key, _cat, _def) Pflt(_key, _cat, !SAVEBLE, _def);
#define ESP_ERR_TEMPR_ADR			(ESP_ERR_TAG_BASE + 0x21)
#define ESP_ERR_TEMPR_ARR			(ESP_ERR_TAG_BASE + 0x22)
#define TEMPR_TAG_MAX_NUM 5
#if (TEMPR_TAG_MAX_NUM < 1)
#error "TEMPR_TAG_MAX_NUM to low must be >= 1"
#endif

#define TEMPRTAG_SCAN_TIME 1000		// milliseconds

class TemprTag : public Tag{
public:
	TemprTag(ds18x20_addr_t _addr, const tagProp_t* p);
	static void init(const gpio_num_t _pin);
	float getTempr(){return getFL();};
		/* reading temperatures */

	virtual ~TemprTag();
protected:
	static gpio_num_t temprPin;
	static TemprTag* arr[TEMPR_TAG_MAX_NUM];
	void arrAdd();
	static void update(void *pvParameter);
	static TemprTag* arrGet(uint8_t n);
	static void arrInit();
	static uint8_t arrHead;
	ds18x20_addr_t adr;
};

#endif /* COMPONENTS_GPIOAPP_TEMPRTAG_H_ */
