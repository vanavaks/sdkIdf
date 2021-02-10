/*
 * Tui32.h
 *
 *  Created on: 8 февр. 2021 г.
 *      Author: Vanavaks
 */

#ifndef COMPONENTS_BASECONFIG_TUI32_H_
#define COMPONENTS_BASECONFIG_TUI32_H_

#include "Tag.h"


struct ui32_p_t{

};
/*
#define CAT_NUMS 		10
#define CAT_TAG_NUMS 	10
class Tcat{
public:
	Tcat(char* _name, uint16_t _size){};

	const char* getName(){return name;}
	uint16_t 	getInd(){return ind;}
	void 		addTag(const Tag* t);
	Tag* 		getTag(const uint16_t n);
	uint16_t 	getNums();


private:
	const char* name;
	uint16_t ind;
	uint16_t size = CAT_TAG_NUMS;
	Tag* arr;									//pointer to tags array dinamically created

	static Tcat arrCats[CAT_NUMS];
	static void arrInit();
	static err1_t arrAdd(Tcat* cat);
	static uint16_t indHead;
};
*/
class Tui32: public Tag {
public:
	Tui32(const ui32_p_t* prop);
	virtual ~Tui32();
	void set(uint32_t v){};
	uint32_t get();

	void Print();
	size_t size(){return sizeof(uint32_t);}

private:
	uint32_t val = 0;
	const ui32_p_t* prop;		// base ???
	//Tcat category; 				// base
};

#endif /* COMPONENTS_BASECONFIG_TUI32_H_ */
