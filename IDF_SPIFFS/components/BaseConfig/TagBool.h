/*
 * TagBool.h
 *
 *  Created on: 16 квіт. 2021 р.
 *      Author: Vanavaks
 */

#ifndef COMPONENTS_BASECONFIG_TAGBOOL_H_
#define COMPONENTS_BASECONFIG_TAGBOOL_H_

#include "Tag.h"

class TagBool : public Tag{
public:
	TagBool(){};
	TagBool(const tagProp_t* tagProp);
	virtual ~TagBool();

	TagBool* operator=(TagBool t){
		setBL(t.getBL());
		return this;
	}
	/*bool operator=(TagBool t){
		setBL(t.getBL());
		return getBL();
	}*/
	/*TagBool* operator=(bool v){
		setBL(v);
		return this;
	}*/

	bool operator!(){
		return !this->getBL();
	}

	bool operator||(TagBool* t){
		return this->getBL() || t->getBL();
	}

	bool operator&&(TagBool* t){
		return this->getBL() && t->getBL();
	}
	bool operator||(bool v){
		return this->getBL() || v;
	}

	bool operator&&(bool v){
		return this->getBL() && v;
	}
};

#endif /* COMPONENTS_BASECONFIG_TAGBOOL_H_ */
