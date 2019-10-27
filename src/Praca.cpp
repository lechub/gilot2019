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

QuickTask taskPraca = QuickTask(QuickTask::QTType::QT_PERIODIC, pollPraca, Praca::TIME_PERIOD_MONITOR_MS);


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


	// z HMI

	   // Po uruchomieniu mrugamy diodami przez @HMI::TIME_STARTUP_INDICATION_MS
	   if (isStartup()){
	     startupDelayMs += TIME_PERIOD_MONITOR_MS;
	   }

	   // wlaczac podswietlenie na TIME_BACKLIGHT_ACTIVE_MS milisekund po nacisnieciu jakiegos klawisza
	   if (keys->getUnbufferedKey() !=  Keyboard::Key::NONE){
	     backlightTimeMs = 0;
	   }

	   if (backlightTimeMs < TIME_BACKLIGHT_ACTIVE_MS){
	     backlightTimeMs += TIME_PERIOD_MONITOR_MS;
	     lcd->setBackLight(true);
	   }else {
	     lcd->setBackLight(false);
	   }

	   // reszta kolejki odpytywania
//	   keyboard->poll();
//	   menu->poll();
}
