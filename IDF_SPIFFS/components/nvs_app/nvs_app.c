/*
 * nvs_app.c
 *
 *  Created on: 12 џэт. 2021 у.
 *      Author: Vanavaks
 */

#include "nvs.h"
#include "nvs_flash.h"
#include "cmd.h"
#include "main.h"
#include "argtable3/argtable3.h"
#include "esp_console.h"

#include "nvs_app.h"

static const char* TAG = "nvs_app";



const char nameSps[] = "Category1";
/** command set par */
static struct {
	struct arg_str *parName;
	struct arg_str *parVal;
	struct arg_end *end;
} nvs_set_args;

static struct {
	struct arg_str *parName;
	struct arg_int *parVal;
	struct arg_end *end;
} nvs_setI_args;


static int nvs_setI(int argc, char **argv) {
	int nerrors = arg_parse(argc, argv, (void**) &nvs_setI_args);
	if (nerrors != 0) {
		arg_print_errors(stderr, nvs_setI_args.end, argv[0]);
		return 1;
	}

	nvs_handle handle;
	ESP_ERROR_CHECK(nvs_open(nameSps, NVS_READWRITE, &handle));
	ESP_LOGI(TAG, "Set int parameter %s = '%d'",nvs_setI_args.parName->sval[0], nvs_setI_args.parVal->ival[0]);
	ESP_ERROR_CHECK(nvs_set_i32(handle, nvs_setI_args.parName->sval[0], nvs_setI_args.parVal->ival[0]));
	nvs_close(handle);

	return 0;
}

static struct {
	struct arg_str *parName;
	struct arg_end *end;
} nvs_getI_args;


static int nvs_getI(int argc, char **argv) {
	nvs_handle handle;

	int nerrors = arg_parse(argc, argv, (void**) &nvs_getI_args);
	if (nerrors != 0) {
		arg_print_errors(stderr, nvs_getI_args.end, argv[0]);
		return 1;
	}

	int32_t val;

	ESP_ERROR_CHECK(nvs_open(nameSps, NVS_READWRITE, &handle));
	ESP_LOGI(TAG, "Get int parameter %s",nvs_getI_args.parName->sval[0]);
	ESP_ERROR_CHECK(nvs_get_i32(handle, nvs_getI_args.parName->sval[0], &val));
	nvs_close(handle);


	ESP_LOGI(TAG,"Parametr %s = %d",nvs_getI_args.parName->sval[0], val);
	return 0;
}

void register_nvsSetIPar() {
	nvs_setI_args.parVal = arg_int0(NULL, "val", "<i>", "int value");
	nvs_setI_args.parName = arg_str0(NULL, NULL, "<parName>", "parameter Name");
	nvs_setI_args.end = arg_end(2);

	const esp_console_cmd_t cmd = {
			.command = "setI",
			.help =	"Set int parameter",
			.hint = NULL,
			.func = &nvs_setI,
			.argtable = &nvs_setI_args };
	ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}


void register_nvsGetIPar() {
	nvs_getI_args.parName = arg_str0(NULL, NULL, "<parName>", "parameter Name");
	nvs_getI_args.end = arg_end(2);

	const esp_console_cmd_t cmd = {
			.command = "getI",
			.help =	"Get int parameter",
			.hint = NULL,
			.func = &nvs_getI,
			.argtable = &nvs_getI_args };
	ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}

static int nvs_set(int argc, char **argv) {
	int nerrors = arg_parse(argc, argv, (void**) &nvs_set_args);
	if (nerrors != 0) {
		arg_print_errors(stderr, nvs_set_args.end, argv[0]);
		return 1;
	}

	nvs_handle handle;
	ESP_ERROR_CHECK(nvs_open(nameSps, NVS_READWRITE, &handle));
	ESP_LOGI(TAG, "Set parameter %s = '%s'",nvs_set_args.parName->sval[0], nvs_set_args.parVal->sval[0]);
	ESP_ERROR_CHECK(nvs_set_str(handle, nvs_set_args.parName->sval[0], nvs_set_args.parVal->sval[0]));
	nvs_close(handle);

	return 0;
}

/** command get par */
static struct {
	struct arg_str *parName;
	struct arg_end *end;
} nvs_get_args;

static int nvs_get(int argc, char **argv) {
	nvs_handle handle;

	int nerrors = arg_parse(argc, argv, (void**) &nvs_get_args);
	if (nerrors != 0) {
		arg_print_errors(stderr, nvs_get_args.end, argv[0]);
		return 1;
	}

	char buf[30];
	size_t buf_len = sizeof(buf);

	ESP_ERROR_CHECK(nvs_open(nameSps, NVS_READWRITE, &handle));
	ESP_LOGI(TAG, "Get parameter %s",nvs_get_args.parName->sval[0]);
	esp_err_t err = nvs_get_str(handle, nvs_get_args.parName->sval[0], buf, &buf_len);
	ESP_ERROR_CHECK(err);
	if (err == 0) ESP_LOGI(TAG,"Parametr %s = %s",nvs_get_args.parName->sval[0], buf);
	nvs_close(handle);


	return 0;
}



void register_nvsSetPar() {
	//ota_update_args.timeout = arg_int0(NULL, "timeout", "<t>", "Connection timeout, ms");
	//ota_update_args.timeout->ival[0] = 5000; // set default value

	nvs_set_args.parName = arg_str0(NULL, NULL, "<parName>", "parameter Name");
	nvs_set_args.parName->sval[0] = "p1";
	nvs_set_args.parVal = arg_str1(NULL, NULL, "<parVal>", "parameter value");
	nvs_set_args.parVal->sval[0] = "null";
	nvs_set_args.end = arg_end(2);

	const esp_console_cmd_t cmd = {
			.command = "set",
			.help =	"Set string parameter",
			.hint = NULL,
			.func = &nvs_set,
			.argtable = &nvs_set_args };
	ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}


void register_nvsGetPar() {
	nvs_get_args.parName = arg_str0(NULL, NULL, "<parName>", "parameter Name");
	//nvs_get_args.parName->sval[0] = "p1";
	nvs_get_args.end = arg_end(2);

	const esp_console_cmd_t cmd = {
			.command = "get",
			.help =	"Get string parameter",
			.hint = NULL,
			.func = &nvs_get,
			.argtable = &nvs_get_args };
	ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}


void initialize_nvs() {
	esp_err_t err = nvs_flash_init();
	if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
		ESP_LOGW(TAG, "nvs_flash_init failed (0x%x), erasing partition and retrying", err);
		ESP_ERROR_CHECK(nvs_flash_erase());
		err = nvs_flash_init();
	}
	else if(err == ESP_ERR_NOT_FOUND){
		ESP_LOGE(TAG, "if no partition with label nvs is found in the partition table");
	}
	else if (err == ESP_OK){

	}
	else{
		ESP_ERROR_CHECK(err);
	}
		ESP_LOGI(TAG, "Initializing NVS result %x", err);
		register_nvsSetPar();
		register_nvsGetPar();
		register_nvsSetIPar();
		register_nvsGetIPar();
}
