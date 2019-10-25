/*
 * Praca.cpp
 *
 *  Created on: 25 paź 2019
 *      Author: lechu
 */

#include <Praca.h>
#include "main.h"


Praca praca = Praca();
Praca * Praca::getInstance(){
	return &praca;
}

void pollPraca(){
	Praca::getInstance()->poll();
}

QuickTask taskPraca = QuickTask(QuickTask::QTType::QT_PERIODIC, pollPraca, Praca::PRACA_POLL_INTERVAL_MILIS);


Praca::Praca() {
	// TODO Auto-generated constructor stub

}

Praca::~Praca() {
	// TODO Auto-generated destructor stub
}

void Praca::poll(){
	char znak = keys->getCharKey();
	if (znak != '\0'){
		lcd->print(znak);
	}
}
