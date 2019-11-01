/*
 * Krokowy.h
 *
 *  Created on: 18 paź 2019
 *      Author: lechu
 */

#ifndef KROKOWY_H_
#define KROKOWY_H_

#include "Pinout.h"


class Krokowy {


public:

	static constexpr uint32_t REFRESH_INTERVAL_MILISECOND = 1;

	static constexpr uint32_t STEP_ANGLE_MILI = 1800; // 1,8 stopnia
	static constexpr uint32_t STEPS_FULL_ROTATION = (360 * 1000) / STEP_ANGLE_MILI;
	static constexpr uint32_t STEPS_DIV8_FULL_ROTATION = STEPS_FULL_ROTATION * 8;
	static constexpr uint32_t DIAMETER_UM = 12 * 1000; // 12 mm średnica walu
	static constexpr uint32_t LENGTH_PER_FULL_ROTATION_UM = (DIAMETER_UM * 3141 )/1000; // 37 mm obwód walu w um
	static constexpr uint32_t STEP_DIV8_MICROMETERS = (STEPS_DIV8_FULL_ROTATION*1000)/LENGTH_PER_FULL_ROTATION_UM;

	static constexpr uint32_t ROZBIEG_MAX_DELAY = 10; // na poczatku 10 razy wolniej;
	static constexpr uint32_t ROZBIEG_STEPS = 10 * 1000 / STEP_DIV8_MICROMETERS;


	typedef enum{
		FORWARD,
		BACK,
	}DIRECTION;

	typedef enum{
		FULL,
		HALF,
		QUARTER,
		EIGHT,
	}StepMode;


private:
	Gpio * gpioStep;
	Gpio * gpioDir;
	Gpio * gpioEnable;
	Gpio * gpioSleep;
	Gpio * gpioMs1;
	Gpio * gpioMs2;

	volatile bool enabled = false;

	volatile uint32_t lengthToGo = 0;	//mikrometry do przesuniecia
	volatile uint32_t lengthFromStart = 0;	//mikrometry przesuniete
	volatile uint32_t rozbiegDelay = 0;

	inline void setDir(DIRECTION dir){
		gpioDir->setOutput(dir == DIRECTION::BACK ? Gpio::Level::Low : Gpio::Level::High);
	}

	inline void enable(bool enabledState){
		if (enabledState != true){
			enabled = false;
			gpioEnable->setOutputUp();
		}else{
			gpioEnable->setOutputDown();
			enabled = true;
		}
	}

public:
	Krokowy(Gpio * step, Gpio * dir, Gpio * enable, Gpio * sleep, Gpio * ms1, Gpio * ms2)
:gpioStep(step), gpioDir(dir), gpioEnable(enable),  gpioSleep(sleep), gpioMs1(ms1), gpioMs2(ms2) {
	}

	static Krokowy * getInstance();


	bool setStepMode(StepMode mode);
	bool isStopped();
	bool stop();
	bool go(uint32_t length_mm);
	inline uint32_t getLengthToGo()const{ return lengthToGo/1000; }
	bool setup();



	void poll();


};

#endif /* KROKOWY_H_ */
