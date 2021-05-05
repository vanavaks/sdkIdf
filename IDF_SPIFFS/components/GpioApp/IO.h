/*
 * IO.h
 *
 *  Created on: 14 квіт. 2021 р.
 *      Author: Vanavaks
 *
 *      3 DO
 *      1 AO
 *      4 DI
 *      1 1Wire
 *      1 AI
 */

#ifndef COMPONENTS_GPIOAPP_IO_H_
#define COMPONENTS_GPIOAPP_IO_H_

#include "driver/gpio.h"
#include "Tag.h"
#include "DO.h"
#define IO_CAT "IO"

//#define PDI(_key) 			{.KeyName = (_key), .category = (IO_CAT), .type = TAG_BOOL, .saveble = (0), .val = { .asbool = (bool)(0) }}
//#define PDO(_key) 			{.KeyName = (_key), .category = (IO_CAT), .type = TAG_BOOL, .saveble = (0), .val = { .asbool = (bool)(0) }}
//#define PAI(_key, _def) 	{.KeyName = (_key), .category = (IO_CAT), .type = TAG_UI32, .saveble = (SAVEBLE), .val = { .asi32 = (int32_t)(_def) }}
//#define PAO(_key, _def) 	{.KeyName = (_key), .category = (IO_CAT), .type = TAG_UI32, .saveble = (SAVEBLE), .val = { .asi32 = (int32_t)(_def) }}

//#define PDI(_key, _pin) 	{.type = IO_DI, .pin = pin, { .KeyName = (_key), .category = (IO_CAT), .type = TAG_BOOL, .saveble = (0), .val = { .asbool = (bool)(0) }}}
//#define PDO(_key, _pin) 	{.type = IO_DO, .pin = pin, { .KeyName = (_key), .category = (IO_CAT), .type = TAG_BOOL, .saveble = (0), .val = { .asbool = (bool)(0) }}}

#define PDI(_key, _def) Pbool(_key, "DI", !SAVEBLE, _def);
#define PDO(_key, _def) Pbool(_key, "DO", !SAVEBLE, _def);

#define ESP_ERR_IO_PIN				(ESP_ERR_TAG_BASE + 0x20)

typedef enum {
	IO_DI, IO_DO, IO_AI, IO_AO, IO_WIRE,
} io_t;

struct ioProp_t {
	io_t type;
	gpio_num_t pin;
	tagProp_t tagProp;

};

class _IO: Tag {
public:
	_IO();
	/*	tag must be staticaly created
	 *
	 */
	//IO(io_t type, Tag* tg, gpio_num_t n);
	_IO(const io_t type, const tagProp_t* tagProp, const gpio_num_t n);_IO(const ioProp_t* prop);

	static void start();
	void setD(bool v);
	bool getD();

	virtual ~_IO();
private:
	const ioProp_t* prp = 0;
	//io_t ioTp;
	//gpio_num_t pin = 0;
	static uint32_t inMask;
	static uint32_t outMask;
};

/*=============================== DI ==================================*/

/*============================== DO ===================================*/

class IO {
public:
	IO();
	virtual ~IO();
};
#endif /* COMPONENTS_GPIOAPP_IO_H_ */

