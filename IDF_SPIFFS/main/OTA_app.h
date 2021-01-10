/*
 * OTA_app.h
 *
 *  Created on: 9 џэт. 2021 у.
 *      Author: Vanavaks
 */

#ifndef MAIN_OTA_APP_H_
#define MAIN_OTA_APP_H_

#pragma once

#define CONFIG_SERVER_IP "192.168.0.103"
#define CONFIG_SERVER_PORT "8070"
#define CONFIG_EXAMPLE_FILENAME "ota.ota.bin"

#define EXAMPLE_SERVER_IP   CONFIG_SERVER_IP
#define EXAMPLE_SERVER_PORT CONFIG_SERVER_PORT
#define EXAMPLE_FILENAME CONFIG_EXAMPLE_FILENAME
#define BUFFSIZE 1500
#define TEXT_BUFFSIZE 1024

typedef enum esp_ota_firm_state {
    ESP_OTA_INIT = 0,
    ESP_OTA_PREPARE,
    ESP_OTA_START,
    ESP_OTA_RECVED,
    ESP_OTA_FINISH,
} esp_ota_firm_state_t;

typedef struct esp_ota_firm {
    uint8_t             ota_num;
    uint8_t             update_ota_num;

    esp_ota_firm_state_t    state;

    size_t              content_len;

    size_t              read_bytes;
    size_t              write_bytes;

    size_t              ota_size;
    size_t              ota_offset;

    const char          *buf;
    size_t              bytes;
} esp_ota_firm_t;

//void ota_example_task(void *pvParameter);
void register_OtaUpdate();

#endif /* MAIN_OTA_APP_H_ */
