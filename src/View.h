/*
 * Screen.h
 *
 *  Created on: 1 lis 2019
 *      Author: lechu
 */

#ifndef VIEW_H_
#define VIEW_H_

#include <NumberEditor.h>

namespace HMI {



class View {
public:
	typedef enum{
		STRING, NUMBER_FIELD, EDIT_FIELD,
	}ViewType;
protected:
	View * next = nullptr;
	uint32_t lines = 1;
public:
	View(uint32_t screenLines){ lines = screenLines; }

	virtual ~View(){;}
};

	bool show(){

	}


} /* namespace HMI */

#endif /* VIEW_H_ */
