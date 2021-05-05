/*
 * Tag.cpp
 *
 *  Created on: 14 янв. 2021 г.
 *      Author: Vanavaks
 */

#include <esp_err.h>
#include <stddef.h>
#include <Tag.h>
#include "nvs_flash.h"

const char* tag = "Tag";

#define TAG_DEBUG
#ifdef TAG_DEBUG
#define TAG_LOGD(format, ...) ESP_LOGI(tag, format, ##__VA_ARGS__)
#else
#define TAG_LOGD(tag, format, ...) ESP_LOGD(tag, format, ##__VA_ARGS__)
#endif


char defString[] = " ";

uint16_t Tag::indHead = 0;
nvs_handle Tag::handle = 0;
uint32_t Tag::saveCnt = 0;
bool Tag::isInit = 0;
Tag* Tag::arr[CONFIG_TAG_ARR_SIZE];


const char* nsDef = "par";

Tag::Tag() {}
Tag::~Tag() {}

void Tag::begin(){
	if(isInit) return;
	initNVS();
	arrInit();
	isInit = true;
}

Tag* Tag::findTag(const char* key){
	for(int i=0;i<indHead;i++){
		if (!strcmp(key,arr[i]->prop->KeyName)) return arr[i];
	}
	return NULL;
}

err1_t Tag::set(const char* key, const char* value) {
	Tag* tg;
	ESP_LOGI(tag, "Set tag: name - %s, val - %s", key, value);
	if((tg = findTag(key)) == NULL) return ESP_ERR_TAG_NAME;

	if(tg->prop->type == TAG_STR){
		TAG_LOGD("tag is STR, ind = %d", tg->index);
		tg->set(value, strlen(value + 1));
	}
	else if(tg->prop->type == TAG_UI32){
		uint32_t v = atol(value);
		TAG_LOGD("tag is ui32, val = %d", v);
		tg->set(v);
		tg->value.asui32 = tg->getUI32();
	}
	else if(tg->prop->type == TAG_I32){
		int32_t v = atol(value);
		TAG_LOGD("tag is i32, val = %d", v);
		tg->set(v);
		tg->value.asi32 = tg->getI32();
	}
	else if(tg->prop->type == TAG_UI8){
		uint8_t v = atoi(value);
		TAG_LOGD("tag is ui8, val = %d", v);
		tg->set(v);
		tg->value.asui8 = tg->getUI8();
	}
	else if(tg->prop->type == TAG_BOOL){	/* WE MUCT CHACK TRUE VALUE AND FAULSE VALUE */
		TAG_LOGD("tag is bool, val = '%s', bool true string is '%s", value, TAG_TRUE_STR);
		if(!strcmp(value, TAG_TRUE_STR)){	/* value is true */
			tg->setBL(true);
			//tg->value.asbool = true;
		}else{
			tg->setBL(false);
			//tg->value.asbool = false;
		}
	}
	else if(tg->prop->type == TAG_FLOAT){
		float v = atof(value);
		TAG_LOGD("tag is float, %f", v);
		tg->value.asfloat = tg->set(v);
	}else{
		ESP_LOGW(tag, "Unknown tag type - '%d'", tg->prop->type);
	}
	return ESP_ERR_TYPE;
}

bool Tag::chackKey(const char* name){
	for(int i=0;i<indHead;i++){
		if (!strcmp(name,arr[i]->prop->KeyName)) return false;
	}
	return true;
}

void Tag::init(const tagProp_t* tagProp){
	err1_t err;
		//check initialisation of Tag class
	if(!isInit) {Tag::begin();}
		//check properties pointer
	if(tagProp == NULL){err = ESP_ERR_TAG_NULL_POINTER; goto err;}
		//chack name
	if(!chackKey(tagProp->KeyName)){err = ESP_ERR_TAG_NAME; goto err;}
		//reg Tag
	err = Tag::arrAdd(this);
		//reg properties
	if (err == ESP_OK) {this->prop = tagProp; return;}
err:
	ESP_ERROR_CHECK(err);
}

//!!!!!!!!!!!!!!!!!!передавать значение в конструктор не имеет смысла!!!!///

Tag::Tag(const tagProp_t* tagProp) {
		//init Tag
	init(tagProp);	//---- unpask here
		//init value
	if(prop->saveble){ read(); }
	else{
		if(prop->type == TAG_STR){
			if(tagProp->val.asstr == NULL){
				value.asstr = defString;
			}else{
				value.asstr = tagProp->val.asstr;
			}
		}
		else if(prop->type == TAG_UI32){
			value.asui32 = tagProp->val.asui32;
		}
		else if(prop->type == TAG_I32){
			value.asi32 = tagProp->val.asi32;
		}
		else if(prop->type == TAG_UI8){
			value.asui8 = tagProp->val.asui8;
		}
		else if(prop->type == TAG_BOOL){
			value.asbool = tagProp->val.asbool;
		}
		else if(prop->type == TAG_BOOL){
			value.asfloat = tagProp->val.asfloat;
		}
	}
}

/*
err1_t Tag::get(char* key, char* val) {
	return ESP_ERR_TAG_OK;
}*/


err1_t Tag::set(uint32_t val) {
	if(this->prop->type != TAG_UI32) {return ESP_ERR_TYPE;}
	value.asui32 = val;
	if(prop->saveble) save();
	return ESP_ERR_TAG_OK;
}




char* Tag::getStr(){
	if(prop->type == TAG_STR)
		if(value.asstr == NULL){
			ESP_LOGE(tag, "value as string have NULL pointer");
			return defString;
		}else{
			return value.asstr;
		}
	else {
		ESP_ERROR_CHECK(ESP_ERR_TYPE);
		return defString;
	}
}

uint32_t Tag::getUI32(){
	if(prop->type == TAG_UI32) return value.asui32;
	else {
		ESP_ERROR_CHECK(ESP_ERR_TYPE);
		return 0;
	}
}
	//handling of type errors  and more others better to make here

bool Tag::getBL() {
	if(prop->type == TAG_BOOL) return value.asbool;
	else {
		ESP_ERROR_CHECK(ESP_ERR_TYPE);
		return NULL;
	}
}

uint8_t Tag::getUI8() {
	if(prop->type == TAG_UI8) return value.asui8;
	else {
		ESP_ERROR_CHECK(ESP_ERR_TYPE);
		return 0;
	}
}

int32_t Tag::getI32() {
	if(prop->type == TAG_I32) return value.asi32;
	else {
		ESP_ERROR_CHECK(ESP_ERR_TYPE);
		return 0;
	}
}

float Tag::getFL() {
	if(prop->type == TAG_FLOAT) return value.asfloat;
		else {
		ESP_ERROR_CHECK(ESP_ERR_TYPE);
		return 0;
	}
}

err1_t Tag::set(bool val) {
	if(this->prop->type != TAG_BOOL) {return ESP_ERR_TYPE;}
	value.asbool = val;
	if(prop->saveble) save();
	return ESP_ERR_TAG_OK;
}

err1_t Tag::set(uint8_t val) {
	if(this->prop->type != TAG_UI8) {return ESP_ERR_TYPE;}
	value.asui8 = val;
	if(prop->saveble) save();
	return ESP_ERR_TAG_OK;
}

err1_t Tag::set(int32_t val) {
	if(this->prop->type != TAG_I32) {return ESP_ERR_TYPE;}
	value.asi32 = val;
	if(prop->saveble) save();
	return ESP_ERR_TAG_OK;
}

err1_t Tag::set(float val) {
	if(this->prop->type != TAG_FLOAT) {return ESP_ERR_TYPE;}
	value.asfloat = val;
	if(prop->saveble) save();
	return ESP_ERR_TAG_OK;
}


err1_t Tag::set(const char* val, size_t size) {
	if(this->prop->type != TAG_STR) {return ESP_ERR_TYPE;}

	if(value.asstr != NULL) {delete [] this->value.asstr;}			/* deleting of old array ??????????? pointer must be */
	this->value.asstr = new char[size+1];							/* creating of new array */
	strcpy(this->value.asstr, val);

	if(prop->saveble) save();
	return ESP_ERR_TAG_OK;
}
/*
err1_t Tag::get(char* key, uint32_t* val) {
	return ESP_ERR_TAG_OK;
}*/

//==================================== NVS ==============================================//

void Tag::initNVS() {
	ESP_LOGI(tag, "Nvs_flash_init starting");
	esp_err_t err = nvs_flash_init();

	if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
		ESP_LOGW(tag, "nvs_flash_init failed (0x%x), erasing partition and retrying", err);
		ESP_ERROR_CHECK(nvs_flash_erase());
		err = nvs_flash_init();
	}
	else if(err == ESP_ERR_NOT_FOUND){
		ESP_LOGE(tag, "if no partition with label nvs is found in the partition table");
	}
	else if (err == ESP_OK){
		ESP_LOGI(tag, "Nvs_init done");
	}
	else{
		ESP_LOGW(tag, "nvs_flash_init failed err (0x%x)", err);
		ESP_ERROR_CHECK(err);
	}
	/*
	if(err == ESP_OK){
		ESP_ERROR_CHECK(nvs_open(nsDef, NVS_READWRITE, &handle));
	}*/
}

void Tag::read() {
	open();
	err1_t err = ESP_OK;

	if(prop->type == TAG_STR){
		size_t size;//= CHAR_BUFF_SIZE;
		err = nvs_get_str(handle, prop->KeyName, NULL, &size);	//reading present and size of string
		if(err == ESP_OK){	//key present, string may have zero size
			value.asstr = (char*) malloc(size + 1);	//allocating of memory; during reading value didn't need free previos pointer
			if(value.asstr == NULL) {
				ESP_LOGE(tag, "No free heap malloc null pointer");
				ESP_ERROR_CHECK(ESP_ERR_TAG_NULL_POINTER)
			}
				//----------here must be free of value.asstr pointer----------
			err = nvs_get_str(handle, prop->KeyName, value.asstr, &size);
			ESP_LOGI(tag, "NVS get value %s = %s", prop->KeyName, value.asstr);
		}
		if(err != ESP_OK) value.asstr = prop->val.asstr;
	}

	else if(prop->type == TAG_UI32){
		err = nvs_get_u32(handle, prop->KeyName, &value.asui32);
		if(err != ESP_OK) value.asui32 = prop->val.asui32;
	}
	else if(prop->type == TAG_UI8){
		unsigned char v;
		err = nvs_get_u8(handle, prop->KeyName, &v);
		value.asui8 = (uint8_t)v;
		if(err != ESP_OK) value.asui8 = prop->val.asui8;
	}
	else if(prop->type == TAG_I32){
		err = nvs_get_i32(handle, prop->KeyName, &value.asi32);
		if(err != ESP_OK) value.asi32 = prop->val.asi32;
	}
	else if(prop->type == TAG_FLOAT){
		float v;
		size_t size = sizeof(v);
		err = nvs_get_blob(handle, prop->KeyName, &value.asfloat, &size);
		if(err != ESP_OK) value.asfloat = prop->val.asfloat;
	}
	else if(prop->type == TAG_BOOL){
		unsigned char v;
		err = nvs_get_u8(handle, prop->KeyName, &v);
		if(v > 0) value.asbool = true;
		else  value.asbool = false;
		if(err != ESP_OK) value.asbool = prop->val.asbool;
	}
	else{
		ESP_ERROR_CHECK(ESP_ERR_TYPE)	//crash bad type
	}

	switch (err){

		/*case ESP_ERR_NVS_INVALID_NAME:
			ESP_LOGW(tag, "Reading absent parameter in NVS %s, err= %d",prop->KeyName, err);
			break;*/
		case ESP_ERR_NVS_NOT_FOUND:
			ESP_LOGI(tag,"Requested key doesn't existin NVS %s, err= %d",prop->KeyName, err);
			break;
		case ESP_OK:
			break;
		default:
			ESP_LOGW(tag, "Reading parameter %s, err= %x", prop->KeyName, err);
		;
	}
	close();
}

void Tag::setBL(bool val) {
	if(this->prop->type != TAG_BOOL) {ESP_ERROR_CHECK(ESP_ERR_TYPE);}
	value.asbool = val;
	if(prop->saveble) save();
}

void Tag::save() {
	open();
	err1_t err = ESP_OK;
	ESP_LOGI(tag, "Trying to save parameter %s",prop->KeyName);
	saveCnt++;			//???????????
	if(prop->type == TAG_STR){
		err = nvs_set_str(handle, prop->KeyName, value.asstr);
	}else if(prop->type == TAG_UI32){
		err = nvs_set_u32(handle, prop->KeyName, value.asui32);
	}else if(prop->type == TAG_BOOL){
		err = nvs_set_u8(handle, prop->KeyName, value.asbool);
	}else if(prop->type == TAG_UI8){
		err = nvs_set_u8(handle, prop->KeyName, value.asui8);
	}else if(prop->type == TAG_I32){
		err = nvs_set_i32(handle, prop->KeyName, value.asi32);
	}else if(prop->type == TAG_FLOAT){
		err = nvs_set_blob(handle, prop->KeyName, &value.asfloat,sizeof(float));
	}else {ESP_ERROR_CHECK(ESP_ERR_TYPE); saveCnt--;}

	if(err != ESP_OK) ESP_LOGW(tag, "saving parameter %s, err= %d",prop->KeyName, err);
	else ESP_LOGI(tag, "saved");
	close();
}

void Tag::Print(){
#if 0
	printf("Tag: category - %s, name - %s, ",prop->category , prop->KeyName);
	if(prop->type == TAG_STR) printf("type - str, val = %s, def val = %s", value.asstr , prop->val.asstr);
	else if(prop->type == TAG_UI32) printf("type - ui32, val = %d, def val = %d, ", value.asui32, prop->val.asui32);
	else if(prop->type == TAG_I32) printf("type - i32, val = %d, def val = %d, ", value.asi32, prop->val.asi32);
	else if(prop->type == TAG_UI8) printf("type - ui8, val = %d, def val = %d, ", value.asi8, prop->val.asi8);
	else if(prop->type == TAG_FLOAT) printf("type - float, val = %f, def val = %f, ", value.asfloat, prop->val.asfloat);
	else if(prop->type == TAG_BOOL) printf("type - bool, val = %d, def val = %d, ", value.asbool, prop->val.asbool);
	printf("arr index - %d, size - %d \r\n", index, this->size());
#endif
}

void Tag::PrintAdr(){
	void* v1 = this;
	void* v2 = (void*)this->prop;
	ESP_LOGI(tag,"Tag adr: '%x', prop adr; %x",(uint)v1, (uint)v2);
	//void* v3,v4,v5;
	/*v1 = (void*)(this->prop.category);
	v2 = (void*)(this->prop.name);
	v3 = (void*)(this->prop.saveble);
	v5 = (void*)(this->prop.val)
	ESP_LOGI(tag,"name adr: '%x', cat adr: '%x', type adr: '%x', savebl adr: '%x', savebl val: '%x', val adr: %x",(uint)v1, (uint)v2, (uint)v3, (uint)(this->prop->saveble), (uint)v5);
	*/
}

void Tag::printAll(){
	for(int i=0; i<indHead; i++){
		arr[i]->Print();
	}
}

size_t Tag::size() {
	uint16_t s=0;
	if(prop->type == TAG_STR){
		char* str = value.asstr;
		for(s=0; *str != 0;s++){str++;}
		return s;
	}else if(prop->type == TAG_UI32){return 4;
	}else if(prop->type == TAG_BOOL){return 1;
	}else if(prop->type == TAG_UI8){return 1;
	}else if(prop->type == TAG_I32){return 4;
	}else if(prop->type == TAG_FLOAT){return 4;
	}else {ESP_ERROR_CHECK(ESP_ERR_TYPE);}
	return sizeof value;
}

/*void Tag::getByCategory(Tag* t[],size_t size) {
}*/

//================================= Array ==============================================//
err1_t Tag::arrAdd(Tag* tag) {
	if(tag == NULL) return ESP_ERR_TAG_NULL_POINTER;
	if(indHead >= CONFIG_TAG_ARR_SIZE) return ESP_ERR_ARR_OVERSIZE;
	tag->index = indHead;
	Tag::arr[indHead++] = tag;
	return ESP_ERR_TAG_OK;
}

void Tag::arrInit(){
	indHead = 0;
	for(int i=0;i<CONFIG_TAG_ARR_SIZE;i++){
		arr[i] = NULL;
	}
}

err1_t Tag::saveVal(val_t v){
	if(prop->type == TAG_STR){
		value.asstr = v.asstr;
	} else if(prop->type == TAG_UI32){
		value.asui32 = v.asui32;
	} else if(prop->type == TAG_I32){
		value.asi32 = v.asi32;
	} else if(prop->type == TAG_UI8){
		value.asui8 = v.asui8;
	} else if(prop->type == TAG_BOOL){
		value.asbool = v.asbool;
	} else if(prop->type == TAG_FLOAT){
		value.asfloat = v.asfloat;
	} else return ESP_ERR_TYPE;
	return ESP_ERR_TAG_OK;
}

Tag* Tag::getNextByCategory(uint16_t* index, const char* cat) {
	TAG_LOGD("Get next tag by cat - '%s', index - %d ", cat, *index);
	if (cat == NULL){
		ESP_LOGE(tag,"Category have NULL pointer");
		return NULL;
	}
	if (*index >= CONFIG_TAG_ARR_SIZE) {
		TAG_LOGD("Index to high - %d ", *index);
		return NULL;
	}

	for (int i = *index; i < indHead; i++) {
		TAG_LOGD(" tag ind - %d, head - %d", i, indHead);

		if(arr[i] != NULL) {
			TAG_LOGD(" arr[%d] not null \r\n", i);

			char ar[20]{0};
			strcpy(ar, arr[i]->prop->category);
			int t = strcmp(ar, cat);
			TAG_LOGD(" str cmp result - %d", t);

			if (t == 0) {
				TAG_LOGD(tag,"Tag %d is equal", i);
				*index = i;
				return Tag::arr[i];
			}
			if (i == indHead-1) *index = i;

			else{TAG_LOGD("Tag not equal -> chack next");}
		}else{ESP_LOGE(tag," tag ind - %d have NULL pointer", i);}
	}
	return NULL;
}
