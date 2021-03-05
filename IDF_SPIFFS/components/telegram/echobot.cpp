/**************************************************************************************************/
// Example: echobot
// Description: 
//   Bot that response to any received text message with the same text received (echo messages).
//   It gives you a basic idea of how to receive and send messages.
// Created on: 21 apr. 2019
// Last modified date: 21 apr. 2019
// Version: 1.0.0
/**************************************************************************************************/

/* Libraries */

// Standard C/C++ libraries
#include <esp_err.h>
#include <esp_event_legacy.h>
#include <esp_log.h>
#include "esp_event.h"
#include "freertos/event_groups.h"
#include <esp_system.h>
#include <esp_wifi.h>
#include <lwip/ip4_addr.h>
#include <portmacro.h>
#include <sys/_stdint.h>
#include <task.h>
#include <tcpip_adapter.h>
#include "utlgbotlib.h"
#include <cstdio>

const char* tagTlg = "Telegram";

/**************************************************************************************************/

// Telegram Bot Token (Get from Botfather)
#define TLG_TOKEN "1363254407:AAGiQ4GA6BtkYGHDDUKRY8yaKRdArt1DsfM"
#define MAX_CONN_FAIL 50
/**************************************************************************************************/

/* Functions Prototypes */

static esp_err_t network_event_handler(void *ctx, system_event_t *e);

/**************************************************************************************************/

/* Globals */
volatile bool wifi_connected = false;
volatile bool wifi_has_ip = false;

/**************************************************************************************************/

/* Telegram bot init */
void tlg_init(){
	// Create Bot object
	    uTLGBot Bot(TLG_TOKEN);
	// Set TCP-IP event handler callback
	    ESP_ERROR_CHECK(esp_event_loop_init(network_event_handler, NULL));

}


static void on_StaConnected(void *arg, esp_event_base_t event_base,int32_t event_id, void *event_data){
	ESP_LOGI(tagTlg, "wifi connected event");
	wifi_connected = true;
}

static void on_StaGotIP(void *arg, esp_event_base_t event_base,int32_t event_id, void *event_data){
	ESP_LOGI(tagTlg, "got IP event");
	wifi_has_ip = true;
}

static void on_StaLostIP(void *arg, esp_event_base_t event_base,int32_t event_id, void *event_data){
	ESP_LOGI(tagTlg, "lost ip event");
	 wifi_has_ip = false;
}

static void on_StaDisconn(void *arg, esp_event_base_t event_base,int32_t event_id, void *event_data){
	ESP_LOGI(tagTlg, "wifi disconnected event");
	wifi_has_ip = false;
	wifi_connected = false;

}


/* Main Function */

void tlg_task(void *pvParameter)
{
	printf("TelegramBot starting\r\n");
    // Create Bot object
    uTLGBot Bot(TLG_TOKEN);
    Bot.set_debug(2);
    // Set TCP-IP event handler callback
    
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_CONNECTED, &on_StaConnected, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &on_StaGotIP, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_LOST_IP, &on_StaLostIP, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &on_StaDisconn, NULL));

    // Main loop
    while(1)
    {
        // Check if device is not connected
        if(!wifi_connected || !wifi_has_ip)
        {
            // Wait 100ms and check again
            vTaskDelay(100/portTICK_PERIOD_MS);
            continue;
        }

        // Check and handle any received message
        while(Bot.getUpdates())
        {
            printf("Message received from %s, echo it back...\n", Bot.received_msg.from.first_name);
            if(!Bot.sendMessage(Bot.received_msg.chat.id, Bot.received_msg.text))
            {
                printf("Send fail.\n");
                continue;
            }
            printf("Send OK.\n\n");
        }
        
        // Wait 1s for next iteration
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

/**************************************************************************************************/

/* Functions */


/**************************************************************************************************/

/* WiFi Change Event Handler */



static esp_err_t network_event_handler(void *ctx, system_event_t *e)
{
    static uint8_t conn_fail_retries = 0;


    switch(e->event_id)
    {
        case SYSTEM_EVENT_STA_START:
            printf("WiFi Station interface Up.\n");
            printf("Connecting...\n");
            esp_wifi_connect();
            break;

        case SYSTEM_EVENT_STA_CONNECTED:
            printf("WiFi connected.\n");
            printf("Waiting for IP...\n");
            wifi_connected = true;
            break;

        case SYSTEM_EVENT_STA_GOT_IP:
            printf("WiFi IPv4 received: %s\n", ip4addr_ntoa(&e->event_info.got_ip.ip_info.ip));
            wifi_has_ip = true;
            break;

        case SYSTEM_EVENT_STA_LOST_IP:
            printf("WiFi IP lost.\n");
            wifi_has_ip = false;
            break;

        case SYSTEM_EVENT_STA_DISCONNECTED:
            if(wifi_connected)
            {
                printf("WiFi disconnected\n");
                conn_fail_retries = 0;
            }
            else
            {
                printf("Can't connect to AP, trying again...\n");
                conn_fail_retries = conn_fail_retries + 1;
            }
            wifi_has_ip = false;
            wifi_connected = false;
            if(conn_fail_retries < MAX_CONN_FAIL)
                esp_wifi_connect();
            else
            {
                printf("WiFi connection fail %d times.\n", MAX_CONN_FAIL);
                printf("Rebooting the system...\n\n");
                esp_restart();
            }
            break;

        case SYSTEM_EVENT_STA_STOP:
            printf("WiFi interface stopped\n");
            conn_fail_retries = 0;
            wifi_has_ip = false;
            wifi_connected = false;
            break;

        default:
            break;
    }
    
    return ESP_OK;
}

/**************************************************************************************************/
