/*
 * AI.h
 *
 *  Created on: 23 квіт. 2021 р.
 *      Author: Vanavaks
 */

#ifndef COMPONENTS_GPIOAPP_AI_H_
#define COMPONENTS_GPIOAPP_AI_H_

#include "driver/adc.h"
#include "Tag.h"
#include "main.h"
#include <esp_err.h>
#include <esp_log.h>

#define PAI(_key, _cat, _def) Pui32(_key, _cat, !SAVEBLE, _def);
//TODO analog scaling
// uint32_t min, max;

class AI : public Tag{
public:
	static AI* getInstance() {
		if(p_instance == NULL)
			p_instance = new AI();
		return p_instance;
	}
	void get(){
		uint16_t v;
		adc_read(&v); //ESP_ERROR_CHACK (adc_read(&v));
		set((uint32_t)v);
	}

private:
	const char* tag = "AI";
	const tagProp_t prop = PAI("AI0", tag, 0);
	static AI* p_instance;
	adc_config_t adc_config;
	AI();
	virtual ~AI();
};

#endif /* COMPONENTS_GPIOAPP_AI_H_ */
