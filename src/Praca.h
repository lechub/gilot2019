/*
 * Praca.h
 *
 *  Created on: 25 paź 2019
 *      Author: lechu
 */

#ifndef PRACA_H_
#define PRACA_H_

#include "Settings.h"
#include "Gpio.h"

class Praca {

public:
	static constexpr uint32_t POLL_PERIOD_MS = 121;

	static constexpr uint32_t CZAS_WYJSCIA_Z_MENU_MS = 30 * 1000;	// pol minuty

	static constexpr uint32_t TIME_BACKLIGHT_ACTIVE_MS = 2 * 60 * 1000;    // 2 minuty po jakimkolwiek klawiszu
	static constexpr uint32_t TIME_STARTUP_INDICATION_MS = 5000;

private:

	uint32_t startupDelayMs = 0;
	uint32_t backlightTimeMs = 0;

	Gpio * backLight = nullptr;

	//uint32_t lengthToGo = 0;
	uint32_t countToGo = 0;

public:

	Praca(){;}
	virtual ~Praca(){;}

	static Praca * getInstance();

	bool setup();

	inline bool isStartup(){
		return startupDelayMs < TIME_STARTUP_INDICATION_MS;
	}

	inline uint32_t getCountToGo()const{ return countToGo; }

//	void setLengthToGo(uint32_t microm){
//		lengthToGo = microm;
//	}

//	void setQuantityToGo(uint32_t quantity){
//		countToGo = quantity;
//	}

	bool isAllDone();

	void cycleWasDone(){
		if (countToGo > 0){
			countToGo--;
		}
	}



	bool startWorkCycle();
	bool startWork();
	bool stopWork();

	void poll();

};

#endif /* PRACA_H_ */
