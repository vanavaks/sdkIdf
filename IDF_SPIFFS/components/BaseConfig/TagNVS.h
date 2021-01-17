/*
 * ParNVS.h
 *
 *  Created on: 14 џэт. 2021 у.
 *      Author: Vanavaks
 */

#ifndef COMPONENTS_BASECONFIG_TAGNVS_HPP_
#define COMPONENTS_BASECONFIG_TAGNVS_HPP_

#include <esp_err.h>
#include <nvs.h>

#include "../../main/main.h"
#include "Tag.h"


#define ESP_ERR_PAR_BASE                0x4100                     /*!< Starting number of error codes */
#define ESP_ERR_PAR_OK     				0
#define ESP_ERR_PAR_ERR           		(ESP_ERR_PAR_BASE + 0x01)   /*!< Requested resource not found */



enum PAR_openMode{
	openOnce,
	openEveryTime,
};

#define TAGNVS_OPEN_MODE openOnce

class TagNVS: public Tag{ //
public:

	TagNVS(const tagProp_t* tagProp, uint32_t v);// : Tag(tagProp, v);
	TagNVS(const tagProp_t* tagProp);

	virtual ~TagNVS();

private:
	TagNVS();
	static nvs_handle handle;
	const char* nsDef = "par";
	err1_t save();
	err1_t read() override;
	err1_t setPar(char* key);
	err1_t getPar(char* key);
	err1_t commitPar();
	err1_t initPar(); //PAR_openMode openMode

	void open(){if(TAGNVS_OPEN_MODE == openEveryTime)ESP_ERROR_CHECK(nvs_open(nsDef, NVS_READWRITE, &handle));};
	void close(){if(TAGNVS_OPEN_MODE == openEveryTime) nvs_close(handle);};
};

#endif /* COMPONENTS_BASECONFIG_TAGNVS_HPP_ */


//virtual Tag::~Tag();
	/*err1_t setUI8(char* key, uint8_t val);
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
