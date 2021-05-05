/*
 * DI.h
 *
 *  Created on: 19 квіт. 2021 р.
 *      Author: Vanavaks
 */

#ifndef COMPONENTS_GPIOAPP_DI_H_
#define COMPONENTS_GPIOAPP_DI_H_

#include "Tag.h"
#include "driver/gpio.h"
#include "DO.h"

#define ESP_ERR_IO_PIN				(ESP_ERR_TAG_BASE + 0x20)
#define PDI(_key, _def) Pbool((_key), "DI", (!SAVEBLE), (_def))

#define DI_INV true

class DI : public Tag{
public:
	DI(const gpio_num_t _pin, const tagProp_t* p);
	DI(const gpio_num_t _pin, const tagProp_t* p, bool inv);
	//void set(bool v){setBL(v);};
	bool get();
	static void start();
	void setTrue(bool v){trueVal = v;};
	operator bool() {
		return this->getBL();
	}

	/*operator DO*() const {
		return this;
	}*/
private:
	bool trueVal = true;
	gpio_num_t pin = GPIO_NUM_0;
	static uint32_t mask;
};

#endif /* COMPONENTS_GPIOAPP_DI_H_ */
