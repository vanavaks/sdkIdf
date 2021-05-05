/*
 * AO.h
 *
 *  Created on: 22 квіт. 2021 р.
 *      Author: Vanavaks
 */

#ifndef COMPONENTS_GPIOAPP_AO_H_
#define COMPONENTS_GPIOAPP_AO_H_

#include "Tag.h"
#include "driver/gpio.h"
#include "driver/pwm.h"

#define TEMPR_TAG "AOTag"
#define PAO(_key, _def) Pui32(_key, "AO", !SAVEBLE, _def);

#define ESP_ERR_TEMPR_ARR			(ESP_ERR_TAG_BASE + 0x22)

#define AO_TAG_MAX_NUM 1
#if (AO_TAG_MAX_NUM < 1)
#error "AO_TAG_MAX_NUM to low must be >= 1"
#endif
	// PWM period 1000us(1Khz), same as depth
#define PWM_PERIOD (1000)


typedef struct{
	gpio_num_t 	pin;
	uint32_t 	dutie;		// duties table, real_duty = duties[x]/PERIOD
	float 		phase;		// phase table, delay = (phase[x]/360)*PERIOD
} aoPar_t;

class AO : Tag{
public:
	AO(const aoPar_t* par, const tagProp_t* p);
		//init must be started after activation of all AO Tags
	static void init();
	//uint32_t get();
	void set(uint32_t v);
	virtual ~AO();
protected:
	uint8_t numChan = 0;
	static uint32_t pin_num[AO_TAG_MAX_NUM];
	static uint32_t duties[AO_TAG_MAX_NUM];
	static uint8_t arrHead;
	//static aoPar_t pars[AO_TAG_MAX_NUM];
	//static AO* arrGet(uint8_t n);
	//void arrAdd();
	//static void arrInit();
};

#endif /* COMPONENTS_GPIOAPP_AO_H_ */
