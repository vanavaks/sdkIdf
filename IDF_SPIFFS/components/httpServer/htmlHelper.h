/*
 * htmpHelper.h
 *
 *  Created on: 20 џэт. 2021 у.
 *      Author: Vanavaks
 */

#ifndef COMPONENTS_HTTPSERVER_HTMLHELPER_H_
#define COMPONENTS_HTTPSERVER_HTMLHELPER_H_


#include "../../../../../../ESP8266_RTOS_SDK_IDF/components/esp_http_server/include/esp_http_server.h"
#include "../../../../../../ESP8266_RTOS_SDK_IDF/components/lwip/lwip/src/include/lwip/err.h"

#ifdef __cplusplus
extern "C" {
#endif

err_t sendHtml(httpd_req_t *req);
err_t sendNetHtml(httpd_req_t *req);

#ifdef __cplusplus
}
#endif

#endif /* COMPONENTS_HTTPSERVER_HTMLHELPER_H_ */
