/*
 * Tg.h
 *
 *  Created on: 8 פוגנ. 2021 ד.
 *      Author: Vanavaks
 */

#ifndef COMPONENTS_BASECONFIG_TG_H_
#define COMPONENTS_BASECONFIG_TG_H_

/*
 * Tg.h
 *
 *  Created on: 14 ÿםג. 2021 ד.
 *      Author: Vanavaks
 */

#ifndef COMPONENTS_BASECONFIG_TG_HPP_
#define COMPONENTS_BASECONFIG_TG_HPP_

#include <sys/_stdint.h>

#include <nvs.h>
#include "../../main/main.h"

#ifdef __cplusplus
extern "C" {
#endif

#define Pstr(_key, _cat, _savebl, _def) 	{.KeyName = (_key), .category = (_cat), .type = TG_STR, .saveble = (_savebl), .val = { .asstr = (char*)(_def) }}
#define Pui32(_key, _cat, _savebl, _def) 	{.KeyName = (_key), .category = (_cat), .type = TG_UI32, .saveble = (_savebl), .val = { .asi32 = (int32_t)(_def) }}
#define Pbool(_key, _cat, _savebl, _def) 	{.KeyName = (_key), .category = (_cat), .type = TG_BOOL, .saveble = (_savebl), .val = { .asbool = (bool)(_def) }}
#define TgIp(_key, _cat, _savebl, _def) 	{.KeyName = (_key), .category = (_cat), .type = TG_IP, .saveble = (_savebl), .val = { .asui32 = (uint32_t)(_def) }}
#define TgTime(_key, _cat, _savebl, _def) 	{.KeyName = (_key), .category = (_cat), .type = TG_Time, .saveble = (_savebl), .val = { .asi32 = (int32_t)(_def) }}

enum PAR_openMode{
	openOnce,
	openEveryTime,
};

//#define TGNVS_OPEN_MODE openOnce
#define TGNVS_OPEN_MODE_EVERY
#define TG_TRUE_STR 			"on"
#define TG_FALSE_STR			"off"

#define CONFIG_TG_ARR_SIZE 100
#if (CONFIG_TG_ARR_SIZE < 10)
#error "Array size must be > 10"
#endif


#define TG_VAL_STR_MAX_SIZE 			20
#define TG_VAL_STR_MAX_SIZE_STR 		"20"
#define CHAR_BUFF_SIZE 20

#define ESP_ERR_TG_BASE                0x4200                     /*!< Starting number of error codes */
#define ESP_ERR_TG_OK     				0
#define ESP_ERR_TG_GEN           		(ESP_ERR_TG_BASE + 0x01)
#define ESP_ERR_TG_NULL_POINTER		(ESP_ERR_TG_BASE + 0x02)
#define ESP_ERR_ARR_OVERSIZE 			(ESP_ERR_TG_BASE + 0x03)
#define ESP_ERR_TYPE		 			(ESP_ERR_TG_BASE + 0x04)
#define ESP_INF_TG_SAVE_DEF			(ESP_ERR_TG_BASE + 0x05)
#define ESP_ERR_TG_SAVE_DEF			(ESP_ERR_TG_BASE + 0x06)
#define ESP_ERR_TG_NAME				(ESP_ERR_TG_BASE + 0x07)

#define SAVE_NVS_DEF_VAL
enum paramtype_t : uint8_t { TG_BOOL, TG_I8, TG_UI8, TG_I16, TG_UI16, TG_I32, TG_UI32, TG_FLOAT, TG_CHAR, TG_STR, TG_PSWD, TG_IP, TG_T };
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

struct tgProp_t{
	const char* KeyName;
	const char* category;
	paramtype_t type;
	bool saveble;
	val_t val;
	/*val_t defVal;
	val_t minVal;
	val_t maxVal;*/
} ;



class Tg {
public:
	Tg(const tgProp_t* tgProp);
	virtual ~Tg();

	static void begin();


	static void printAll();
	void Print();
			/* return number of tgs */
	static uint16_t getTgNumb(){return indHead;}
	static Tg* getByIndex(uint16_t i){if(i<indHead) return arr[i]; else return NULL;}
	static Tg* getNextByCategory(uint16_t* index, const char* cat);
			/* setting a tg by key */
	static err1_t set(const char* key, const char* value);
			/* return tg's value size */
	virtual size_t size();
	virtual void print();

			/* getters */
	const char* getKey() {return this->prop->KeyName;}
	paramtype_t getType() {return this->prop->type;}
	bool 		isSaveble() {return this->prop->saveble;}

protected:
	Tg();
	const tgProp_t* prop;
	int16_t index = -1;  //-1 - noinit

	err1_t saveVal(val_t v);
	void init(const tgProp_t* tgProp);
	static Tg* findTg(const char* key);


	static nvs_handle handle;
	static uint16_t indHead;
	static Tg* arr[CONFIG_TG_ARR_SIZE];
	static bool isInit;

	static void arrInit();
	static err1_t arrAdd(Tg* tg);
	static bool chackKey(const char* name);

	/* ============================== NVS ===================================== */

			/* Initialization NVS system */
	static void initNVS();
			/* open nvs for value saving */
	inline void open(){
#ifdef TGNVS_OPEN_MODE_EVERY
		 ESP_ERROR_CHECK(nvs_open("par", NVS_READWRITE, &handle));
#endif
	};
			/* read from NVS */
	void read();
			/* save to NVS */
	void save();
			/* close nvs after value saving */
	inline void close(){
#ifdef TGNVS_OPEN_MODE_EVERY
		nvs_close(handle);
#endif
	};
};

#ifdef __cplusplus
}
#endif

#endif /* COMPONENTS_BASECONFIG_TG_HPP_ */


#endif /* COMPONENTS_BASECONFIG_TG_H_ */
