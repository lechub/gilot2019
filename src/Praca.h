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
	static constexpr uint32_t CZAS_WYJSCIA_Z_MENU_MS = 30 * 1000;	// pol minuty
	static constexpr uint32_t TIME_PERIOD_MONITOR_MS = 12;

	static constexpr uint32_t TIME_BACKLIGHT_ACTIVE_MS = 2 * 60 * 1000;    // 2 minuty po jakimkolwiek klawiszu
	static constexpr uint32_t TIME_STARTUP_INDICATION_MS = 5000;

private:

	uint32_t startupDelayMs = 0;
	uint32_t backlightTimeMs = 0;

	Gpio * backLight = nullptr;

	uint32_t lengthToGo = 0;
	uint32_t countToGo = 0;

public:

	Praca();
	virtual ~Praca();

	static Praca * getInstance();

	bool setup(){
		return true;
	}

//	void setBackLight(bool enable){ backLight->setOutput(enable); }

	inline bool isStartup(){
		return startupDelayMs < TIME_STARTUP_INDICATION_MS;
	}

//	inline bool isBackLightInUse(){
//		return backLight->getOutput();
//	}

	inline uint32_t getCountToGo()const{ return countToGo; }

	void setLength(uint32_t microm){
		lengthToGo = microm;
	}

	void setQuantity(uint32_t quantity){
		countToGo = quantity;
	}

	bool startWork();
	bool stopWork();

	void poll();

};

#endif /* PRACA_H_ */
