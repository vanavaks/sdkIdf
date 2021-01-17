/*
 * Tag.cpp
 *
 *  Created on: 14 џэт. 2021 у.
 *      Author: Vanavaks
 */

#include <esp_err.h>
#include <stddef.h>
#include <Tag.h>

uint16_t Tag::indHead = 0;

Tag::Tag() {
	// TODO Auto-generated constructor stub

}




err1_t Tag::get(char* key, char* val) {
	return ESP_ERR_TAG_OK;
}

Tag::~Tag() {
	// TODO Auto-generated destructor stub
}

/*Tag::Tag(const tagProp_t* tagProp, uint32_t v) {
	ESP_ERROR_CHECK(Tag::arrAdd(this));
	this->value.asui32 = v;
}*/

void Tag::init(const tagProp_t* tagProp, paramtype_t tagType){
	err1_t err;
		//reg propertyes
	if(tagProp == NULL){err = ESP_ERR_TAG_NULL_POINTER; goto err;}
		//chack type
	if(tagProp->type != tagType) {err = ESP_ERR_TYPE; goto err;}
		//reg Tag
	err = Tag::arrAdd(this);
	if (err != ESP_ERR_TAG_OK) {goto err;}
	else { this->prop = tagProp; }
err:	ESP_ERROR_CHECK(err);
}

err1_t Tag::init(const tagProp_t* tagProp){
	err1_t err;
		//reg propertyes
	if(tagProp == NULL){err = ESP_ERR_TAG_NULL_POINTER; goto err;}
		//reg Tag
	err = Tag::arrAdd(this);
	if (err != ESP_ERR_TAG_OK) {goto err;}
	this->prop = tagProp;
err:	return err;
}

Tag::Tag(const tagProp_t* tagProp) {
	init(tagProp, TAG_STR);
		//initialization of value
	//this->value.asstr = tagProp->val.asstr;
	value = tagProp->val;
}

Tag::Tag(const tagProp_t* tagProp, char* v) {
	init(tagProp, TAG_STR);
		//init value
	if(v != NULL){this->value.asstr = v;}
	else {value.asstr = tagProp->val.asstr;}
}

Tag::Tag(const tagProp_t* tagProp, uint32_t v) {
	ESP_ERROR_CHECK(init(tagProp));
		//init value
	ESP_ERROR_CHECK(set(v));
}

err1_t Tag::get(uint32_t* val) {
	//chack type
	if(this->prop->type != TAG_UI32) {return ESP_ERR_TYPE;}
	return ESP_ERR_TAG_OK;
}

err1_t Tag::set(uint32_t val) {
	if(this->prop->type != TAG_UI32) {return ESP_ERR_TYPE;}
	value.asui32 = val;
	return ESP_ERR_TAG_OK;
}

err1_t Tag::get(char* val) {
	//chack type
	if(this->prop->type != TAG_STR) {return ESP_ERR_TYPE;}
	//val = (char*)"Temp"; //value.asstr;
	return ESP_ERR_TAG_OK;
}

err1_t Tag::set(char* val) {
	//chack type
	if(this->prop->type != TAG_STR) {return ESP_ERR_TYPE;}
	value.asstr = val;
	return ESP_ERR_TAG_OK;
}

err1_t Tag::get(char* key, uint32_t* val) {
	return ESP_ERR_TAG_OK;
}
/*
err1_t Tag::set(char* key, char* val) {
	return ESP_ERR_TAG_OK;
}

err1_t Tag::get(char* val) {
	return ESP_ERR_TAG_OK;
}*/


err1_t Tag::arrAdd(Tag* tag) {
	if(tag == NULL) return ESP_ERR_TAG_NULL_POINTER;
	if(indHead >= CONFIG_TAG_ARR_SIZE) return ESP_ERR_ARR_OVERSIZE;
	tag->index = indHead;
	Tag::arr[indHead++] = tag;
	return ESP_ERR_TAG_OK;
}

err1_t Tag::saveVal(val_t v){
	if(prop->type == TAG_STR){
		value.asstr = v.asstr;
	} else if(prop->type == TAG_UI32){
		value.asui32 = v.asui32;
	} else if(prop->type == TAG_I32){
		value.asi32 = v.asi32;
	} else return ESP_ERR_TYPE;
	return ESP_ERR_TAG_OK;
}

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


