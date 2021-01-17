/*
 * Tag.h
 *
 *  Created on: 14 янв. 2021 г.
 *      Author: Vanavaks
 */

#ifndef COMPONENTS_BASECONFIG_TAG_HPP_
#define COMPONENTS_BASECONFIG_TAG_HPP_

#include <sys/_stdint.h>

#include "../../main/main.h"


#define CONFIG_TAG_ARR_SIZE 100

#define ESP_ERR_TAG_BASE                0x4200                     /*!< Starting number of error codes */
#define ESP_ERR_TAG_OK     				0
#define ESP_ERR_TAG_GEN           		(ESP_ERR_TAG_BASE + 0x01)
#define ESP_ERR_TAG_NULL_POINTER		(ESP_ERR_TAG_BASE + 0x02)
#define ESP_ERR_ARR_OVERSIZE 			(ESP_ERR_TAG_BASE + 0x03)
#define ESP_ERR_TYPE		 			(ESP_ERR_TAG_BASE + 0x04)

enum paramtype_t : uint8_t { TAG_BOOL, TAG_I8, TAG_UI8, TAG_I16, TAG_UI16, TAG_I32, TAG_UI32, TAG_FLOAT, TAG_CHAR, TAG_STR, TAG_PSWD };
#define SAVEBLE true

struct val_t{
	union {
		bool asbool;
		int8_t asi8;
		uint8_t asui8;
		int16_t asi16;
		uint16_t asui16;
		int32_t asi32;
		uint32_t asui32;
		float asfloat;
		char aschar;
		const char* asstr;
	} ;
};

struct tagProp_t{
	const char* name;
	const char* category;
	paramtype_t type;
	bool saveble;
	val_t val;
	/*val_t defVal;
	val_t minVal;
	val_t maxVal;*/
} ;



class Tag {
public:
	Tag(const tagProp_t* tagProp, val_t v);
	Tag(const tagProp_t* tagProp, char* v);
	Tag(const tagProp_t* tagProp, uint32_t v);
	Tag(const Tag& tag) : prop(tag.prop), value(tag.value){ESP_ERROR_CHECK(Tag::arrAdd(this));}


	Tag & operator= (const uint32_t v){ESP_ERROR_CHECK(set(v));return *this;}


	virtual ~Tag();

	err1_t set(uint32_t val);		//установка значения с проверкой типа
	//static err1_t set(char* key, uint32_t val); //установка значения с проверкой типа по ключу, не имея екземпляра тега

	err1_t get(uint32_t* val);
	static err1_t get(char* key, uint32_t* val);

	err1_t set(char* val);		//установка значения с проверкой типа
	static err1_t set(char* key, char* val); //установка значения с проверкой типа по ключу, не имея екземпляра тега

	err1_t get(char* val);
	static err1_t get(char* key, char* val);


	 //interfase for parameters implementing
	virtual err1_t read() = 0;
	/*virtual err1_t save();

	virtual err1_t setPar(char* key);
	virtual err1_t getPar(char* key);
	virtual err1_t commitPar();
	virtual err1_t initPar();*/


protected:
	Tag();
	Tag(const tagProp_t* tagProp);
	err1_t saveVal(val_t v);
	void init(const tagProp_t* tagProp, paramtype_t tagType);
	err1_t init(const tagProp_t* tagProp);

	const tagProp_t* prop;
	int16_t index = -1;  //-1 - noinit
	val_t value;

	static uint16_t indHead;
	static Tag* arr[CONFIG_TAG_ARR_SIZE];
	static err1_t arrAdd(Tag* tag);
	//static err1_t arrGet(uint16_t index, Tag* tag);

};

#endif /* COMPONENTS_BASECONFIG_TAG_HPP_ */


/*//static void registerTag(const tagProp_t* tagProp);
	//static void registerTags(const tagProp_t* tagProp, uint16_t num);
	//Tag(const tagProp_t* tagProp, val_t v);


	err1_t setUI16(char* key, uint16_t val);
	err1_t setUI32(char* key, uint32_t val);
	err1_t setI8(char* key, int8_t val);
	err1_t setI16(char* key, int16_t val);
	err1_t setI32(char* key, int32_t val);
	err1_t setStr(char* key, char* val);
	err1_t setUI8(char* key, uint8_t val);
	err1_t setUI16(char* key, uint16_t val);
	err1_t setUI32(char* key, uint32_t val);
	err1_t setI8(char* key, int8_t val);
	err1_t setI16(char* key, int16_t val);
	err1_t setI32(char* key, int32_t val);

	err1_t getUI8(char* key, uint8_t* val);
	err1_t getUI16(char* key, uint16_t* val);
	err1_t getUI32(char* key, uint32_t* val);
	err1_t getI8(char* key, int8_t* val);
	err1_t getI16(char* key, int16_t* val);
	err1_t getI32(char* key, int32_t* val);
	err1_t getUI8(char* key, uint8_t* val);
	err1_t getUI16(char* key, uint16_t* val);
	err1_t getUI32(char* key, uint32_t* val);
	err1_t getI8(char* key, int8_t* val);
	err1_t getI16(char* key, int16_t* val);
	err1_t getI32(char* key, int32_t* val);
	err1_t getStr(char* key, char* val);*/

