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

uint16_t Tag::indHead = 0;
nvs_handle Tag::handle = 0;
uint32_t Tag::saveCnt = 0;
Tag* Tag::arr[CONFIG_TAG_ARR_SIZE];

const char* tag = "Tag";
const char* nsDef = "par";

Tag::Tag() {}
Tag::~Tag() {}


void Tag::init(const tagProp_t* tagProp){
	err1_t err;
		//check properties pointer
	if(tagProp == NULL){err = ESP_ERR_TAG_NULL_POINTER; goto err;}
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
			value.asstr = tagProp->val.asstr;
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


err1_t Tag::get(char* key, char* val) {
	return ESP_ERR_TAG_OK;
}


err1_t Tag::set(uint32_t val) {
	if(this->prop->type != TAG_UI32) {return ESP_ERR_TYPE;}
	value.asui32 = val;
	if(prop->saveble) save();
	return ESP_ERR_TAG_OK;
}



char* Tag::getStr(){
	if(prop->type == TAG_STR) return value.asstr;
	else {
		ESP_ERROR_CHECK(ESP_ERR_TYPE);
		return NULL;
	}
}

uint32_t Tag::getUI32(){
	if(prop->type == TAG_UI32) return value.asui32;
	else {
		ESP_ERROR_CHECK(ESP_ERR_TYPE);
		return 0;
	}
}

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


err1_t Tag::set(char* val) {
	if(this->prop->type != TAG_STR) {return ESP_ERR_TYPE;}
	value.asstr = val;
	if(prop->saveble) save();
	return ESP_ERR_TAG_OK;
}

err1_t Tag::get(char* key, uint32_t* val) {
	return ESP_ERR_TAG_OK;
}

//==================================== NVS ==============================================//

void Tag::initNVS() {
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

	}
	else{
		ESP_ERROR_CHECK(err);
	}

	if(err == ESP_OK){
		ESP_ERROR_CHECK(nvs_open(nsDef, NVS_READWRITE, &handle));
	}
}

void Tag::read() {
	open();
	err1_t err = ESP_OK;

	if(prop->type == TAG_STR){
		size_t size;//= CHAR_BUFF_SIZE;
		err = nvs_get_str(handle, prop->name, NULL, &size);	//reading present and size of string
		if(err == ESP_OK){	//key present, string may have zero size
			value.asstr = (char*) malloc(size + 1);	//allocating of memory; during reading value didn't need free previos pointer
			if(value.asstr == NULL) {
				ESP_LOGE(tag, "No free heap malloc null pointer");
				ESP_ERROR_CHECK(ESP_ERR_TAG_NULL_POINTER)
			}
				//----------here must be free of value.asstr pointer----------
			err = nvs_get_str(handle, prop->name, value.asstr, &size);
			ESP_LOGI(tag, "NVS get value %s = %s", prop->name, value.asstr);
		}
		if(err != ESP_OK) value.asstr = prop->val.asstr;
	}

	else if(prop->type == TAG_UI32){
		err = nvs_get_u32(handle, prop->name, &value.asui32);
		if(err != ESP_OK) value.asui32 = prop->val.asui32;
	}
	else if(prop->type == TAG_UI8){
		unsigned char v;
		err = nvs_get_u8(handle, prop->name, &v);
		value.asui8 = (uint8_t)v;
		if(err != ESP_OK) value.asui8 = prop->val.asui8;
	}
	else if(prop->type == TAG_I32){
		err = nvs_get_i32(handle, prop->name, &value.asi32);
		if(err != ESP_OK) value.asi32 = prop->val.asi32;
	}
	else if(prop->type == TAG_FLOAT){
		float v;
		size_t size = sizeof(v);
		err = nvs_get_blob(handle, prop->name, &value.asfloat, &size);
		if(err != ESP_OK) value.asfloat = prop->val.asfloat;
	}
	else if(prop->type == TAG_BOOL){
		unsigned char v;
		err = nvs_get_u8(handle, prop->name, &v);
		if(v > 0) value.asbool = true;
		else  value.asbool = false;
		if(err != ESP_OK) value.asbool = prop->val.asbool;
	}
	else{
		ESP_ERROR_CHECK(ESP_ERR_TYPE)	//crash bad type
	}

	switch (err){

		case ESP_ERR_NVS_INVALID_NAME:
			ESP_LOGW(tag, "Reading absent parameter in NVS %s, err= %d",prop->name, err);
			break;
		case ESP_OK:
			break;
		default:
			ESP_LOGW(tag, "Reading parameter %s, err= %d", prop->name, err);
		;
	}
	close();
}

void Tag::save() {
	open();
	err1_t err = ESP_OK;
	ESP_LOGI(tag, "Trying to save parameter %s",prop->name);
	saveCnt++;
	if(prop->type == TAG_STR){
		err = nvs_set_str(handle, prop->name, value.asstr);
	}else if(prop->type == TAG_UI32){
		err = nvs_set_u32(handle, prop->name, value.asui32);
	}else if(prop->type == TAG_BOOL){
		err = nvs_set_u8(handle, prop->name, value.asbool);
	}else if(prop->type == TAG_UI8){
		err = nvs_set_u8(handle, prop->name, value.asui8);
	}else if(prop->type == TAG_I32){
		err = nvs_set_i32(handle, prop->name, value.asi32);
	}else if(prop->type == TAG_FLOAT){
		err = nvs_set_blob(handle, prop->name, &value.asfloat,sizeof(float));
	}else {ESP_ERROR_CHECK(ESP_ERR_TYPE); saveCnt--;}

	if(err != ESP_OK) ESP_LOGW(tag, "saving parameter %s, err= %d",prop->name, err);
	close();
}

void Tag::Print(){
	printf("Tag: category - %s, name - %s, ",prop->category , prop->name);
	if(prop->type == TAG_STR) printf("type - str, val = %s, def val = %s", value.asstr , prop->val.asstr);
	else if(prop->type == TAG_UI32) printf("type - ui32, val = %d, def val = %d, ", value.asui32, prop->val.asui32);
	else if(prop->type == TAG_I32) printf("type - i32, val = %d, def val = %d, ", value.asi32, prop->val.asi32);
	else if(prop->type == TAG_UI8) printf("type - ui8, val = %d, def val = %d, ", value.asi8, prop->val.asi8);
	else if(prop->type == TAG_FLOAT) printf("type - float, val = %f, def val = %f, ", value.asfloat, prop->val.asfloat);
	else if(prop->type == TAG_BOOL) printf("type - bool, val = %d, def val = %d, ", value.asbool, prop->val.asbool);
	printf("arr index - %d \r\n", index);
}

void Tag::printAll(){
	for(int i=0; i<indHead; i++){
		arr[i]->Print();
	}
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


/*err1_t Tag::get(uint32_t* val) {
	err1_t err = ESP_OK;
	if(this->prop->type != TAG_UI32) {err = ESP_ERR_TYPE;}	//chack type
	if(val == NULL){err = ESP_ERR_TAG_NULL_POINTER;}		//chack pointer
	if(err == ESP_OK) {*val = value.asui32;}
	else {ESP_ERROR_CHECK(err);} 	//crash
	return ESP_OK;
}*/
/*err1_t Tag::get(char** val) {
	err1_t err = ESP_OK;
	if(this->prop->type != TAG_STR) {err = ESP_ERR_TYPE;}	//chack type
	if(val == NULL){err = ESP_ERR_TAG_NULL_POINTER;}		//chack pointer
	if(err == ESP_OK) {*val = value.asstr;}
	else {ESP_ERROR_CHECK(err);} 	//crash
	return ESP_OK;
}*/

/*
 *
 * ------ver 1 with default saving of data to NVS -----------
err1_t Tag::readStr() {
	open();
	size_t size;//= CHAR_BUFF_SIZE;
	err1_t err = nvs_get_str(handle, prop->name, NULL, &size);	//reading present and size of string
	if(err == ESP_OK){	//key present, string may have zero size
		value.asstr = (char*) malloc(size + 1);	//allocating of memory; during reading value didn't need free previos pointer
		if(value.asstr != NULL) {
				//----------here must be free of value.asstr pointer----------
			err = nvs_get_str(handle, prop->name, value.asstr, &size);
			if(err != ESP_OK){
				value.asstr = prop->val.asstr;
				ESP_LOGW(tag, "Reading str parameter %s , err= %s",prop->name, err);
			}else{ ESP_LOGI(tag, "Reading parameter %s = %s, size = %d",prop->name, value.asstr, size);	}
		}
		else{//not enough free heap
			ESP_LOGE(tag, "No free heap malloc null pointer");
			ESP_ERROR_CHECK(err)
		}
	}
	else if(err == ESP_ERR_NVS_INVALID_NAME){ //set property with default value
		value.asstr = prop->val.asstr;
		ESP_LOGW(tag, "Reading absent parameter %s set default %s, err= %s",prop->name, value.asstr, err);
		err = nvs_set_str(handle, prop->name, prop->val.asstr);
		if(err != ESP_ERR_TAG_OK) ESP_LOGE(tag, "saving parameter %s, err= %s",prop->name, err);
	}
	else{
		ESP_LOGW(tag, "Reading str parameter %s , err= %s",prop->name, err);
		value.asstr = prop->val.asstr;
	}
	close();
	return ESP_ERR_TAG_OK;
}

err1_t Tag::readUI32() {
	err1_t err = nvs_get_u32(handle, prop->name, &value.asui32);	//reading value
	if(err == ESP_OK) return ESP_OK;
	value.asui32 = prop->val.asui32;
#ifdef  ESP_ERR_TAG_SAVE_DEF
	if(err == ESP_ERR_NVS_INVALID_NAME){ //set property with default value
		err = nvs_set_u32(handle, prop->name, prop->val.asui32);	//savin def value
		if(err != ESP_OK) return ESP_ERR_TAG_SAVE_DEF;
		return ESP_INF_TAG_SAVE_DEF;
	}
#endif
	return err;
}

*/
/*

 ------ver 2 without default saving of data to NVS -----------
err1_t Tag::readUI32() {
	open();
	err1_t err = nvs_get_u32(handle, prop->name, &value.asui32);	//reading present and size of string

	if(err == ESP_ERR_NVS_INVALID_NAME){ //set property with default value
		value.asui32 = prop->val.asui32;
		ESP_LOGW(tag, "Reading absent parameter %s set default %s, err= %s",prop->name, value.asstr, err);
		err = nvs_set_u32(handle, prop->name, prop->val.asui32);
		if(err != ESP_ERR_TAG_OK) ESP_LOGE(tag, "saving parameter %s, err= %s",prop->name, err);
	}else if(err == ESP_OK){	//key present, string may have zero size

	}
	else{
		ESP_LOGW(tag, "Reading str parameter %s , err= %s",prop->name, err);
		value.asui32 = prop->val.asui32;
	}
	close();
	return ESP_ERR_TAG_OK;
}
*/


/*
 * Tag::Tag(const tagProp_t* tagProp, val_t v = NULL) {
		//reg Tag
	ESP_ERROR_CHECK(Tag::arrAdd(this));
		//reg propertyes
	if(tagProp == NULL){ESP_ERROR_CHECK(ESP_ERR_TAG_NULL_POINTER);}
	this->prop = tagProp;
		//init value
	if(v != NULL){this->value = v;}
	else {value = tagProp->val;}
}*/

/*Tag::Tag(const tagProp_t* tagProp, uint32_t v) {
	ESP_ERROR_CHECK(Tag::arrAdd(this));
	this->value.asui32 = v;
}*/


/*
err1_t Tag::saveStr() {
	open();
	err1_t err = nvs_set_str(handle, prop->name, prop->val.asstr);
	if(err != ESP_ERR_TAG_OK) ESP_LOGW(tag, "saving parameter %s, err= %s",prop->name, err);
	close();
	return ESP_ERR_TAG_OK;
}
*/



/*
err1_t Tag::set(char* key, char* val) {
	return ESP_ERR_TAG_OK;
}

err1_t Tag::get(char* val) {
	return ESP_ERR_TAG_OK;
}*/


/*err1_t Tag::init(const tagProp_t* tagProp){
	err1_t err;
		//reg propertyes
	if(tagProp == NULL){err = ESP_ERR_TAG_NULL_POINTER; goto err;}
		//reg Tag
	err = Tag::arrAdd(this);
	if (err != ESP_ERR_TAG_OK) {goto err;}
	this->prop = tagProp;
err:	return err;
}*/

/*Tag::Tag(const tagProp_t* tagProp) {
	init(tagProp, TAG_STR);
		//initialization of value
	//this->value.asstr = tagProp->val.asstr;
	value = tagProp->val;
}*/
