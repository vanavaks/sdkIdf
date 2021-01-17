/*
 * ParBase.h
 *
 *  Created on: 14 џэт. 2021 у.
 *      Author: Vanavaks
 */

#ifndef COMPONENTS_BASECONFIG_PARBASE_H_
#define COMPONENTS_BASECONFIG_PARBASE_H_

#include "main.h"


class ParBase {
public:
	ParBase();
	virtual err1_t set(char* key);
	virtual err1_t get(char* key);
	virtual err1_t commit();
	virtual err1_t init();
	virtual ~ParBase();
};

#endif /* COMPONENTS_BASECONFIG_PARBASE_H_ */
