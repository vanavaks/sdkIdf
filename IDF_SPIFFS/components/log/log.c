/*
 * log.c
 *
 *  Created on: 15 мар. 2021 г.
 *      Author: Vanavaks
 */

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>
#include <stddef.h>
#include "log.h"
#if 1

/*		messData:
 * 	1. const char*	- message
 * 	2. enum			- log level
 * 	3. const char*	- tag
 *
 * 	dinamic struct
 *
 * 	callback
 */
xQueueHandle xQueue;

void log_task(void *pvParameter){
	void lReceivedValue;
	portBASE_TYPE xStatus;
	for(;;){
		xStatus = xQueueReceive(xQueue, &lReceivedValue, 100 /portTICK_RATE_MS);
		esp_log_level_t level;
		const char* tag = "";
		const char* val = "";
		 esp_log_write(level, tag, val, );
	}
}

void logInit(){
	xTaskCreate(&log_task, "cmd_task", 2000, NULL, 5, NULL);
}

void logI();

#define LOGI(tag, ...) do{\
} while(0)

void log(esp_log_level_t level, const char* tag, const char* format, ...) __attribute__ ((format (printf, 3, 4))){
	_lock_acquire_recursive(;)
}

#endif
