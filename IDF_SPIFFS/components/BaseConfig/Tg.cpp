/*
 * Tg.cpp
 *
 *  Created on: 8 февр. 2021 г.
 *      Author: Vanavaks
 */

#include "Tg.h"
#include <esp_err.h>
#include <stddef.h>
#include "nvs_flash.h"

const char* TG = "Tg";

#define TG_DEBUG
#ifdef TG_DEBUG
#define TG_LOGD(format, ...) ESP_LOGI(TG, format, ##__VA_ARGS__)
#else
#define TG_LOGD(tg, format, ...) ESP_LOGD(TG, format, ##__VA_ARGS__)
#endif


char defString[] = " ";



uint16_t Tg::indHead = 0;
nvs_handle Tg::handle = 0;

bool Tg::isInit;
Tg* Tg::arr[CONFIG_TG_ARR_SIZE];


const char* nsDef = "par";

Tg::Tg() {}
Tg::~Tg() {}

void Tg::begin(){
	if(isInit) return;
	initNVS();
	arrInit();
	isInit = true;
}

Tg* Tg::findTg(const char* key){
	for(int i=0;i<indHead;i++){
		if (!strcmp(key,arr[i]->prop->KeyName)) return arr[i];
	}
	return NULL;
}

#if 0
err1_t Tg::set(const char* val, size_t size) {
	if(this->prop->type != TG_STR) {return ESP_ERR_TYPE;}

	if(value.asstr != NULL) {delete [] this->value.asstr;}			/* deleting of old array ??????????? pointer must be */
	this->value.asstr = new char[size+1];							/* creating of new array */
	strcpy(this->value.asstr, val);

	if(prop->saveble) save();
	return ESP_ERR_TG_OK;
}

err1_t Tg::set(const char* key, const char* value) {
	Tg* tg;
	ESP_LOGI(TG, "Set tg: name - %s, val - %s", key, value);
	if((tg = findTg(key)) == NULL) return ESP_ERR_TG_NAME;

	if(tg->prop->type == TG_STR){
		TG_LOGD("tg is STR, ind = %d", tg->index);
		tg->set(value, strlen(value + 1));
	}
	else if(tg->prop->type == TG_UI32){
		uint32_t v = atol(value);
		TG_LOGD("tg is ui32, val = %d", v);
		tg->set(v);
		tg->value.asui32 = tg->getUI32();
	}
	else if(tg->prop->type == TG_I32){
		int32_t v = atol(value);
		TG_LOGD("tg is i32, val = %d", v);
		tg->set(v);
		tg->value.asi32 = tg->getI32();
	}
	else if(tg->prop->type == TG_UI8){
		uint8_t v = atoi(value);
		TG_LOGD("tg is ui8, val = %d", v);
		tg->set(v);
		tg->value.asui8 = tg->getUI8();
	}
	else if(tg->prop->type == TG_BOOL){	/* WE MUCT CHACK TRUE VALUE AND FAULSE VALUE */
		TG_LOGD("tg is bool, val = '%s', bool true string is '%s", value, TG_TRUE_STR);
		if(!strcmp(value, TG_TRUE_STR)){	/* value is true */
			tg->value.asbool = true;
		}else{
			tg->value.asbool = false;
		}
	}
	else if(tg->prop->type == TG_FLOAT){
		float v = atof(value);
		TG_LOGD("tg is float, %f", v);
		tg->value.asfloat = tg->set(v);
	}else{
		ESP_LOGW(TG, "Unknown tg type - '%d'", tg->prop->type);
	}
	return ESP_ERR_TYPE;
}
#endif

bool Tg::chackKey(const char* name){
	for(int i=0;i<indHead;i++){
		if (!strcmp(name,arr[i]->prop->KeyName)) return false;
	}
	return true;
}

void Tg::init(const tgProp_t* tgProp){
	err1_t err;
		//check initialisation of Tg class
	if(!isInit) {Tg::begin();}
		//check properties pointer
	if(tgProp == NULL){err = ESP_ERR_TG_NULL_POINTER; goto err;}
		//chack name
	if(!chackKey(tgProp->KeyName)){err = ESP_ERR_TG_NAME; goto err;}
		//reg Tg
	err = Tg::arrAdd(this);
		//reg properties
	if (err == ESP_OK) {this->prop = tgProp; return;}
err:
	ESP_ERROR_CHECK(err);
}

//!!!!!!!!!!!!!!!!!!передавать значение в конструктор не имеет смысла!!!!///

Tg::Tg(const tgProp_t* tgProp) {
		//init Tg
	init(tgProp);	//---- unpask here
		//init value
	if(prop->saveble){ read(); }
	else{
		/*if(prop->type == TG_STR){
			if(tgProp->val.asstr == NULL){
				value.asstr = defString;
			}else{
				value.asstr = tgProp->val.asstr;
			}
		}
		else if(prop->type == TG_UI32){
			value.asui32 = tgProp->val.asui32;
		}
		else if(prop->type == TG_I32){
			value.asi32 = tgProp->val.asi32;
		}
		else if(prop->type == TG_UI8){
			value.asui8 = tgProp->val.asui8;
		}
		else if(prop->type == TG_BOOL){
			value.asbool = tgProp->val.asbool;
		}
		else if(prop->type == TG_BOOL){
			value.asfloat = tgProp->val.asfloat;
		}*/
	}
}


//==================================== NVS ==============================================//

void Tg::initNVS() {

	esp_err_t err = nvs_flash_init();
	ESP_LOGI(TG, "Nvs_flash_init starting");
	if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
		ESP_LOGW(TG, "nvs_flash_init failed (0x%x), erasing partition and retrying", err);
		ESP_ERROR_CHECK(nvs_flash_erase());
		err = nvs_flash_init();
	}
	else if(err == ESP_ERR_NOT_FOUND){
		ESP_LOGE(TG, "if no partition with label nvs is found in the partition table");
	}
	else if (err == ESP_OK){

	}
	else{
		ESP_ERROR_CHECK(err);
	}

	if(err == ESP_OK){
		ESP_ERROR_CHECK(nvs_open(nsDef, NVS_READWRITE, &handle));
	}
}

void Tg::read() {
	open();
	err1_t err = ESP_OK;
/*
	if(prop->type == TG_STR){
		size_t size;//= CHAR_BUFF_SIZE;
		err = nvs_get_str(handle, prop->KeyName, NULL, &size);	//reading present and size of string
		if(err == ESP_OK){	//key present, string may have zero size
			value.asstr = (char*) malloc(size + 1);	//allocating of memory; during reading value didn't need free previos pointer
			if(value.asstr == NULL) {
				ESP_LOGE(tg, "No free heap malloc null pointer");
				ESP_ERROR_CHECK(ESP_ERR_TG_NULL_POINTER)
			}
				//----------here must be free of value.asstr pointer----------
			err = nvs_get_str(handle, prop->KeyName, value.asstr, &size);
			ESP_LOGI(TG, "NVS get value %s = %s", prop->KeyName, value.asstr);
		}
		if(err != ESP_OK) value.asstr = prop->val.asstr;
	}

	else if(prop->type == TG_UI32){
		err = nvs_get_u32(handle, prop->KeyName, &value.asui32);
		if(err != ESP_OK) value.asui32 = prop->val.asui32;
	}
	else if(prop->type == TG_UI8){
		unsigned char v;
		err = nvs_get_u8(handle, prop->KeyName, &v);
		value.asui8 = (uint8_t)v;
		if(err != ESP_OK) value.asui8 = prop->val.asui8;
	}
	else if(prop->type == TG_I32){
		err = nvs_get_i32(handle, prop->KeyName, &value.asi32);
		if(err != ESP_OK) value.asi32 = prop->val.asi32;
	}
	else if(prop->type == TG_FLOAT){
		float v;
		size_t size = sizeof(v);
		err = nvs_get_blob(handle, prop->KeyName, &value.asfloat, &size);
		if(err != ESP_OK) value.asfloat = prop->val.asfloat;
	}
	else if(prop->type == TG_BOOL){
		unsigned char v;
		err = nvs_get_u8(handle, prop->KeyName, &v);
		if(v > 0) value.asbool = true;
		else  value.asbool = false;
		if(err != ESP_OK) value.asbool = prop->val.asbool;
	}
	else{
		ESP_ERROR_CHECK(ESP_ERR_TYPE)	//crash bad type
	}
*/
	switch (err){

		case ESP_ERR_NVS_INVALID_NAME:
			ESP_LOGW(TG, "Reading absent parameter in NVS %s, err= %d",prop->KeyName, err);
			break;
		case ESP_OK:
			break;
		default:
			ESP_LOGW(TG, "Reading parameter %s, err= %x", prop->KeyName, err);
		;
	}
	close();
}

void Tg::save() {	//mak
	open();
	err1_t err = ESP_OK;
	ESP_LOGI(TG, "Trying to save parameter %s",prop->KeyName);
	/*saveCnt++;			//???????????
	if(prop->type == TG_STR){
		err = nvs_set_str(handle, prop->KeyName, value.asstr);
	}else if(prop->type == TG_UI32){
		err = nvs_set_u32(handle, prop->KeyName, value.asui32);
	}else if(prop->type == TG_BOOL){
		err = nvs_set_u8(handle, prop->KeyName, value.asbool);
	}else if(prop->type == TG_UI8){
		err = nvs_set_u8(handle, prop->KeyName, value.asui8);
	}else if(prop->type == TG_I32){
		err = nvs_set_i32(handle, prop->KeyName, value.asi32);
	}else if(prop->type == TG_FLOAT){
		err = nvs_set_blob(handle, prop->KeyName, &value.asfloat,sizeof(float));
	}else {ESP_ERROR_CHECK(ESP_ERR_TYPE); saveCnt--;}

	if(err != ESP_OK) ESP_LOGW(TG, "saving parameter %s, err= %d",prop->KeyName, err);*/
	close();
}

/*
void Tg::Print(){
	printf("Tg: category - %s, name - %s, ",prop->category , prop->KeyName);
	if(prop->type == TG_STR) printf("type - str, val = %s, def val = %s", value.asstr , prop->val.asstr);
	else if(prop->type == TG_UI32) printf("type - ui32, val = %d, def val = %d, ", value.asui32, prop->val.asui32);
	else if(prop->type == TG_I32) printf("type - i32, val = %d, def val = %d, ", value.asi32, prop->val.asi32);
	else if(prop->type == TG_UI8) printf("type - ui8, val = %d, def val = %d, ", value.asi8, prop->val.asi8);
	else if(prop->type == TG_FLOAT) printf("type - float, val = %f, def val = %f, ", value.asfloat, prop->val.asfloat);
	else if(prop->type == TG_BOOL) printf("type - bool, val = %d, def val = %d, ", value.asbool, prop->val.asbool);
	printf("arr index - %d, size - %d \r\n", index, this->size());
}*/


void Tg::printAll(){
	for(int i=0; i<indHead; i++){
		//arr[i]->print();
	}
}
/*
size_t Tg::size() {
	uint16_t s=0;
	if(prop->type == TG_STR){
		char* str = value.asstr;
		for(s=0; *str != 0;s++){str++;}
		return s;
	}else if(prop->type == TG_UI32){return 4;
	}else if(prop->type == TG_BOOL){return 1;
	}else if(prop->type == TG_UI8){return 1;
	}else if(prop->type == TG_I32){return 4;
	}else if(prop->type == TG_FLOAT){return 4;
	}else {ESP_ERROR_CHECK(ESP_ERR_TYPE);}
	return sizeof value;
}*/

/*void Tg::getByCategory(Tg* t[],size_t size) {
}*/

//================================= Array ==============================================//
err1_t Tg::arrAdd(Tg* tg) {
	if(tg == NULL) return ESP_ERR_TG_NULL_POINTER;
	if(indHead >= CONFIG_TG_ARR_SIZE) return ESP_ERR_ARR_OVERSIZE;
	tg->index = indHead;
	Tg::arr[indHead++] = tg;
	return ESP_ERR_TG_OK;
}

void Tg::arrInit(){
	indHead = 0;
	for(int i=0;i<CONFIG_TG_ARR_SIZE;i++){
		arr[i] = NULL;
	}
}

Tg* Tg::getNextByCategory(uint16_t* index, const char* cat) {
	TG_LOGD("Get next tg by cat - '%s', index - %d ", cat, *index);
	if (cat == NULL){
		ESP_LOGE(TG,"Category have NULL pointer");
		return NULL;
	}
	if (*index >= CONFIG_TG_ARR_SIZE) {
		TG_LOGD("Index to high - %d ", *index);
		return NULL;
	}

	for (int i = *index; i < indHead; i++) {
		TG_LOGD(" tg ind - %d, head - %d", i, indHead);

		if(arr[i] != NULL) {
			TG_LOGD(" arr[%d] not null \r\n", i);

			char ar[20]{0};
			strcpy(ar, arr[i]->prop->category);
			int t = strcmp(ar, cat);
			TG_LOGD(" str cmp result - %d", t);

			if (t == 0) {
				TG_LOGD(TG,"Tg %d is equal", i);
				*index = i;
				return Tg::arr[i];
			}
			else{TG_LOGD("Tg not equal -> chack next");}
		}else{ESP_LOGE(TG," tg ind - %d have NULL pointer", i);}
	}
	return NULL;
}
