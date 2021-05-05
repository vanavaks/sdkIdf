/*
 * DO.h
 *
 *  Created on: 19 квіт. 2021 р.
 *      Author: Vanavaks
 */

#ifndef COMPONENTS_GPIOAPP_DO_H_
#define COMPONENTS_GPIOAPP_DO_H_
#include "Tag.h"
#include "driver/gpio.h"
#include "DI.h"

#define ESP_ERR_IO_PIN				(ESP_ERR_TAG_BASE + 0x20)

#define PDO(_key, _def) Pbool(_key, "DO", !SAVEBLE, _def)

class DO : public Tag{
public:
	DO(const gpio_num_t _pin, const tagProp_t* p);
	void set(bool v);
	bool get();
	static void start();
	//static void scanOut();
	DO* operator=(Tag* t){
		setBL(t->getBL());
		return this;
	}


	/*DO* operator=(DI* t){
		setBL(t->getBL());
		return this;
	}*/

	DO* operator=(bool v){
		setBL(v);
		return this;
	}

	virtual void setBL(bool val){
		Tag::setBL(val);
		gpio_set_level(pin, getBL());
	}

private:
	gpio_num_t pin = GPIO_NUM_0;
	static uint32_t mask;
};

#endif /* COMPONENTS_GPIOAPP_DO_H_ */
