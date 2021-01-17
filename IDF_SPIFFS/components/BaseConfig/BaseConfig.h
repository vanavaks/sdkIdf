/*
 * BaseConfig.h
 *
 *  Created on: 14 џэт. 2021 у.
 *      Author: Vanavaks
 */

#ifndef COMPONENTS_BASECONFIG_BASECONFIG_H_
#define COMPONENTS_BASECONFIG_BASECONFIG_H_

#include "main.h"
enum paramtype_t : uint8_t { PAR_BOOL, PAR_I8, PAR_UI8, PAR_I16, PAR_UI16, PAR_I32, PAR_UI32, PAR_FLOAT, PAR_CHAR, PAR_STR, PAR_PSWD };

struct __packed param_t {
  paramtype_t _type;
  const char* _name;
  const char* _descr;
  uint16_t _size;
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
  } _default;
};

#define PARAM_BOOL(name, descr, def) { ._type = PAR_BOOL, ._name = (name), ._descr = (descr), ._size = sizeof(bool), ._default = { .asbool = (def) } }
#define PARAM_I8(name, descr, def) { ._type = PAR_I8, ._name = (name), ._descr = (descr), ._size = sizeof(int8_t), ._default = { .asi8 = (def) } }
#define PARAM_UI8(name, descr, def) { ._type = PAR_UI8, ._name = (name), ._descr = (descr), ._size = sizeof(uint8_t), ._default = { .asui8 = (def) } }
#define PARAM_I16(name, descr, def) { ._type = PAR_I16, ._name = (name), ._descr = (descr), ._size = sizeof(int16_t), ._default = { .asi16 = (def) } }
#define PARAM_UI16(name, descr, def) { ._type = PAR_UI16, ._name = (name), ._descr = (descr), ._size = sizeof(uint16_t), ._default = { .asui16 = (def) } }
#define PARAM_I32(name, descr, def) { ._type = PAR_I32, ._name = (name), ._descr = (descr), ._size = sizeof(int32_t), ._default = { .asi32 = (def) } }
#define PARAM_UI32(name, descr, def) { ._type = PAR_UI32, ._name = (name), ._descr = (descr), ._size = sizeof(uint32_t), ._default = { .asui32 = (def) } }
#define PARAM_FLOAT(name, descr, def) { ._type = PAR_FLOAT, ._name = (name), ._descr = (descr), ._size = sizeof(float), ._default = { .asfloat = (def) } }
#define PARAM_CHAR(name, descr, def) { ._type = PAR_CHAR, ._name = (name), ._descr = (descr), ._size = sizeof(char), ._default = { .aschar = (def) } }
#define PARAM_STR(name, descr, size, def) { ._type = PAR_STR, ._name = (name), ._descr = (descr), ._size = (size), ._default = { .asstr = (def) } }
#define PARAM_PSWD(name, descr, size, def) { ._type = PAR_PSWD, ._name = (name), ._descr = (descr), ._size = (size), ._default = { .asstr = (def) } }

const char EMPTYSTR[]  = ""; //PROGMEM
const char CONFIG_FILE_NAME[]  = "/config.json"; //PROGMEM
/*
class BaseConfig {
public:
  static const uint8_t ERR_INDEX = 0xFF;

  BaseConfig(const param_t *params, uint8_t paramCount) : _params((param_t*)params), _paramCount(paramCount) {}

  uint8_t paramCount() const {
    return _paramCount;
  }
  bool getParam(uint8_t index, param_t &param) const;
  uint8_t findParam(const char *name) const;
  uint8_t findParam_P(const char* name) const;
  paramtype_t paramType(uint8_t index) const;
  const char* paramName(uint8_t index) const;
  const char* paramDescr(uint8_t index) const;
  uint16_t paramSize(uint8_t index) const;
  virtual void *getParamPtr(uint8_t index) = 0;

  virtual void clear();
  virtual bool load();
  virtual bool save();

  virtual String toString();
  virtual bool fromString(const String &str);

protected:
  static const uint16_t JSON_BUF_SIZE = 1024;

  virtual void read(const JsonDocument &doc);
  virtual void write(JsonDocument &doc);

  struct __packed {
    param_t *_params;
    uint8_t _paramCount;
  };
};

*/
#endif /* COMPONENTS_BASECONFIG_BASECONFIG_H_ */
