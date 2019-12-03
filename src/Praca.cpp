/*
 * Praca.cpp
 *
 *  Created on: 25 paź 2019
 *      Author: lechu
 */

#include <Praca.h>
#include "main.h"

void pollPraca();

Praca praca = Praca();
Praca * Praca::getInstance(){
	return &praca;
}

void pollPraca(){
	Praca::getInstance()->poll();
}

QuickTask taskPraca = QuickTask(QuickTask::QTType::QT_PERIODIC, pollPraca, Praca::POLL_PERIOD_MS);

bool Praca::setup(){
	return stopWork();;
}

bool Praca::startWork(){
  countToGo = VEprom::readWord(VEprom::VirtAdres::ILOSC);
  startWorkCycle();
  return true;
}

bool Praca::startWorkCycle(){
	knife->stop();
	krokowy->go(VEprom::readWord(VEprom::VirtAdres::DLUGOSC));
	return true;
}

bool Praca::stopWork(){
	krokowy->stop();
	knife->stop();
	countToGo = VEprom::readWord(VEprom::VirtAdres::ILOSC);
	return true;
}

bool Praca::isAllDone(){
	if (countToGo > 0){
		return false;
	}
	stopWork();
	return true;

}

void Praca::poll(){

	if (isStartup()){
		startupDelayMs += POLL_PERIOD_MS;
	}

	// wlaczac podswietlenie na TIME_BACKLIGHT_ACTIVE_MS milisekund po nacisnieciu jakiegos klawisza
	if (keys->getUnbufferedKey() !=  Keyboard::Key::NONE){
		backlightTimeMs = 0;
	}

	if (backlightTimeMs < TIME_BACKLIGHT_ACTIVE_MS){
		backlightTimeMs += POLL_PERIOD_MS;
		lcd->setBackLight(true);
	}else {
		lcd->setBackLight(false);
	}

}
