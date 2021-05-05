/*
 * AO.cpp
 *
 *  Created on: 22 квіт. 2021 р.
 *      Author: Vanavaks
 */

#include "AO.h"

//aoPar_t AO::pars[AO_TAG_MAX_NUM];
uint8_t AO::arrHead = 0;
uint32_t AO::pin_num[AO_TAG_MAX_NUM];
uint32_t AO::duties[AO_TAG_MAX_NUM];

AO::AO(const aoPar_t* par, const tagProp_t* p) : Tag(p){
	AO::numChan = arrHead;
	if(arrHead >= AO_TAG_MAX_NUM) {ESP_ERROR_CHECK(ESP_ERR_TEMPR_ARR); return;}
	pin_num[arrHead] = par->pin;
			// Here must be dutie chack
	duties[arrHead] = par->dutie;
	numChan = arrHead++;
}

void AO::set(uint32_t v) {
	Tag::set(v);
	ESP_ERROR_CHECK( pwm_set_duty(numChan, getUI32()));
}

AO::~AO() {
	// TODO Auto-generated destructor stub
}
/*
void AO::arrAdd() {
	if(arrHead >= AO_TAG_MAX_NUM) {ESP_ERROR_CHECK(ESP_ERR_TEMPR_ARR); return;}
	arr[arrHead] = this;
	arrHead++;
}

AO* AO::arrGet(uint8_t n) {
	if(n >= AO_TAG_MAX_NUM) {ESP_ERROR_CHECK(ESP_ERR_TEMPR_ARR); return NULL;}
	return arr[n];
}
*/
void AO::init() {
	pwm_init(PWM_PERIOD, duties, arrHead, pin_num);
	//pwm_set_phases(phase);
	pwm_start();
}
/*
void AO::arrInit() {
	arrHead = 0;
	for(int i=0;i<AO_TAG_MAX_NUM; i++){
		arr[i] = NULL;
	}
}*/
