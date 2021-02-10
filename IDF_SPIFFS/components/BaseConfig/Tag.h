/*
 * Tag.h
 *
 *  Created on: 14 янв. 2021 г.
 *      Author: Vanavaks
 */

#ifndef COMPONENTS_BASECONFIG_TAG_HPP_
#define COMPONENTS_BASECONFIG_TAG_HPP_

#include <sys/_stdint.h>

#include <nvs.h>
#include "../../main/main.h"

#ifdef __cplusplus
extern "C" {
#endif

#define Pstr(_key, _cat, _savebl, _def) 	{.KeyName = (_key), .category = (_cat), .type = TAG_STR, .saveble = (_savebl), .val = { .asstr = (char*)(_def) }}
#define Pui32(_key, _cat, _savebl, _def) 	{.KeyName = (_key), .category = (_cat), .type = TAG_UI32, .saveble = (_savebl), .val = { .asi32 = (int32_t)(_def) }}
#define Pbool(_key, _cat, _savebl, _def) 	{.KeyName = (_key), .category = (_cat), .type = TAG_BOOL, .saveble = (_savebl), .val = { .asbool = (bool)(_def) }}
#define TagIp(_key, _cat, _savebl, _def) 	{.KeyName = (_key), .category = (_cat), .type = TAG_IP, .saveble = (_savebl), .val = { .asui32 = (uint32_t)(_def) }}
#define TagTime(_key, _cat, _savebl, _def) 	{.KeyName = (_key), .category = (_cat), .type = TAG_Time, .saveble = (_savebl), .val = { .asi32 = (int32_t)(_def) }}

enum PAR_openMode{
	openOnce,
	openEveryTime,
};

//#define TAGNVS_OPEN_MODE openOnce
#define TAGNVS_OPEN_MODE_EVERY
#define TAG_TRUE_STR 			"on"
#define TAG_FALSE_STR			"off"

#define CONFIG_TAG_ARR_SIZE 100
#if (CONFIG_TAG_ARR_SIZE < 10)
#error "Array size must be > 10"
#endif


#define TAG_VAL_STR_MAX_SIZE 			20
#define TAG_VAL_STR_MAX_SIZE_STR 		"20"
#define CHAR_BUFF_SIZE 20

#define ESP_ERR_TAG_BASE                0x4200                     /*!< Starting number of error codes */
#define ESP_ERR_TAG_OK     				0
#define ESP_ERR_TAG_GEN           		(ESP_ERR_TAG_BASE + 0x01)
#define ESP_ERR_TAG_NULL_POINTER		(ESP_ERR_TAG_BASE + 0x02)
#define ESP_ERR_ARR_OVERSIZE 			(ESP_ERR_TAG_BASE + 0x03)
#define ESP_ERR_TYPE		 			(ESP_ERR_TAG_BASE + 0x04)
#define ESP_INF_TAG_SAVE_DEF			(ESP_ERR_TAG_BASE + 0x05)
#define ESP_ERR_TAG_SAVE_DEF			(ESP_ERR_TAG_BASE + 0x06)
#define ESP_ERR_TAG_NAME				(ESP_ERR_TAG_BASE + 0x07)

#define SAVE_NVS_DEF_VAL
enum paramtype_t : uint8_t { TAG_BOOL, TAG_I8, TAG_UI8, TAG_I16, TAG_UI16, TAG_I32, TAG_UI32, TAG_FLOAT, TAG_CHAR, TAG_STR, TAG_PSWD, TAG_IP, TAG_T };
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
		char* asstr;
	} ;
};

struct tagProp_t{
	const char* KeyName;
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
	Tag(const tagProp_t* tagProp);
	Tag(const Tag& tag) : prop(tag.prop), value(tag.value){ESP_ERROR_CHECK(Tag::arrAdd(this));}
	virtual ~Tag();

	static void begin();

	//Tag & operator= (const uint32_t v){ESP_ERROR_CHECK(set(v));return *this;}


	err1_t set(uint32_t val);		//установка значения с проверкой типа
	//static err1_t set(char* key, uint32_t val); //установка значения с проверкой типа по ключу, не имея екземпляра тега
	err1_t setUI32(uint32_t val){ return set(val); }

	//err1_t get(uint32_t* val);
	//static err1_t get(char* key, uint32_t* val);
	uint32_t getUI32();

	err1_t set(const char* val, size_t size);		/* сохранение созданием новой строки в куче c удалением из кучи предыдущей */
	err1_t setSTR(const char* val, size_t size){return set(val, size);}
	err1_t setNew(char* val);
	//static err1_t set(const char* key, const char* val){return 0;}; //установка значения с проверкой типа по ключу, не имея екземпляра тега

	//err1_t get(char** val);//static err1_t get(char* key, char* val);
	char* getStr();

	err1_t set(bool val);
	err1_t set(uint8_t val);
	err1_t set(int32_t val);
	err1_t set(float val);

	bool getBL();
	uint8_t getUI8();
	int32_t getI32();
	float getFL();


	static void printAll();
	void Print();
	void PrintAdr();
			/* return number of tags */
	static uint16_t getTagNumb(){return indHead;}
	static Tag* getByIndex(uint16_t i){if(i<indHead) return arr[i]; else return NULL;}
	static Tag* getNextByCategory(uint16_t* index, const char* cat);
			/* setting a tag by key */
	static err1_t set(const char* key, const char* value);
			/* return tag's value size */
	size_t size();

			/* getters */
	const char* getKey() {return this->prop->KeyName;}
	paramtype_t getType() {return this->prop->type;}
	bool 		isSaveble() {return this->prop->saveble;}

protected:
	Tag();
	const tagProp_t* prop;
	val_t value;
	int16_t index = -1;  //-1 - noinit



	err1_t saveVal(val_t v);
	void init(const tagProp_t* tagProp);
	static Tag* findTag(const char* key);






	static nvs_handle handle;
	static uint16_t indHead;
	static Tag* arr[CONFIG_TAG_ARR_SIZE];
	static bool isInit;
	static uint32_t saveCnt;

	static void arrInit();
	static err1_t arrAdd(Tag* tag);
	static bool chackKey(const char* name);

	/* ============================== NVS ===================================== */

			/* Initialization NVS system */
	static void initNVS();
			/* open nvs for value saving */
	inline void open(){
#ifdef TAGNVS_OPEN_MODE_EVERY
		 ESP_ERROR_CHECK(nvs_open("par", NVS_READWRITE, &handle));
#endif
	};
			/* read from NVS */
	void read();
			/* save to NVS */
	void save();
			/* close nvs after value saving */
	inline void close(){
#ifdef TAGNVS_OPEN_MODE_EVERY
		nvs_close(handle);
#endif
	};
};

#ifdef __cplusplus
}
#endif

#endif /* COMPONENTS_BASECONFIG_TAG_HPP_ */

