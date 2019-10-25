/*
 * Praca.h
 *
 *  Created on: 25 paź 2019
 *      Author: lechu
 */

#ifndef PRACA_H_
#define PRACA_H_

#include "Settings.h"

class Praca {
private:


public:
	static constexpr uint32_t PRACA_POLL_INTERVAL_MILIS = 12;

	Praca();
	virtual ~Praca();

	static Praca * getInstance();

	bool setup(){}

	void poll();

};

#endif /* PRACA_H_ */
