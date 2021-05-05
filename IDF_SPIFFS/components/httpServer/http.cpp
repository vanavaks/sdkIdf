/*
 * http.c
 *
 *  Created on: 13 џэт. 2021 у.
 *      Author: Vanavaks
 *
 *      class uri handler
 *
 *      handler
 *      URI handler structure
 *      register (get uri)
 *
 *
 *      title
 *      categorys list
 *
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
#define TAG_VAL_MAX_SIZE  64
#define HTTP_DEBUG_LEVEL ESP_LOG_INFO
#define ESP_ERR_HTTP_POST_KEY_SIZE 0x01
#define ESP_ERR_HTTP_POST_VAL_SIZE 0x02

static const char *http_tag="HTTP";
static httpd_handle_t server = NULL;


		/* ======== POST parsing ======== */

bool isSymbol(char c){
	if(c >= ' ') return true;
	if(c >= '0' && c <= '9') return true;
	if(c >= 'A' && c <= 'Z') return true;
	if(c >= 'a' && c <= 'z') return true;
	if(c == '_') return true;
	if(c == '%') return true;
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
	size_t buffSize = strlen(buff)+1;
	char s;
	
	for(int i=0;i<buffSize;i++){

		s = buff[i];

		if(s == '=' && keyInd > 0){			/* key is parsed -> start value parsing */
			keyValid = true;
			ESP_LOGI(http_tag, "post key parsed");
		}else if(s == '&' || i >= buffSize-1 || s == 0 || s == 'CR' || s == 'LF'){		/* first key-value pair parsed */
			if(keyValid){							/* key can't be zero */

				Tag::set(key,value);				
			}										/* bad key */
			keyValid = false;
			keyInd = 0;
			valInd = 0;
			strInit(key, sizeof(key));
			strInit(value, sizeof(value));			
		}else if(isSymbol(s)){							/* is symbol */
			if(s == '%'){								/* ASCII symbol */
				char symv[3]{0};
				symv[0] = buff[++i];
				symv[1] = buff[++i];
				s = (char)strtol(symv,NULL,16);
			}
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
    ESP_LOGI(http_tag, "Sanding html page");
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

const http_category_t http_tlg = {
		.title = "Telegram configuration",
		.url ="/tlg",
		.menuTitle = "Tlg",
		.category = "Tlg"
};

const http_category_t http_DO = {
		.title = "Digital outputs",
		.url ="/DO",
		.menuTitle = "IO",
		.category = "DO"
};

const http_category_t http_AO = {
		.title = "Analog outputs",
		.url ="/AO",
		.menuTitle = "IO",
		.category = "AO"
};


esp_err_t cfg_get_handler(httpd_req_t *req){
	http_category_t* ctx = (http_category_t*)req->user_ctx;
	ESP_LOGI(http_tag, "Sanding %s page", ctx->category);
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

httpd_uri_t tlg_configGet = {
    .uri       = http_tlg.url,
    .method    = HTTP_GET,
    .handler   = cfg_get_handler,
    .user_ctx  = (http_category_t*)&http_tlg
};

#define POST_BUFF_LEN 300

esp_err_t cfg_post_handler(httpd_req_t *req){
	char buf[POST_BUFF_LEN]{0};
	    int ret, remaining = req->content_len;

	    if (remaining > 0) {

	    	//strInit(buf, POST_BUFF_LEN);
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
	        ESP_LOGI(http_tag, "=========== RECEIVED DATA ==========");
	        ESP_LOGI(http_tag, "%.*s", ret, buf);
	        ESP_LOGI(http_tag, "====================================");
	        http_parsePostReq(buf);

	        /* Send back the same data */
	        http_category_t* ctx = (http_category_t*)req->user_ctx;
	        ESP_LOGI(http_tag, "Sanding %s page to back",ctx->category);

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

httpd_uri_t tlg_configPost = {
    .uri       = http_tlg.url,
    .method    = HTTP_POST,
    .handler   = cfg_post_handler,
    .user_ctx  = (http_category_t*)&http_tlg
};


	/* ========== IO PAGE =================== */

esp_err_t http_cfg_get_handler(httpd_req_t *req){
	http_category_t* ctx = (http_category_t*)req->user_ctx;
	ESP_LOGI(http_tag, "Sanding %s page", ctx->category);
	//ESP_LOGI(http_tag, "Sanding 'io' page");
	err_t err = html_sendConfigContent(req, ctx, 5);
	if (err != ESP_OK) return err;
	return ESP_OK;
}

esp_err_t http_cfg_post_handler(httpd_req_t *req){
	char buf[POST_BUFF_LEN]{0};
	    int ret, remaining = req->content_len;

	    if (remaining > 0) {

	    	//strInit(buf, POST_BUFF_LEN);
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
	        ESP_LOGI(http_tag, "=========== RECEIVED DATA ==========");
	        ESP_LOGI(http_tag, "%.*s", ret, buf);
	        ESP_LOGI(http_tag, "====================================");
	        http_parsePostReq(buf);

	        /* Send back the same data */
	        http_category_t* ctx = (http_category_t*)req->user_ctx;
	        ESP_LOGI(http_tag, "Sanding %s page to back",ctx->category);

			err_t err = html_sendConfigContent(req, ctx, 5);
			if (err != ESP_OK) return err;
	    }

	    return ESP_OK;
}
#if 0
const http_category_t IO_cats[] = {
		{.url = "/di", .title = "Digital inputs", .category = "DI", .butName = "setDO", .menuTitle = ""},
		{.url = "/do", .title = "Digital outputs", .category = "DO", .butName = "setDO", .menuTitle = ""},
		{.url = "/ai", .title = "Analog inputs", .category = "AI", .butName = "setDO", .menuTitle = ""},
		{.url = "/ao", .title = "Analog outputs", .category = "AO", .butName = "setAO", .menuTitle = ""},
		{.url = "/tempr", .title = "Temperature sensors", .category = "tempr", .butName = "setDO", .menuTitle = ""}
};
#endif

//TODO to relize transfer of categorys numbers to html helper (struct)
const http_category_t IO_cats[] = {
		{"Digital inputs", "/io", "", "DI", "setDi"},
		{"Digital outputs", "/io", "", "DO", "setDO"},
		{"Analog inputs", "/io", "", "AI", "set Analog Input"},
		{"Analog outputs", "/io", "", "AO", "setAO"},
		{"Temperature sensors", "/io", "", "tempr", NULL}
};

const httpd_uri_t io_configGet = {
    .uri       = "/io",
	.method    = HTTP_GET,
    .handler   = http_cfg_get_handler,
	.user_ctx  = (http_category_t*)&IO_cats
};

const httpd_uri_t io_configPost = {
    .uri       = "/io",
	.method    = HTTP_POST,
    .handler   = http_cfg_post_handler,
	.user_ctx  = (http_category_t*)&IO_cats
};

const httpd_uri_t do_configPost = {
    .uri       = http_DO.url,
    .method    = HTTP_POST,
    .handler   = cfg_post_handler,
    .user_ctx  = (http_category_t*)&IO_cats
};

const httpd_uri_t ao_configPost = {
    .uri       = http_AO.url,
    .method    = HTTP_POST,
    .handler   = cfg_post_handler,
    .user_ctx  = (http_category_t*)&IO_cats
};

	/* ========== Web server init =========== */

httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    config.max_uri_handlers = 16;

    	// Start the httpd server
    esp_log_level_set(http_tag, HTTP_DEBUG_LEVEL);
    html_init();
    ESP_LOGI(http_tag, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        	// Set URI handlers
        ESP_LOGI(http_tag, "Registering URI handlers");
        httpd_register_uri_handler(server, &hello);
        httpd_register_uri_handler(server, &net_configGet);
        httpd_register_uri_handler(server, &net_configPost);
        httpd_register_uri_handler(server, &ntp_configGet);
        httpd_register_uri_handler(server, &ntp_configPost);
        httpd_register_uri_handler(server, &tlg_configGet);
        httpd_register_uri_handler(server, &tlg_configPost);
        httpd_register_uri_handler(server, &io_configGet);
        httpd_register_uri_handler(server, &io_configPost);
        //httpd_register_uri_handler(server, &do_configPost);
        //httpd_register_uri_handler(server, &ao_configPost);
        return server;
    }

    ESP_LOGI(http_tag, "Error starting server!");
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
        ESP_LOGI(http_tag, "Stopping webserver");
        stop_webserver(*server);
        *server = NULL;
    }
}


static void connect_handler(void* arg, esp_event_base_t event_base,
                            int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        ESP_LOGI(http_tag, "Starting webserver");
        *server = start_webserver();
    }
}


void http_startWebServer(){
	ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
	ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));

	server = start_webserver();
}

void http_stopWebServ(){
	ESP_LOGI(http_tag,"Heap before deleting web server%d\n", esp_get_free_heap_size());
	stop_webserver(server);
	ESP_LOGI(http_tag,"Heap after deleting web server%d\n", esp_get_free_heap_size());
}

void http_strtWebServ(){
	ESP_LOGI(http_tag,"Heap before creating web server%d\n", esp_get_free_heap_size());
	server = start_webserver();
	ESP_LOGI(http_tag,"Heap after creating web server%d\n", esp_get_free_heap_size());
}

