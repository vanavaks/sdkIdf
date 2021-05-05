/*
 * IO_app.cpp
 *
 *  Created on: 16 квіт. 2021 р.
 *      Author: Vanavaks
 */

#include "IO_app.h"
#include "IO.h"
#include "main.h"
#include "ds18x20.h"
#include "AI.h"
#include "AO.h"

const char* IO_tag = "IO";

#define ONEWIRE_PIN GPIO_NUM_5
#define NUM_TEMPR_SENSORS 3
//------------------------- DI  -------------------------------//
DI* di_0 { 0 };		const tagProp_t prop_DI0 = PDI("Di0", false);//Pbool("Di_0","DI", false, false);
DI* di_1 { 0 };		const tagProp_t prop_DI1 = PDI("Di1", false);
DI* di_2 { 0 };		const tagProp_t prop_DI2 = PDI("Di2", false);
DI* di_3 { 0 };		const tagProp_t prop_DI3 = PDI("Di3", false);
DI* di_4 { 0 };		const tagProp_t prop_DI4 = PDI("Di4", false);

//------------------------- DO --------------------------------//

DO* dO_boiler { 0 };			const tagProp_t prop_DO0 = PDO("DO0", false);
DO* dO_heatValve { 0 };			const tagProp_t prop_DO2 = PDO("DO2", false);
DO* dO_3 { 0 };					const tagProp_t prop_DO3 = PDO("DO3", false);

//------------------------- AI --------------------------------//

//AI* ai = AI::getInstance();

//------------------------- AO --------------------------------//

AO* ao_0 { 0 };		const tagProp_t prop_AO0 = PAO("AO0", 0);
					const aoPar_t prop_AO = {.pin = GPIO_NUM_12, .dutie = 500};
//------------------------- Tempr -----------------------------//

ds18x20_addr_t tempSensors[NUM_TEMPR_SENSORS]{0};
ds18x20_addr_t temp1Adr = 51613513;
float tempr1;

void IOapp_Init(){

	di_0 = new DI(GPIO_NUM_16, &prop_DI0, DI_INV);
//	di_1 = new DI(GPIO_NUM_5, &prop_DI1);

	di_2 = new DI(GPIO_NUM_4, &prop_DI2);
	di_3 = new DI(GPIO_NUM_0, &prop_DI3);
	di_4 = new DI(GPIO_NUM_2, &prop_DI4, DI_INV);
	DI::start();

	dO_boiler = new DO(GPIO_NUM_14, &prop_DO0);
	dO_heatValve = new DO(GPIO_NUM_13, &prop_DO2);
	dO_3 = new DO(GPIO_NUM_15, &prop_DO3);
	DO::start();

	ao_0 = new AO(&prop_AO, &prop_AO0);
	size_t numSens;


	ESP_ERROR_CHECK(ds18x20_scan_devices(ONEWIRE_PIN, tempSensors, NUM_TEMPR_SENSORS, &numSens));

	ESP_LOGI(IO_tag, "IO started");
	 while (1) {


		 vTaskDelay(1000 / portTICK_RATE_MS);
		 //ESP_ERROR_CHECK(ds18x20_read_temperature(ONEWIRE_PIN, temp1Adr, &tempr1));
		 di_0->get();
		 di_2->get();
		 di_3->get();
		 di_4->get();

		 bool v = false;



		 dO_heatValve->setBL(di_0->get());
		 //dO_heatValve->setBL(false);
		 dO_boiler->setBL(di_2->get());

		 //dO_2 = dI_2->getBL();
	}
}

//ds18_b20

//temperature init



//to realize simple logical operation with bool tags and bools vars = || &&
//simple declaration

