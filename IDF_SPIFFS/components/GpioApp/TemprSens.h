/*
 * TemprSens.h
 *
 *  Created on: 29 квіт. 2021 р.
 *      Author: Vanavaks
 */

#ifndef COMPONENTS_GPIOAPP_TEMPRSENS_H_
#define COMPONENTS_GPIOAPP_TEMPRSENS_H_

#include "Tag.h"
#include "driver/gpio.h"
#include "ds18x20.h"

#define TEMPR_TAG "tempTag"
#define TEMPR_CAT "DS18B20"

//TODO auto find adr mode
#define TEMPR_SENS(_key) 		TemprSens(PTEMPR_ADR(_key), PTEMPR_VAL(_key))
#define PTEMPR_ADR(_key) 		Pstr(_key + "_v", TEMPR_CAT, !SAVEBLE, "0");
#define PTEMPR_VAL(_key) 		Pflt(_key + "_a", TEMPR_CAT, SAVEBLE, -50);

#define ESP_ERR_TEMPR_ADR			(ESP_ERR_TAG_BASE + 0x21)
#define ESP_ERR_TEMPR_ARR			(ESP_ERR_TAG_BASE + 0x22)
#define TEMPR_TAG_MAX_NUM 5
#if (TEMPR_TAG_MAX_NUM < 1)
#error "TEMPR_TAG_MAX_NUM to low must be >= 1"
#endif

#define TEMPRTAG_SCAN_TIME 1000		// milliseconds

class TemprSens {
public:
	TemprSens(const tagProp_t* adrp, const tagProp_t* valp);
	virtual ~TemprSens();

	static void init(const gpio_num_t _pin, uint16_t scanTime);
	float getTempr(){return val->getFL();};
private:
	static gpio_num_t temprPin;
	static uint16_t scanTime = 1000;
	static TemprSens* arr[TEMPR_TAG_MAX_NUM];
	void arrAdd();
	Tag* val;
	Tag* adr;
};

#endif /* COMPONENTS_GPIOAPP_TEMPRSENS_H_ */
