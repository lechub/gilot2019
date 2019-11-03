/*
 * Menu.h
 *
 *  Created on: 04.07.2017
 *      Author: lechu
 */

#ifndef MENU_H_
#define MENU_H_

#include <VEprom.h>
#include "Keyboard.h"
#include "FrameBuffer.h"
#include "Settings.h"
#include "NumberEditor.h"
//#include "HMI.h"

class Menu {

public:

	static constexpr uint32_t POLL_PERIOD_MS = 338;

	typedef enum{
		e_INIT,				// ekran poczatkowy
		e_READY,			// ekran startowy - w oczekiwaniu na klawisz
		e_PRACA_MOTOR,		// ekran gdy obraca silnik
		e_PRACA_KNIFE,		// ekran gdy tnie noz
		e_UST_DLUGOSC,		// ustawienie dlugosci ciecia
		e_UST_ILOSC,		// ustawienie ilosci blaszek
		e_UST_KALIBR,		// ustawienie danych kalibracyjnych
		e_DEBUG,			// podglad roznycvh rzeczy
	}EKRAN;

private:
	NumberEditor neDlugosc = NumberEditor("DLUGOSC:####0 mm",1);
	NumberEditor neIlosc   = NumberEditor("ILOSC:####0 szt.",1);
	NumberEditor neKalibr  = NumberEditor("KALIBRAC.:##0 mm",1);
	NumberEditor neToGo 	  = NumberEditor(" [##0] ", 1);
	NumberEditor neToCut 	  = NumberEditor(        "<####0> ", 1);
	FrameBuffer * lcd;
	EKRAN ekran;

public:

	void init(FrameBuffer * pLcd);


	void goToEkran(EKRAN nowyEkran);

	void printPattern(const char * pattern, uint32_t value);

	bool processHaslo(Keyboard::Key key);

	void showEkran();

	void poll();

};


#endif /* MENU_H_ */
