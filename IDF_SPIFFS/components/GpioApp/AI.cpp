/*
 * AI.cpp
 *
 *  Created on: 23 ���. 2021 �.
 *      Author: Vanavaks
 */

#include "AI.h"
#include "Tag.h"
AI* AI::p_instance;

AI::AI() : Tag(&prop){
	// TODO Auto-generated constructor stub
	adc_config_t adc_config;

	// Depend on menuconfig->Component config->PHY->vdd33_const value
	// When measuring system voltage(ADC_READ_VDD_MODE), vdd33_const must be set to 255.
	adc_config.mode = ADC_READ_TOUT_MODE;
	//ADC_READ_TOUT_MODE;
	adc_config.clk_div = 8; // ADC sample collection clock = 80MHz/clk_div = 10MHz
	ESP_ERROR_CHECK(adc_init(&adc_config));
}

AI::~AI() {
	// TODO Auto-generated destructor stub
}

