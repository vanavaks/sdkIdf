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
#include <esp_console.h>
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

#include <sys/socket.h>
#include "freertos/queue.h"
#include "http.h"
const char* tlgTag = "Telegram";
#define TLG_debug_level true

#define _printf(...) do { if(TLG_debug_level) {ESP_LOGI(tagTlg,__VA_ARGS__);} } while(0)
//#define _println(x) do { if(TLG_debug_level) 	ESP_LOGI(tagTlg, x); } while(0)
/**************************************************************************************************/

// Telegram Bot Token (Get from Botfather)
#define TLG_TOKEN "1376297845:AAFpVmpSaz50vvzjuY7mq1CgpgvW-RhdzOA"
#define MAX_CONN_FAIL 50
/**************************************************************************************************/

/* Functions Prototypes */

static esp_err_t network_event_handler(void *ctx, system_event_t *e);

/**************************************************************************************************/

/* Globals */
volatile bool wifi_connected = false;
volatile bool wifi_has_ip = false;

/**************************************************************************************************/


static void on_StaConnected(void *arg, esp_event_base_t event_base,int32_t event_id, void *event_data){
	ESP_LOGI(tlgTag, "wifi connected event");
	wifi_connected = true;
}

static void on_StaGotIP(void *arg, esp_event_base_t event_base,int32_t event_id, void *event_data){
	ESP_LOGI(tlgTag, "got IP event");
	wifi_has_ip = true;
}

static void on_StaLostIP(void *arg, esp_event_base_t event_base,int32_t event_id, void *event_data){
	ESP_LOGI(tlgTag, "lost ip event");
	 wifi_has_ip = false;
}

static void on_StaDisconn(void *arg, esp_event_base_t event_base,int32_t event_id, void *event_data){
	ESP_LOGI(tlgTag, "wifi disconnected event");
	wifi_has_ip = false;
	wifi_connected = false;

}
#define TLG_CHAT_ID "1336112235"
#define TLG_DEF_MESS "Hello its console, master"
#if 1
uTLGBot* _Bot;
xQueueHandle xQueue;
typedef enum{TLG_GETUPD, TLG_SEND} tlg_cmd;

int tlgSend(int argc, char **argv)
{
	/*if (_Bot != NULL) _Bot->getUpdates();
	else{
		printf("Object null pointer.\n");
		return 0;
	}*/

	/*ESP_LOGI(tagTlg, "Trying to send message");
	if(!_Bot->sendMessage(TLG_CHAT_ID, "Hello its console cmd")){
		printf("Send fail.\n");
	}*/
	tlg_cmd lValueToSend = TLG_SEND;
	portBASE_TYPE xStatus;
	xStatus = xQueueSendToBack(xQueue, &lValueToSend, 0);
	 if (xStatus != pdPASS) {
		 //_printf("Could not send to the queue.\r\n");
		 ESP_LOGW(tlgTag,"Could not send to the queue.\r\n");
	 }
    return 0;
}

void register_tlgSend()
{
    const esp_console_cmd_t cmd = {
        .command = "tlg",
        .help = "Send telegram message",
        .hint = NULL,
        .func = &tlgSend
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );
}

#endif

/* Main Function */
#define TLG_QUEUI
#ifdef TLG_QUEUI




void tlg_task(void *pvParameter){

	ESP_LOGI(tlgTag,"TelegramBot starting");
	    // Create Bot object
	uTLGBot Bot(TLG_TOKEN);
	Bot.set_debug(2);
	_Bot = &Bot;
	register_tlgSend();
	portBASE_TYPE xStatus;
	vTaskDelay(500/portTICK_PERIOD_MS);

	tlg_cmd lReceivedValue;
	for(;;){
		xStatus = xQueueReceive(xQueue, &lReceivedValue, 100 /portTICK_RATE_MS);
		if(xStatus == pdPASS){
			http_stopWebServ();
			vTaskDelay(500/portTICK_PERIOD_MS);
			if(lReceivedValue == TLG_GETUPD){
				ESP_LOGI(tlgTag,"Trying to get updates");
				while(Bot.getUpdates()){
					ESP_LOGI(tlgTag,"Message received from %s, echo it back...", Bot.received_msg.from.first_name);
					if(!Bot.sendMessage(Bot.received_msg.chat.id, Bot.received_msg.text))
					{
						ESP_LOGW(tlgTag,"Send fail.");
						continue;
					}
					ESP_LOGI(tlgTag,"Send OK.");
					//isId = true;

				}
			}else if(lReceivedValue == TLG_SEND){
				ESP_LOGI(tlgTag,"Trying to send message");
				if(!Bot.sendMessage(TLG_CHAT_ID, TLG_DEF_MESS)){
					ESP_LOGI(tlgTag,"Send fail.\n");
				}else{
					ESP_LOGI(tlgTag,"Send OK.");
				}
			}
		http_strtWebServ();
		}
		//vTaskDelay(5000/portTICK_PERIOD_MS);
		taskYIELD();
	}
}


void tlg_cycle_task(void *pvParameter){
	vTaskDelay(1000/portTICK_PERIOD_MS);
	//ESP_LOGI(tlgTag,"tlg_cykle update echo task started");
	tlg_cmd lValueToSend = TLG_GETUPD;
	portBASE_TYPE xStatus;
	for(;;){
		xStatus = xQueueSendToBack(xQueue, &lValueToSend, 0);
		if (xStatus != pdPASS) {
			//ESP_LOGE(tlgTag,"Could not send to the queue.");
		}
		vTaskDelay(15000/portTICK_PERIOD_MS);
	}
}


#else
void tlg_task(void *pvParameter)
{
	bool isId = false;
	printf("TelegramBot starting\r\n");
    // Create Bot object
	uTLGBot Bot(TLG_TOKEN);
    Bot.set_debug(2);
    _Bot = &Bot;
    register_tlgSend();

    // Set TCP-IP event handler callback

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_CONNECTED, &on_StaConnected, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &on_StaGotIP, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_LOST_IP, &on_StaLostIP, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &on_StaDisconn, NULL));

    // Main loop
    while(1)
    {
#if 1
        // Check if device is not connected
        if(!wifi_connected || !wifi_has_ip)
        {
            // Wait 100ms and check again
            vTaskDelay(300/portTICK_PERIOD_MS);
            continue;
        }

        // Check and handle any received message
        //if(!isId){
			while(Bot.getUpdates())
			{
				printf("Message received from %s, echo it back...\n", Bot.received_msg.from.first_name);
				if(!Bot.sendMessage(Bot.received_msg.chat.id, Bot.received_msg.text))
				{
					printf("Send fail.\n");
					continue;
				}
				printf("Send OK.\n\n");
				//isId = true;

			}
#endif
			vTaskDelay(3000/portTICK_PERIOD_MS);
        /*}else{
        	Bot.sendMessage(Bot.received_msg.chat.id, "I'm here, master...");
        	vTaskDelay(30000/portTICK_PERIOD_MS);
        }*/

        // Wait 1s for next iteration

        //isId = 0;
    }
}

#endif

void tlgInit(){
	xQueue = xQueueCreate(5, sizeof(tlg_cmd));
	xTaskCreate(&tlg_task, "tlg_task", 16000, NULL, 4, NULL);
	xTaskCreate(&tlg_cycle_task, "tlg_cykle_task", 256, NULL, 4, NULL);
}

void vAFunction( void )
 {
 TaskHandle_t xHandle;
 TaskStatus_t xTaskDetails;

    // Obtain the handle of a task from its name.
    xHandle = xTaskGetHandle( "Task_Name" );

    // Check the handle is not NULL.
    configASSERT( xHandle );

    // Use the handle to obtain further information about the task.
    vTaskGetInfo( xHandle,
                  &xTaskDetails,
                  pdTRUE, // Include the high water mark in xTaskDetails.
                  eInvalid ); // Include the task state in xTaskDetails.
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
        	ESP_LOGI(tlgTag,"WiFi Station interface Up.");
        	ESP_LOGI(tlgTag,"Connecting...");
            esp_wifi_connect();
            break;

        case SYSTEM_EVENT_STA_CONNECTED:
        	ESP_LOGI(tlgTag,"WiFi connected.");
        	ESP_LOGI(tlgTag,"Waiting for IP...");
            wifi_connected = true;
            break;

        case SYSTEM_EVENT_STA_GOT_IP:
        	ESP_LOGI(tlgTag,"WiFi IPv4 received: %s", ip4addr_ntoa(&e->event_info.got_ip.ip_info.ip));
            wifi_has_ip = true;
            break;

        case SYSTEM_EVENT_STA_LOST_IP:
        	ESP_LOGI(tlgTag,"WiFi IP lost.");
            wifi_has_ip = false;
            break;

        case SYSTEM_EVENT_STA_DISCONNECTED:
            if(wifi_connected)
            {
            	ESP_LOGI(tlgTag,"WiFi disconnected");
                conn_fail_retries = 0;
            }
            else
            {
            	ESP_LOGI(tlgTag,"Can't connect to AP, trying again...");
                conn_fail_retries = conn_fail_retries + 1;
            }
            wifi_has_ip = false;
            wifi_connected = false;
            if(conn_fail_retries < MAX_CONN_FAIL)
                esp_wifi_connect();
            else
            {
            	ESP_LOGI(tlgTag,"WiFi connection fail %d times.", MAX_CONN_FAIL);
            	ESP_LOGI(tlgTag,"Rebooting the system...");
                esp_restart();
            }
            break;

        case SYSTEM_EVENT_STA_STOP:
        	ESP_LOGI(tlgTag,"WiFi interface stopped");
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
