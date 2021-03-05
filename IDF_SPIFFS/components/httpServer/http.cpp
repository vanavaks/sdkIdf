/*
 * http.c
 *
 *  Created on: 13 џэт. 2021 у.
 *      Author: Vanavaks
 */

#include <esp_err.h>
#include <esp_event.h>
#include <esp_event_base.h>
#include <esp_log.h>
#include <esp_wifi_types.h>
#include <http.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/param.h>
#include <tcpip_adapter.h>
#include "htmlHelper.h"
#include "Tag.h"


#define TAG_KEY_MAX_SIZE  20
#define TAG_VAL_MAX_SIZE  20

#define ESP_ERR_HTTP_POST_KEY_SIZE 0x01
#define ESP_ERR_HTTP_POST_VAL_SIZE 0x02

static const char *TAG="HTTP";
static httpd_handle_t server = NULL;


		/* ======== POST parsing ======== */

bool isSymbol(char c){
	if(c >= ' ') return true;
	if(c >= '0' && c <= '9') return true;
	if(c >= 'A' && c <= 'Z') return true;
	if(c >= 'a' && c <= 'z') return true;
	if(c == '_' ) return true;
	return false;
}

bool isDigSymv(char c){
	if(c >= '0' && c <= '9') return true;
	return false;
}

bool isIpSymv(char c){
	if(c >= '0' && c <= '9') return true;
	if(c == '.') return true;
	return false;
}

bool isUrlSymv(char c){
	if(isSymbol(c))
	if(c >= ' ') return true;
	return false;
}

static void strInit(char* s, uint8_t len){
	if(s == NULL) return;
	for(int i = 0; i<len;i++){
		*s = NULL;
		s++;
	}
}

err_t http_parsePostReq(const char* buff){

	if(buff == NULL) return 1;
	char key[TAG_KEY_MAX_SIZE] = {0};
	char value[TAG_VAL_MAX_SIZE] = {0};
	int keyInd = 0;
	int valInd = 0;
	bool keyValid = false;
	size_t buffSize = strlen(buff);
	char s;
	
	for(int i=0;i<buffSize;i++){

		s = buff[i];

		if(s == '=' && keyInd > 0){			/* key is parsed -> start value parsing */
			keyValid = true;
		}else if(s == '&' || i >= buffSize){		/* first key-value pair parsed */
			if(keyValid){							/* key can't be zero */
				Tag::set(key,value);				
			}										/* bad key */
			keyValid = false;
			keyInd = 0;
			valInd = 0;
			strInit(key, sizeof(key));
			strInit(value, sizeof(value));			
		}else if(isSymbol(s)){							/* is symbol */
			if(!keyValid){							/* key parsing */
				if(keyInd >= TAG_KEY_MAX_SIZE) return ESP_ERR_HTTP_POST_KEY_SIZE;
				key[keyInd++] = s;
			}else{									/* value parsing*/
				if(keyInd >= TAG_VAL_MAX_SIZE) return ESP_ERR_HTTP_POST_VAL_SIZE;
				value[valInd++] = s;
			}
		}
	}
	return 0;
}

		/* ========= HTTP hendlers ========= */

/* An HTTP GET handler */
esp_err_t hello_get_handler(httpd_req_t *req)
{
    /* Send response with custom headers and body set as the*/
    ESP_LOGI(TAG, "Sanding html page");
    err_t err = sendHtml(req);
    if (err != ESP_OK) return err;
    return ESP_OK;
}

httpd_uri_t hello = {
    .uri       = "/hello",
    .method    = HTTP_GET,
    .handler   = hello_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = (char*)"Hello World!"
};


		/* ======== Config hendlers ======== */

const http_category_t http_ntp = {
		.title = "NTP configuration",
		.url ="/ntp",
		.menuTitle = "ntp",
		.category = "ntp"
};

const http_category_t http_net = {
		.title = "Net configuration",
		.url ="/net",
		.menuTitle = "Net",
		.category = "Net"
};

esp_err_t cfg_get_handler(httpd_req_t *req){
	http_category_t* ctx = (http_category_t*)req->user_ctx;
	ESP_LOGI(TAG, "Sanding %s page", ctx->category);
	err_t err = sendConfigContent(req, ctx);
	if (err != ESP_OK) return err;
	return ESP_OK;
}

httpd_uri_t ntp_configGet = {
    .uri       = http_ntp.url,
	.method    = HTTP_GET,
    .handler   = cfg_get_handler,
    .user_ctx  = (http_category_t*)&http_ntp
};

httpd_uri_t net_configGet = {
    .uri       = http_net.url,
    .method    = HTTP_GET,
    .handler   = cfg_get_handler,
    .user_ctx  = (http_category_t*)&http_net
};

#define POST_BUFF_LEN 300

esp_err_t cfg_post_handler(httpd_req_t *req){
	char buf[POST_BUFF_LEN];
	    int ret, remaining = req->content_len;

	    if (remaining > 0) {
	        /* Read the data for the request */
	        if ((ret = httpd_req_recv(req, buf,
	                        MIN(remaining, sizeof(buf)))) <= 0) {
	            if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
	                /* Retry receiving if timeout occurred */
	            	;
	            }
	            return ESP_FAIL;
	        }
	        remaining -= ret;


	        /* Log data received */
	        ESP_LOGI(TAG, "=========== RECEIVED DATA ==========");
	        ESP_LOGI(TAG, "%.*s", ret, buf);
	        ESP_LOGI(TAG, "====================================");
	        http_parsePostReq(buf);

	        /* Send back the same data */
	        http_category_t* ctx = (http_category_t*)req->user_ctx;
	        ESP_LOGI(TAG, "Sanding %s page to back",ctx->category);

			err_t err = sendConfigContent(req, ctx);
			if (err != ESP_OK) return err;
	    }

	    return ESP_OK;
}

httpd_uri_t ntp_configPost = {
    .uri       = http_ntp.url,
    .method    = HTTP_POST,
    .handler   = cfg_post_handler,
    .user_ctx  = (http_category_t*)&http_ntp
};

httpd_uri_t net_configPost = {
    .uri       = http_net.url,
    .method    = HTTP_POST,
    .handler   = cfg_post_handler,
    .user_ctx  = (http_category_t*)&http_net
};

	/* ========== Web server init =========== */

httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &hello);
        //httpd_register_uri_handler(server, &echo);
        //httpd_register_uri_handler(server, &ctrl);
        httpd_register_uri_handler(server, &net_configGet);
        httpd_register_uri_handler(server, &net_configPost);
        httpd_register_uri_handler(server, &ntp_configGet);
        httpd_register_uri_handler(server, &ntp_configPost);
        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

void stop_webserver(httpd_handle_t server)
{
    // Stop the httpd server
    httpd_stop(server);
}


static void disconnect_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        ESP_LOGI(TAG, "Stopping webserver");
        stop_webserver(*server);
        *server = NULL;
    }
}


static void connect_handler(void* arg, esp_event_base_t event_base,
                            int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        ESP_LOGI(TAG, "Starting webserver");
        *server = start_webserver();
    }
}


void http_startWebServer(){
	ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
	ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));

	server = start_webserver();
}


