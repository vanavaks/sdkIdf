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
#include "Tag.h"
#include "wifi_connect.h"


/**************************************************************************************************/

/* Functions Prototypes */
void register_tlgSend();

const char* tlgTag = "Telegram";


/************************************DEFINES******************************************************/
#define TLG_CMD_SEND
#define TLG_debug_level true
#define _printf(...) do { if(TLG_debug_level) {ESP_LOGI(tagTlg,__VA_ARGS__);} } while(0)

//#define TLG_TOKEN "1376297845:AAFpVmpSaz50vvzjuY7mq1CgpgvW-RhdzOA"
//#define TLG_CHAT_ID "1336112235"
#define TLG_DEF_MESS "Hello its console, master"

typedef enum {
	TLG_GETUPD, TLG_SEND
} tlg_cmd;

/**************************************************************************************************/
/* variables */

uTLGBot* _Bot;
xQueueHandle xQueue;

Tag* tlg_token { 0 };
Tag* tlg_chatId { 0 };
Tag* tlg_echo { 0 };		//echo scan time (s), 0 - echo off,
Tag* tlg_enable { 0 };

const tagProp_t prop_tlg_token = Pstr("Tlg_token", "Tlg", SAVEBLE, "1376297845:AAFpVmpSaz50vvzjuY7mq1CgpgvW-RhdzOA");
const tagProp_t prop_tlg_chatId = Pstr("Tlg_chatId", "Tlg", SAVEBLE, "1336112235");
const tagProp_t prop_tlg_echo = Pbool("Tlg_echo","Tlg",SAVEBLE,0);
const tagProp_t prop_tlg_enable = Pbool("Tlg_enable","Tlg",SAVEBLE,0);

/*************************************************************************************************/



/* Main Function */
void tlg_task(void *pvParameter) {

	ESP_LOGI(tlgTag, "TelegramBot starting");
	// Create Bot object
	uTLGBot Bot(tlg_token); //TLG_TOKEN
	Bot.set_debug(2);
	_Bot = &Bot;
	register_tlgSend();
	portBASE_TYPE xStatus;
	vTaskDelay(500 / portTICK_PERIOD_MS);

	tlg_cmd lReceivedValue;
	for (;;) {
		if (wifi_is_connected()) {
			xStatus = xQueueReceive(xQueue, &lReceivedValue,
					100 / portTICK_RATE_MS);
			if (xStatus == pdPASS) {
				http_stopWebServ();
				vTaskDelay(500 / portTICK_PERIOD_MS);
				if (lReceivedValue == TLG_GETUPD) {
					ESP_LOGI(tlgTag, "Trying to get updates");
					while (Bot.getUpdates()) {
						ESP_LOGI(tlgTag,
								"Message received from %s, echo it back...",
								Bot.received_msg.from.first_name);
						if (!Bot.sendMessage(Bot.received_msg.chat.id,
								Bot.received_msg.text)) {
							ESP_LOGW(tlgTag, "Send fail.");
							continue;
						}
						ESP_LOGI(tlgTag, "Send OK.");
						//isId = true;

					}
				} else if (lReceivedValue == TLG_SEND) {
					ESP_LOGI(tlgTag, "Trying to send message");
					if (!Bot.sendMessage(tlg_chatId->getStr(), TLG_DEF_MESS)) {
						ESP_LOGI(tlgTag, "Send fail.\n");
					} else {
						ESP_LOGI(tlgTag, "Send OK.");
					}
				}
				Bot.disconnect();
				vTaskDelay(1000 / portTICK_PERIOD_MS);
				http_strtWebServ();
			}
		}
		//vTaskDelay(5000/portTICK_PERIOD_MS);
		taskYIELD();
	}
}

void tlg_cycle_task(void *pvParameter) {
	vTaskDelay(1000 / portTICK_PERIOD_MS);
	//ESP_LOGI(tlgTag,"tlg_cykle update echo task started");
	tlg_cmd lValueToSend = TLG_GETUPD;
	portBASE_TYPE xStatus;
	for (;;) {
		if(tlg_echo->getBL()){
			xStatus = xQueueSendToBack(xQueue, &lValueToSend, 0);
			if (xStatus != pdPASS) {
				//ESP_LOGE(tlgTag,"Could not send to the queue.");
			}
		}
		vTaskDelay(60000 / portTICK_PERIOD_MS);
	}
}


void tlgInit() {

	tlg_token = new Tag(&prop_tlg_token);
	tlg_chatId = new Tag(&prop_tlg_chatId);
	tlg_echo = new Tag(&prop_tlg_echo);
	tlg_enable = new Tag(&prop_tlg_enable);

	xQueue = xQueueCreate(5, sizeof(tlg_cmd));
	xTaskCreate(&tlg_task, "tlg_task", 16000, NULL, 4, NULL);
	xTaskCreate(&tlg_cycle_task, "tlg_cykle_task", 256, NULL, 4, NULL);
}

/**************************************************************************************************/

/* Functions */

/**************************************************************************************************/

//console cmd
#ifdef TLG_CMD_SEND

int tlgSend(int argc, char **argv) {
	tlg_cmd lValueToSend = TLG_SEND;
	portBASE_TYPE xStatus;
	xStatus = xQueueSendToBack(xQueue, &lValueToSend, 0);
	if (xStatus != pdPASS) {
		//_printf("Could not send to the queue.\r\n");
		ESP_LOGW(tlgTag, "Could not send to the queue.\r\n");
	}
	return 0;
}

void register_tlgSend() {
	const esp_console_cmd_t cmd = {
			.command = "tlg",
			.help = "Send telegram message",
			.hint = NULL,
			.func = &tlgSend };
	ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}

#endif

/**************************************************************************************************/
