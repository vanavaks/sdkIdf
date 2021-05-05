/* Common functions for protocol examples, to establish Wi-Fi or Ethernet connection.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
 */

#include <string.h>
#include "wifi_connect.h"

#include "sdkconfig.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event_loop.h"
#include "tcpip_adapter.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/ip4_addr.h"
#include "Tag.h"


#define GOT_IPV4_BIT BIT(0)
#define GOT_IPV6_BIT BIT(1)

#ifdef CONFIG_EXAMPLE_CONNECT_IPV6
#define CONNECTED_BITS (GOT_IPV4_BIT | GOT_IPV6_BIT)
#if defined(CONFIG_EXAMPLE_CONNECT_IPV6_PREF_LOCAL_LINK)
#define EXAMPLE_CONNECT_PREFERRED_IPV6_TYPE 0
#elif defined(CONFIG_EXAMPLE_CONNECT_IPV6_PREF_GLOBAL)
#define EXAMPLE_CONNECT_PREFERRED_IPV6_TYPE 1
#endif
#else
#define CONNECTED_BITS (GOT_IPV4_BIT)
#endif

static EventGroupHandle_t s_connect_event_group;
static ip4_addr_t s_ip_addr;
static bool wifi_defConfig = false;

static wifi_config_t wifi_config = { 0 }; //must be local
static bool wifi_connected = false;
static bool wifi_has_ip = false;

#if 0
#define CONFIG_EXAMPLE_WIFI_SSID "vanavaks"
#define CONFIG_EXAMPLE_WIFI_PASSWORD "programmer"
#else
#define CONFIG_EXAMPLE_WIFI_SSID		"asumdf"
#define CONFIG_EXAMPLE_WIFI_PASSWORD 	"sipartsipart"
#endif
#define STATIC_IP		"192.168.0.222"
#define SUBNET_MASK		"255.255.255.0"
#define GATE_WAY		"192.168.0.1"
#define DNS_SERVER		"8.8.8.8"

static const char * tag = "wifi";

#define EN_STAT_IP

const char defSsid[] = "asumdf";
const char defPass[] = "sipartsipart";

const tagProp_t prop_WIFI_ssid1 = Pstr("WIFI_ssid1","Net",SAVEBLE,"vanavaks");
const tagProp_t prop_WIFI_ssid2 = Pstr("WIFI_ssid2","Net",SAVEBLE,"asumdf");
const tagProp_t prop_WIFI_pass1 = Pstr("WIFI_pass_1","Net",SAVEBLE,"programmer");
const tagProp_t prop_WIFI_pass2 = Pstr("WIFI_pass_2","Net",SAVEBLE,"sipartsipart");

const tagProp_t prop_WIFI_IP = 		Pstr("WIFI_IP","Net",SAVEBLE,"192.168.0.222");
const tagProp_t prop_WIFI_mask = 	Pstr("WIFI_mask","Net",SAVEBLE,"255.255.255.0");
const tagProp_t prop_WIFI_gw = 		Pstr("WIFI_gw","Net",SAVEBLE,"192.168.0.1");
const tagProp_t prop_WIFI_DNS = 	Pstr("WIFI_DNS","Net",SAVEBLE,"8.8.8.8");

Tag* tag_WIFI_ssid1{0};
Tag* tag_WIFI_ssid2{0};
Tag* tag_WIFI_pass1{0};
Tag* tag_WIFI_pass2{0};

Tag* tag_WIFI_IP{0};
Tag* tag_WIFI_mask{0};
Tag* tag_WIFI_gw{0};
Tag* tag_WIFI_DNS{0};

static char s_connection_name[32] = CONFIG_EXAMPLE_WIFI_SSID;
static char s_connection_passwd[32] = CONFIG_EXAMPLE_WIFI_PASSWORD;

#ifdef CONFIG_EXAMPLE_CONNECT_IPV6
static ip6_addr_t s_ipv6_addr;
#endif

static const char *TAG = "example_connect";

static void strInit(char* s, uint8_t len){
	if(s == NULL) return;
	for(int i = 0; i<len;i++){
		*s = NULL;
		s++;
	}
}



static void wifi_start( Tag* ssid, Tag* pass){
	strInit((char *)&wifi_config.sta.ssid, 32);
	strInit((char *)&wifi_config.sta.password, 32);

		/* setting of static network parameters */
#if 0
	tcpip_adapter_ip_info_t tcp_info = {/*.ip = 0, .netmask = 0, .gw = 0*/};


	IP4_ADDR(&tcp_info.ip, 192, 168, 0, 105);
	IP4_ADDR(&tcp_info.netmask, 255,255,255,0);
	IP4_ADDR(&tcp_info.gw, 192,168,0,1);

	tcp_info.ip.addr = 0xC0A80069;
	tcp_info.netmask.addr = 0xffffff00;
	tcp_info.gw.addr = 0xC0A80001;
#endif


#if 0
	/* checking dhcp status must be stoped */
tcpip_adapter_dhcp_status_t dhcpcStatus, dhcpsStatus;
esp_err_t e;
tcpip_adapter_dhcps_get_status(TCPIP_ADAPTER_IF_STA, &dhcpsStatus);
ESP_LOGI(TAG,"DHCPS status - %d", dhcpsStatus);
if(dhcpsStatus == TCPIP_ADAPTER_DHCP_INIT){
	e = tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_STA);
	ESP_LOGI(TAG,"starting DHCPS... Result - %d", e);
}
if (dhcpsStatus == TCPIP_ADAPTER_DHCP_STARTED ){
	e = tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_STA);
	ESP_LOGI(TAG,"DHCPS was started - stopping...  Result - %d", e);
	tcpip_adapter_dhcps_get_status(TCPIP_ADAPTER_IF_STA, &dhcpsStatus);
}

tcpip_adapter_dhcpc_get_status(TCPIP_ADAPTER_IF_AP, &dhcpcStatus);
ESP_LOGI(TAG,"DHCPC status - %d", dhcpcStatus);
if(dhcpcStatus == TCPIP_ADAPTER_DHCP_INIT){
	e = tcpip_adapter_dhcpc_start(TCPIP_ADAPTER_IF_AP);
	ESP_LOGI(TAG,"starting DHCPC... Result - %d", e);
}
if (dhcpcStatus == TCPIP_ADAPTER_DHCP_STARTED){
	esp_err_t e = tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_AP);
	ESP_LOGI(TAG,"DHCPC was started - stopping... \r\n Result - %d", e);
	tcpip_adapter_dhcps_get_status(TCPIP_ADAPTER_IF_AP, &dhcpcStatus);
}

//	ESP_LOGI(TAG,"Stoping DHCP .");
//	uint64_t i = 0;
//	while(dhcpcStatus == TCPIP_ADAPTER_DHCP_STARTED || dhcpcStatus == TCPIP_ADAPTER_DHCP_STARTED){
//		if(i % 100000 == 0) printf(".");
//		if(i++ > 2000000000) break;
//
//	}

if((dhcpcStatus == TCPIP_ADAPTER_DHCP_STOPPED && dhcpsStatus == TCPIP_ADAPTER_DHCP_STOPPED)){
	ESP_LOGI(TAG,"====== seting static net config ======");
	ESP_ERROR_CHECK(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA,&tcp_info));
	ESP_LOGI(TAG,"====== seting static net config ======");
}
#endif


	//ESP_LOGI(TAG,"IP = %x", (uint32_t)tcp_info.ip);
	//ESP_LOGI(TAG,"Mask = %x", (uint32_t)tcp_info.netmask);
	//ESP_LOGI(TAG,"Hateway = %x", (uint32_t)tcp_info.gw);

	if(ssid != NULL){
		strncpy((char *)&wifi_config.sta.ssid, ssid->getStr(), ssid->size());
	}else{
		size_t s1 = sizeof(defSsid);
		strncpy((char *)&wifi_config.sta.ssid, defSsid, s1);
	}

	if(pass != NULL){
			strncpy((char *)&wifi_config.sta.password, pass->getStr(), pass->size());
	}else{
		size_t s2 = sizeof(defPass);
		strncpy((char *)&wifi_config.sta.password, defPass, s2);
	}

    ESP_LOGI(TAG, "Connecting to %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());
}


static void on_wifi_disconnect(void *arg, esp_event_base_t event_base,int32_t event_id, void *event_data){

    system_event_sta_disconnected_t *event = (system_event_sta_disconnected_t *)event_data;

    wifi_connected = false;

    ESP_LOGI(TAG, "Wi-Fi disconnected, trying to reconnect...");
    if (event->reason == WIFI_REASON_BASIC_RATE_NOT_SUPPORT) {
        /*Switch to 802.11 bgn mode */
        esp_wifi_set_protocol(ESP_IF_WIFI_STA, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N);
    }

    if (!wifi_defConfig){
    	ESP_LOGI(tag,"Trying to connect with default  config: \n ssid -%s, pass - %s", wifi_config.sta.ssid, wifi_config.sta.password);
		wifi_start(tag_WIFI_ssid1, tag_WIFI_pass1);
		wifi_defConfig = true;
    }else{
		ESP_LOGI(tag,"Trying to connect with alternate config: \n ssid -%s, pass - %s", wifi_config.sta.ssid, wifi_config.sta.password);
		wifi_start(tag_WIFI_ssid2, tag_WIFI_pass2);
		wifi_defConfig = false;
	}
}


static void on_wifi_connect(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data){
	wifi_connected = true;
}


static void on_got_ip(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data){
	wifi_has_ip = true;
    ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
    memcpy(&s_ip_addr, &event->ip_info.ip, sizeof(s_ip_addr));
    xEventGroupSetBits(s_connect_event_group, GOT_IPV4_BIT);
}

static void on_lost_ip(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data){
	wifi_has_ip = false;
}

#ifdef CONFIG_EXAMPLE_CONNECT_IPV6

static void on_got_ipv6(void *arg, esp_event_base_t event_base,
                        int32_t event_id, void *event_data)
{
    ip_event_got_ip6_t *event = (ip_event_got_ip6_t *)event_data;
    memcpy(&s_ipv6_addr, &event->ip6_info.ip, sizeof(s_ipv6_addr));
    if (EXAMPLE_CONNECT_PREFERRED_IPV6_TYPE) {
        if (ip6_addr_isglobal(&s_ipv6_addr)) {
            xEventGroupSetBits(s_connect_event_group, GOT_IPV6_BIT);
        }
    } else {
        xEventGroupSetBits(s_connect_event_group, GOT_IPV6_BIT);
    }
}

#endif // CONFIG_EXAMPLE_CONNECT_IPV6


static void start(void)
{
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &on_wifi_disconnect, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &on_got_ip, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_CONNECTED, &on_wifi_connect, NULL));
	ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_LOST_IP, &on_lost_ip, NULL));

    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    //wifi_config_t wifi_config = { 0 };
/*
    strncpy((char *)&wifi_config.sta.ssid, s_connection_name, 32);
    strncpy((char *)&wifi_config.sta.password, s_connection_passwd, 32);
*/

    /*char * ssid = tag_WIFI_ssid1->getStr();
    char * pass = tag_WIFI_pass1->getStr();
    ESP_LOGI(tag,"actual pass - %s, size - %d", pass, tag_WIFI_pass1->size());
    ESP_LOGI(tag,"actual ssid -%s, size - %d", ssid, tag_WIFI_ssid1->size());

    strncpy((char *)&wifi_config.sta.ssid, ssid, tag_WIFI_ssid1->size());
    strncpy((char *)&wifi_config.sta.password, pass, tag_WIFI_pass1->size());*/


	/*strInit((char *)&wifi_config.sta.ssid, 32);
	strInit((char *)&wifi_config.sta.password, 32);
	strncpy((char *)&wifi_config.sta.ssid, tag_WIFI_ssid1->getStr(), tag_WIFI_ssid1->size());
	strncpy((char *)&wifi_config.sta.password, tag_WIFI_pass1->getStr(), tag_WIFI_pass1->size());
	ESP_LOGI(tag,"Trying to connect with default  config: \n ssid -%s, pass - %s", wifi_config.sta.ssid, wifi_config.sta.password);
	wifi_defConfig = true;


    ESP_LOGI(TAG, "Connecting to %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());*/

    ESP_LOGI(tag,"Trying to connect with default  config: \n ssid -%s, pass - %s", wifi_config.sta.ssid, wifi_config.sta.password);
    wifi_start(tag_WIFI_ssid1, tag_WIFI_pass1);
    wifi_defConfig = true;
}

static void stop(void)
{
    esp_err_t err = esp_wifi_stop();
    if (err == ESP_ERR_WIFI_NOT_INIT) {
        return;
    }
    ESP_ERROR_CHECK(err);

    ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &on_wifi_disconnect));
    ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &on_got_ip));
    ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, WIFI_EVENT_STA_CONNECTED, &on_wifi_connect));
	ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_LOST_IP, &on_lost_ip));

    ESP_ERROR_CHECK(esp_wifi_deinit());
}

esp_err_t wifi_connect(void)
{
    if (s_connect_event_group != NULL) {
        return ESP_ERR_INVALID_STATE;
    }

#ifdef EN_STAT_IP
    esp_err_t e;
	e = tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA);
	ESP_LOGI(TAG,"stoping dhcps - %d", e);
			//Set static IP
	tcpip_adapter_ip_info_t ipInfo;
	inet_pton(AF_INET, tag_WIFI_IP->getStr(), &ipInfo.ip);
	inet_pton(AF_INET, tag_WIFI_gw->getStr(), &ipInfo.gw);
	inet_pton(AF_INET, tag_WIFI_mask->getStr(), &ipInfo.netmask);
	e = tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA, &ipInfo);
	ESP_LOGI(TAG,"seting static net config - %d", e);
			//Set Main DNS server
	tcpip_adapter_dns_info_t dnsInfo;
	inet_pton(AF_INET, tag_WIFI_DNS->getStr(), &dnsInfo.ip);
	e = tcpip_adapter_set_dns_info(TCPIP_ADAPTER_IF_STA, TCPIP_ADAPTER_DNS_MAIN, &dnsInfo);
	ESP_LOGI(TAG,"seting dns info - %d", e);
#endif

    s_connect_event_group = xEventGroupCreate();
    start();
    xEventGroupWaitBits(s_connect_event_group, CONNECTED_BITS, true, true, portMAX_DELAY);
    ESP_LOGI(TAG, "Connected to %s", s_connection_name);
    ESP_LOGI(TAG, "IPv4 address: " IPSTR, IP2STR(&s_ip_addr));
    return ESP_OK;
}

esp_err_t wifi_disconnect(void)
{
    if (s_connect_event_group == NULL) {
        return ESP_ERR_INVALID_STATE;
    }
    vEventGroupDelete(s_connect_event_group);
    s_connect_event_group = NULL;
    stop();
    ESP_LOGI(TAG, "Disconnected from %s", s_connection_name);
    s_connection_name[0] = '\0';
    return ESP_OK;
}

esp_err_t wifi_set_connection_info(const char *ssid, const char *passwd)
{
    strncpy(s_connection_name, ssid, sizeof(s_connection_name));
    strncpy(s_connection_passwd, passwd, sizeof(s_connection_passwd));
    return ESP_OK;
}

esp_err_t wifi_begin(){
	tag_WIFI_ssid1 = new Tag(&prop_WIFI_ssid1);
	tag_WIFI_ssid2 = new Tag(&prop_WIFI_ssid2);
	tag_WIFI_pass1 = new Tag(&prop_WIFI_pass1);
	tag_WIFI_pass2 = new Tag(&prop_WIFI_pass2);


	tag_WIFI_IP = new Tag(&prop_WIFI_IP);
	tag_WIFI_mask = new Tag(&prop_WIFI_mask);
	tag_WIFI_gw = new Tag(&prop_WIFI_gw);
	tag_WIFI_DNS = new Tag(&prop_WIFI_DNS);

	Tag::printAll();

	return wifi_connect();
}

bool wifi_is_connected(){
	if(wifi_connected || wifi_has_ip) return true;
	return false;
}
