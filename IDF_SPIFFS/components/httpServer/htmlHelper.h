/*
 * htmpHelper.h
 *
 *  Created on: 20 ���. 2021 �.
 *      Author: Vanavaks
 */

#ifndef COMPONENTS_HTTPSERVER_HTMLHELPER_H_
#define COMPONENTS_HTTPSERVER_HTMLHELPER_H_


#include "../../../../../../ESP8266_RTOS_SDK_IDF/components/esp_http_server/include/esp_http_server.h"
#include "../../../../../../ESP8266_RTOS_SDK_IDF/components/lwip/lwip/src/include/lwip/err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	const char* title;
	const char* url;
	const char* menuTitle;
	const char* category;
	const char* butName;
}http_category_t;

err_t sendHtml(httpd_req_t *req);
err_t sendNetHtml(httpd_req_t *req);
err_t sendConfigContent(httpd_req_t *req, http_category_t* cat);
err_t sendIOContent(httpd_req_t *req);
err_t html_sendConfigContent(httpd_req_t *req, http_category_t* cat, uint8_t num);
void html_init();
#ifdef __cplusplus
}
#endif

#endif /* COMPONENTS_HTTPSERVER_HTMLHELPER_H_ */
