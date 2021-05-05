/*
 * WebPage.h
 *
 *  Created on: 27 квіт. 2021 р.
 *      Author: Vanavaks
 *      Elements:
 *      	-Title
 *      	-Lists of Tags
 *      	-callback to contant creation function
 *      	-commit button
 *      	-auto update time
 *
 */

#ifndef COMPONENTS_HTTPSERVER_WEBPAGE_H_
#define COMPONENTS_HTTPSERVER_WEBPAGE_H_

#include "Tag.h"
#include <esp_wifi_types.h>
#include "../../../../../../ESP8266_RTOS_SDK_IDF/components/esp_http_server/include/esp_http_server.h"
#define WEBPAGE_CATEGORY_NUM 10

class WebPage {
public:
	WebPage();
	virtual ~WebPage();
private:
	const char* title;
	const char* category;
	const char* url;
	Tag* arr[WEBPAGE_CATEGORY_NUM];
	const httpd_uri_t* get_config;
	const httpd_uri_t* post_config;
};

#endif /* COMPONENTS_HTTPSERVER_WEBPAGE_H_ */
