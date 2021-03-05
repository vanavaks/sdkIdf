/*
 * tlg.cpp
 *
 *  Created on: 11 февр. 2021 г.
 *      Author: Vanavaks
 */

#include "tlg.h"

#include <sys/_stdint.h>

#include "../../../../../../ESP8266_RTOS_SDK_IDF/components/esp-tls/esp_tls.h"
#include "esp_tls.h"


extern const uint8_t server_root_cert_pem_start[] asm("_binary_server_root_cert_pem_start");
extern const uint8_t server_root_cert_pem_end[]   asm("_binary_server_root_cert_pem_end");

esp_tls_cfg_t cfg = {
            .cacert_pem_buf  = server_root_cert_pem_start,
            .cacert_pem_bytes = (uint8_t)(server_root_cert_pem_end - server_root_cert_pem_start),
        };


void tlg_init(){
    esp_tls_conn_http_new(" ", &cfg);
}
