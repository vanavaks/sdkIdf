/*
 * htmlHelper.c
 *
 *  Created on: 20 янв. 2021 г.
 *      Author: Vanavaks
 */

#include "main.h"
#include <sys/param.h>
#include <esp_http_server.h>
#include "htmlHelper.h"
#include "Tag.h"
#include "http.h"
#define TAG "html"
#define html_log_level ESP_LOG_INFO
//#define HTML_DEBUG
#ifdef HTML_DEBUG
#define HTML_LOGD(tag, format, ...) ESP_LOGI(tag, format, ##__VA_ARGS__)
#else
#define HTML_LOGD(tag, format, ...) ESP_LOGD(tag, format, ##__VA_ARGS__)
#endif

#define HTML_TITLE "Boiler controller"

const char* html_html_b ="<!DOCTYPE html>\n \
							<html>\n\
 							<head>\n\
							  <meta charset=\"utf-8\">\n\
							  <title>" HTML_TITLE "</title>\n";


const char* html_style = "<style>\n\
   body {\n\
    font: 11pt Arial, Helvetica, sans-serif; /* Рубленый шрифт текста */\n\
    margin: 0; /* Отступы на странице */\n\
   }\n\
   h1 {\n\
    font-size: 36px; /* Размер шрифта */\n\
    margin: 0; /* Убираем отступы */\n\
    color: #fc6; /* Цвет текста */\n\
   }\n\
   h2 {\n\
    margin-top: 0; /* Убираем отступ сверху */\n\
   }\n\
   #header { /* Верхний блок */\n\
    background: #0080c0; /* Цвет фона */\n\
    padding: 10px; /* Поля вокруг текста */\n\
   }\n\
   #sidebar { /* Левая колонка */\n\
    float: left; /* Обтекание справа */\n\
    border: 1px solid #333; /* Параметры рамки вокруг */\n\
    width: 20%; /* Ширина колонки */\n\
    padding: 5px; /* Поля вокруг текста */\n\
    margin: 10px 10px 20px 5px; /* Значения отступов */\n\
   }\n\
   #content { /* Правая колонка */\n\
    margin: 10px 5px 20px 25%; /* Значения отступов */\n\
    padding: 5px; /* Поля вокруг текста */\n\
    border: 1px solid #333; /* Параметры рамки */\n\
   }\n\
   #footer { /* Нижний блок */\n\
    background: #333; /* Цвет фона */\n\
    padding: 5px; /* Поля вокруг текста */\n\
    color: #fff; /* Цвет текста */\n\
    clear: left; /* Отменяем действие float */\n\
   }\n\
  </style>\n";

const char* html_header_e =  "</head>\n <body>\n";
const char* html_html_e =  "</body>\n</html>\n";

#define HTML_HEADER HTML_TITLE
#define HTML_FOOTER "Vanavaks Digital"
const char* html_header = "<div id=\"header\"><h1>" HTML_HEADER "</h1></div>";
const char* html_footer = "<div id=\"footer\">&copy;" HTML_FOOTER "</div>";

const char* html_sidebar = "<div id=\"sidebar\">\
							<p><a href=\"hello\">Start page</a></p>\
							<p><a href=\"b_author.html\">Config</a></p>\
							<p><a href=\"ntp\">Ntp config</a></p>\
							<p><a href=\"net\">Net config</a></p>\
							<p><a href=\"tlg\">Telegram config</a></p>\\
							<p><a href=\"io\">IO</a></p>\\
							<p><a href=\"b_last.html\">Info</a></p>\
							</div>";

const char* html_context = "<h2>Some information </h2>  <p>As part of the esp-open-rtos build process, all binary SDK symbols are \
		prefixed with sdk_. This makes it easier to differentiate binary & open source code, and also \
		prevents namespace conflicts.Espressifs RTOS SDK provided a \"libssl\" based on axTLS. This has \
		been replaced with the more up to date mbedTLS library (see below). Some binary libraries appear \
		to contain unattributed open source code: </p>";

const char* html_net_context = "<h2>Network information </h2>  <p>Here will been network parameters</p>";
const char* html_form_post = "<form action=\"net\" method=\"POST\">";
const char* html_but_save = "<input type=\"submit\" class = \"buttonFlp\"  value=\"Сохранить\"></form>";
const char* html_end_form = "</form>";

void http_createNetParsList(char* buff, char* category);

void http_createParsList(char* buff,const char* category);
err_t sendStyle(httpd_req_t *req){
	esp_err_t err;
	if((err = httpd_resp_send_chunk(req, html_style, strlen(html_style))) !=ESP_OK) return err;
	return ESP_OK;
}

err_t sendSideBar(httpd_req_t *req){
	esp_err_t err;
	if((err = httpd_resp_send_chunk(req, html_sidebar, strlen(html_sidebar))) !=ESP_OK) return err;
	return ESP_OK;
}

err_t sendContent(httpd_req_t *req){
	esp_err_t err;
		if((err = httpd_resp_send_chunk(req, html_context, strlen(html_context))) !=ESP_OK) return err;
	return ESP_OK;
}/*
#ifdef __cplusplus
extern "C" {
#endif*/

err_t sendHtml(httpd_req_t *req){
	esp_err_t err;

	if((err = httpd_resp_send_chunk(req, html_html_b, strlen(html_html_b))) !=ESP_OK) return err;
	if((err = sendStyle(req)) !=ESP_OK) return err;
	if((err = httpd_resp_send_chunk(req, html_header, strlen(html_header))) !=ESP_OK) return err;

	if((err = sendSideBar(req)) !=ESP_OK) return err;
	if((err = sendContent(req)) !=ESP_OK) return err;

	if((err = httpd_resp_send_chunk(req, html_footer, strlen(html_footer))) !=ESP_OK) return err;
	if((err = httpd_resp_send_chunk(req, html_html_e, strlen(html_html_e))) !=ESP_OK) return err;
	return ESP_OK;
}
/*
#define buffSize 2048
char* buffer[buffSize]{0};
void initBuff(char* buff, uint16_t size){
	memset (buff, '\0', size);
}
*/
err_t sendNetHtml(httpd_req_t *req){
	esp_err_t err;

	if((err = httpd_resp_send_chunk(req, html_html_b, strlen(html_html_b))) !=ESP_OK) return err;
	if((err = sendStyle(req)) !=ESP_OK) return err;
	if((err = httpd_resp_send_chunk(req, html_header, strlen(html_header))) !=ESP_OK) return err;

	if((err = sendSideBar(req)) !=ESP_OK) return err;

	if((err = httpd_resp_send_chunk(req, html_form_post, strlen(html_form_post))) !=ESP_OK) return err;

	char buff[2048]{0}; //-------проверять размер буфера и увеличивать при необходимости
	strcat(buff, "<div><h2>Network information </h2></div>");
	http_createParsList(buff,"Net");
	if((err = httpd_resp_send_chunk(req, buff, strlen(buff))) !=ESP_OK) return err;
	if((err = httpd_resp_send_chunk(req, html_but_save, strlen(html_but_save))) !=ESP_OK) return err;

	if((err = httpd_resp_send_chunk(req, html_footer, strlen(html_footer))) !=ESP_OK) return err;
	if((err = httpd_resp_send_chunk(req, html_html_e, strlen(html_html_e))) !=ESP_OK) return err;
	return ESP_OK;
}

const char* cnt = "<div id=\"content\">";
const char* cntEnd = "</div>";

err_t sendConfigContent(httpd_req_t *req, http_category_t* cat){ //char* title, const char* category, const char* postUrl){
	esp_err_t err;
	if((err = httpd_resp_send_chunk(req, html_html_b, strlen(html_html_b))) !=ESP_OK) return err;
	if((err = sendStyle(req)) !=ESP_OK) return err;
	if((err = httpd_resp_send_chunk(req, html_header, strlen(html_header))) !=ESP_OK) return err;

	if((err = sendSideBar(req)) !=ESP_OK) return err;

	char buff[1524]{0}; //-------проверять размер буфера и увеличивать при необходимости
	if((err = httpd_resp_send_chunk(req, cnt, strlen(cnt))) !=ESP_OK) return err;
	strcat(buff, "<form action=\"");
	strcat(buff, cat->url);
	strcat(buff, "\" method=\"POST\"> <div><h2> ");
	strcat(buff, cat->title);
	strcat(buff, " </h2></div>");
	http_createParsList(buff, cat->category);
	if((err = httpd_resp_send_chunk(req, buff, strlen(buff))) !=ESP_OK) return err;
	if((err = httpd_resp_send_chunk(req, html_but_save, strlen(html_but_save))) !=ESP_OK) return err;
	if((err = httpd_resp_send_chunk(req, cntEnd, strlen(cntEnd))) !=ESP_OK) return err;

	if((err = httpd_resp_send_chunk(req, html_footer, strlen(html_footer))) !=ESP_OK) return err;
	if((err = httpd_resp_send_chunk(req, html_html_e, strlen(html_html_e))) !=ESP_OK) return err;
	return ESP_OK;
}

#define HTML_FORM_BUFF_SIZE 2048 //1524
/*
err_t html_sendForm(httpd_req_t *req, const char* url, const char* title, const char* category, const char* butName){
	esp_err_t err;
	char buff[HTML_FORM_BUFF_SIZE]{0};
	strcat(buff, "<form action=\"");
	strcat(buff, url);
	strcat(buff, "\" method=\"POST\"> <div><h2> ");
	strcat(buff, title);
	strcat(buff, " </h2></div>");
	http_createParsList(buff, category);
	if (butName != NULL){
		strcat(buff, "<input type=\"submit\" class = \"buttonFlp\"  value=\"");
		strcat(buff, butName);
		strcat(buff, "\">");
	}
	strcat(buff, "</form>");
	if((err = httpd_resp_send_chunk(req, buff, strlen(buff))) !=ESP_OK) return err;
	return ESP_OK;
}*/

/* if butName == NULL -> commit button will not generated */
typedef struct{
	const char* url;
	const char* title;
	const char* category;
	const char* butName = NULL;
}html_form_t;

/* form buffer is to mach in future try to send form partically */
err_t html_sendForm(httpd_req_t *req, const html_form_t* data){
	esp_err_t err;
	char buff[HTML_FORM_BUFF_SIZE]{0};
	strcat(buff, "<form action=\"");
	strcat(buff, data->url);
	strcat(buff, "\" method=\"POST\"> <div><h2> ");
	strcat(buff, data->title);
	strcat(buff, " </h2></div>");
	http_createParsList(buff, data->category);
	if (data->butName != NULL){
		strcat(buff, "<input type=\"submit\" class = \"buttonFlp\"  value=\"");
		strcat(buff, data->butName);
		strcat(buff, "\">");
	}
	strcat(buff, "</form>");
	if((err = httpd_resp_send_chunk(req, buff, strlen(buff))) !=ESP_OK) return err;
	return ESP_OK;
}

/* form buffer is to mach in future try to send form partically */
err_t html_sendForm(httpd_req_t *req, http_category_t* data){
	esp_err_t err;
	char buff[HTML_FORM_BUFF_SIZE]{0};
	strcat(buff, "<form action=\"");
	strcat(buff, data->url);
	strcat(buff, "\" method=\"POST\"> <div><h2> ");
	strcat(buff, data->title);
	strcat(buff, " </h2></div>");
	http_createParsList(buff, data->category);
	if (data->butName != NULL){
		strcat(buff, "<input type=\"submit\" class = \"buttonFlp\"  value=\"");
		strcat(buff, data->butName);
		strcat(buff, "\">");
	}
	strcat(buff, "</form>");
	if((err = httpd_resp_send_chunk(req, buff, strlen(buff))) !=ESP_OK) return err;
	return ESP_OK;
}
#if 0
err_t sendIOContent(httpd_req_t *req){ //char* title, const char* category, const char* postUrl){
	esp_err_t err;
	if((err = httpd_resp_send_chunk(req, html_html_b, strlen(html_html_b))) !=ESP_OK) return err;
	if((err = sendStyle(req)) !=ESP_OK) return err;
	if((err = httpd_resp_send_chunk(req, html_header, strlen(html_header))) !=ESP_OK) return err;

	if((err = sendSideBar(req)) !=ESP_OK) return err;

	const html_form_t di_data = {.url = "/di", .title = "Digital inputs", .category = "DI"};
	html_sendForm(req, &di_data);

	const html_form_t do_data = {.url = "/do", .title = "Digital outputs", .category = "DO", .butName = "setDO"};
	html_sendForm(req, &do_data);

	const html_form_t ai_data = {.url = "/ai", .title = "Analog inputs", .category = "AI"};
	html_sendForm(req, &ai_data);

	const html_form_t ao_data = {.url = "/ao", .title = "Analog outputs", .category = "AO", .butName = "setAO"};
	html_sendForm(req, &ao_data);

	const html_form_t tempr_data = {.url = "/tempr", .title = "Analog outputs", .category = "tempr"};
	html_sendForm(req, &ao_data);


	if((err = httpd_resp_send_chunk(req, html_footer, strlen(html_footer))) !=ESP_OK) return err;
	if((err = httpd_resp_send_chunk(req, html_html_e, strlen(html_html_e))) !=ESP_OK) return err;
	return ESP_OK;
}
#endif


// TODO buttons and sliders for tag



err_t html_sendConfigContent(httpd_req_t *req, http_category_t* cat, uint8_t num){
	esp_err_t err;
	if((err = httpd_resp_send_chunk(req, html_html_b, strlen(html_html_b))) !=ESP_OK) return err;
	if((err = sendStyle(req)) !=ESP_OK) return err;
	if((err = httpd_resp_send_chunk(req, html_header, strlen(html_header))) !=ESP_OK) return err;

	if((err = sendSideBar(req)) !=ESP_OK) return err;

	if((err = httpd_resp_send_chunk(req, cnt, strlen(cnt))) !=ESP_OK) return err;

	for (int i=0; i<num; i++){
		html_sendForm(req, &cat[i]);
	}

	if((err = httpd_resp_send_chunk(req, cntEnd, strlen(cntEnd))) !=ESP_OK) return err;

	return ESP_OK;
}
/*
err_t sendNtpHtml(httpd_req_t *req){
	esp_err_t err;
	return ESP_OK;
}*/

void html_webVisElementHeader(char* buff, const char* style, const char* label){
	strcat(buff,"<td>");
	strcat(buff,label);
	strcat(buff,"</td>");
}

char* html_webInputFieldElement(char* buff, const char* style, const char* label, const char* value, const char* name, const char* maxlength){	//1507
	html_webVisElementHeader(buff, style,  label);
	strcat(buff,"<td><input type=\"text\" name=\"");
	strcat(buff,name);
	strcat(buff,"\" value=\"");
	strcat(buff,value);
	strcat(buff,"\"  maxlength=\"");
	strcat(buff,maxlength);
	strcat(buff,"\" ></td>");
	return buff;
}

#if 0

void html_webVisElementHeader(char* buff, const char* style, const char* label){
	strcat(buff,"<div");
	if (strlen(style) > 0) {
		strcat(buff," class=\"");
		strcat(buff,style);
		strcat(buff,"\"");
	}
	strcat(buff,"> <p>");
	strcat(buff,label);
	strcat(buff,"   ");
}

char* html_webInputFieldElement(char* buff, const char* style, const char* label, const char* value, const char* name, const char* maxlength){	//1507
	html_webVisElementHeader(buff, style,  label);
	strcat(buff,"<input type=\"text\" name=\"");
	strcat(buff,name);
	strcat(buff,"\" value=\"");
	strcat(buff,value);
	strcat(buff,"\"  maxlength=\"");
	strcat(buff,maxlength);
	strcat(buff,"\" ></p></div>");
	return buff;
}
#endif

char* html_webCheckBoxElement1(char* buff, char* style, const char* label, bool value, const char* name){
	strcat(buff,"<div");
	//size_t *size;
	//size = strlen(style);
	if (strlen(style) > 0) {
		strcat(buff," class=\"");
		strcat(buff,style);
		strcat(buff,"\"");
	}
	strcat(buff,"> <label><input type=\"checkbox\" id=\"");
	strcat(buff,name);
	strcat(buff,"1\"");

	if(value) { strcat(buff," checked ");}
	strcat(buff,"/>");
	strcat(buff,label);
	strcat(buff,"</label><input type=\"hidden\" name=\"");
	//strcat(buff,name);
	strcat(buff,"\" id=\"");
	strcat(buff,name);
	strcat(buff,"2");
	strcat(buff,"\" value=\"");
	if(value) {strcat(buff, "on");} else { strcat(buff, "off");}
	strcat(buff, "\"><script> var el");
	strcat(buff,name);
	strcat(buff, " = document.querySelector('#");
	strcat(buff,name);
	strcat(buff,"1");
	strcat(buff,"'); el");
	strcat(buff,name);
	strcat(buff, ".onclick = function() {if (el");
	strcat(buff,name);
	strcat(buff, ".checked) { document.getElementById('");
	strcat(buff,name);
	strcat(buff,"2");
	strcat(buff,"').value ='on'; } else { document.getElementById('");
	strcat(buff,name);
	strcat(buff,"2");
	strcat(buff,"').value ='off';}} </script> </div>");
	return buff;
}

char* html_webCheckBoxElement(char* buff, char* style, const char* label, bool value, const char* name){
	strcat(buff,"<td>");
	strcat(buff,name);
	strcat(buff,"</td>");

	strcat(buff,"<td>");

	strcat(buff,"<label><input type=\"checkbox\" id=\"");
	strcat(buff,name);
	strcat(buff,"1\"");

	if(value) { strcat(buff," checked ");}
	strcat(buff,"/>");
	strcat(buff,label);
	strcat(buff,"</label><input type=\"hidden\" name=\"");
	strcat(buff,name);
	strcat(buff,"\" id=\"");
	strcat(buff,name);
	strcat(buff,"2");
	strcat(buff,"\" value=\"");
	if(value) {strcat(buff, "on");} else { strcat(buff, "off");}
	strcat(buff, "\"><script> var el");
	strcat(buff,name);
	strcat(buff, " = document.querySelector('#");
	strcat(buff,name);
	strcat(buff,"1");
	strcat(buff,"'); el");
	strcat(buff,name);
	strcat(buff, ".onclick = function() {if (el");
	strcat(buff,name);
	strcat(buff, ".checked) { document.getElementById('");
	strcat(buff,name);
	strcat(buff,"2");
	strcat(buff,"').value ='on'; } else { document.getElementById('");
	strcat(buff,name);
	strcat(buff,"2");
	strcat(buff,"').value ='off';}} </script> </div>");
	strcat(buff,"</td>");
	return buff;
}

char* html_webTextElement(char* buff, const char* style, const char* label, const char* value){
	ESP_LOGI("html","webText element");
	html_webVisElementHeader(buff, style,  label);
	strcat(buff, "<td>");
	strcat(buff, value);
	strcat(buff, "</td>");
	return buff;
}


// visualiz eparameters by config? may be in parConfig?
void http_createParsList(char* buff,const char* category){

	Tag* tag = NULL;
	uint16_t tags = Tag::getTagNumb();

	HTML_LOGD(TAG,"Creating tag list for %s category", category);
	strcat(buff,"<table border=\"1\">");
	for(uint16_t index = 0; index < tags; index++){
		tag = Tag::getNextByCategory(&index, category);
		if(tag == NULL) continue;
		HTML_LOGD(TAG,"Tag %d isnt null", index);
		strcat(buff,"<tr>");
		if(tag->isSaveble()){								//so->is editable (input field)
			HTML_LOGD("html","Tag saveble");
			if(tag->getType() == TAG_STR){					//text input field
				HTML_LOGD(TAG,"string inputField");
				html_webInputFieldElement(buff, "", tag->getKey(), tag->getStr(), tag->getKey(), TAG_VAL_STR_MAX_SIZE_STR);
			}else if(tag->getType() == TAG_UI32){
				HTML_LOGD(TAG,"ui32 inputField");
				char b[15]{0};
				itoa(tag->getUI32(),b,10);
				HTML_LOGD(TAG,"value -'%s'",b);
				html_webInputFieldElement(buff, "", tag->getKey(), b, tag->getKey(), TAG_VAL_STR_MAX_SIZE_STR);
			}else if(tag->getType() == TAG_BOOL){
				bool v = tag->getBL();
				HTML_LOGD(TAG,"bool value -%d", v);
				html_webCheckBoxElement(buff, "", tag->getKey(), v, tag->getKey());
			}else{
				ESP_LOGW(TAG,"Unhandled tag type %d", tag->getType());
			}
		}
		else{	//text
			if(tag->getType() == TAG_STR){	//text field
				html_webTextElement(buff, "", tag->getKey(), tag->getStr());
			}else if(tag->getType() == TAG_UI32){
				char b[15]{0};
				itoa(tag->getUI32(),b,10);
				html_webTextElement(buff, "", tag->getKey(), b);
			}else if(tag->getType() == TAG_BOOL){
				bool v = tag->getBL();
				HTML_LOGD(TAG,"bool value -%d", v);
				html_webCheckBoxElement(buff, "", tag->getKey(), v, tag->getKey());
			}else{
				ESP_LOGW(TAG,"Unhandled tag type %d", tag->getType());
			}
		}
		strcat(buff,"</tr>");
	}
	strcat(buff,"</table>");
}

void http_createNetParsList(char* buff){
	//initBuff(buff, buffSize);
	strcat(buff, "<div><h2>Network information </h2></div>");
	http_createParsList(buff,"Net");
}

void parsePostWebServerRequest(char* buff){		//838, 815

}

void html_init(){
	esp_log_level_set(TAG, html_log_level);
}
