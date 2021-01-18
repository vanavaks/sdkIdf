/*
 * ParNVS.cpp
 *
 *  Created on: 14 џэт. 2021 у.
 *      Author: Vanavaks
 */

#include <TagNVS.h>

struct tagProp_t;
#if 0
nvs_handle TagNVS::handle = 0;



err1_t TagNVS::setPar(char* key) {
	return ESP_ERR_TAG_OK;
}

err1_t TagNVS::getPar(char* key) {
	return ESP_ERR_TAG_OK;
}

err1_t TagNVS::commitPar() {
	return ESP_ERR_TAG_OK;
}

TagNVS::TagNVS() {
}

err1_t TagNVS::save() {
	return ESP_ERR_PAR_OK;
}

err1_t TagNVS::read() {
	open();
	//ESP_ERROR_CHECK(nvs_set_str(handle, this->prop->name, this->prop->val.asstr));
	close();
	return ESP_ERR_PAR_OK;
}

TagNVS::TagNVS(const tagProp_t* tagProp) {
	Tag::Tag(tagProp);
	saveble option reading value
	if(tagProp->saveble)
		ESP_ERROR_CHECK(read());
}

TagNVS::~TagNVS() {
	//~base//::~Tag();
	// TODO Auto-generated destructor stub
}

TagNVS::TagNVS(const tagProp_t* tagProp, uint32_t v) {
	 ;
}

/*
err1_t ParNVS::set(char* key) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::get(char* key) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::commit() {
	return ESP_ERR_PAR_OK;
}
*/
err1_t TagNVS::initPar() {
	if(TAGNVS_OPEN_MODE == openOnce){
		ESP_ERROR_CHECK(nvs_open(nsDef, NVS_READWRITE, &handle));
	}
	return ESP_ERR_PAR_OK;
}

/*TagNVS::TagNVS(const tagProp_t* tagProp) {
	Tag::value = tagProp->val;
}*/
/*
err1_t ParNVS::setUI8(char* key, uint8_t val) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::setUI16(char* key, uint16_t val) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::setUI32(char* key, uint32_t val) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::setI8(char* key, int8_t val) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::setI16(char* key, int16_t val) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::setI32(char* key, int32_t val) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::setUI8(char* key, uint8_t val) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::setUI16(char* key, uint16_t val) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::setUI32(char* key, uint32_t val) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::setI8(char* key, int8_t val) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::setI16(char* key, int16_t val) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::setI32(char* key, int32_t val) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::getUI8(char* key, uint8_t* val) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::getUI16(char* key, uint16_t* val) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::getUI32(char* key, uint32_t* val) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::getI8(char* key, int8_t* val) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::getI16(char* key, int16_t* val) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::getI32(char* key, int32_t* val) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::getUI8(char* key, uint8_t* val) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::getUI16(char* key, uint16_t* val) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::getUI32(char* key, uint32_t* val) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::getI8(char* key, int8_t* val) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::getI16(char* key, int16_t* val) {
	return ESP_ERR_PAR_OK;
}

err1_t ParNVS::getI32(char* key, int32_t* val) {
	return ESP_ERR_TAG_OK;
}

err1_t ParNVS::initPar(PAR_openMode openMode) {
	return ESP_ERR_TAG_OK;
}
*/
/*err1_t TagNVS::getStr(char* key, char* val) {
	open(NVS_READONLY);
	close();
	return ESP_ERR_PAR_OK;
}



err1_t TagNVS::setStr(char* key, char* val) {
	open();
	ESP_ERROR_CHECK(nvs_set_str(handle, key, val));
	close();
	return 0;
}*/

#endif
