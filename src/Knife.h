/*
 * Knife.h
 *
 *  Created on: 3 lis 2019
 *      Author: lechu
 */

#ifndef KNIFE_H_
#define KNIFE_H_

#include "Gpio.h"


class Knife {

protected:

	Gpio * gpioAction;
	Gpio * gpioDetect;

	bool work = false;
	bool goingDown = true;

	bool isUp(){
		return gpioDetect->getInput() == Gpio::Level::Low;
	}

public:
	Knife(Gpio * knifeAction, Gpio * knifeDetect) : gpioAction(knifeAction), gpioDetect(knifeDetect){

	}

	virtual ~Knife(){;}

	void setup(){
		gpioAction->setup(Gpio::GpioMode::OUTPUT, Gpio::GpioOType::PushPull, Gpio::GpioPuPd::NoPull, Gpio::GpioSpeed::HighSpeed);
		gpioDetect->setup(Gpio::GpioMode::INPUT, Gpio::GpioOType::PushPull, Gpio::GpioPuPd::NoPull, Gpio::GpioSpeed::HighSpeed);
	}

	void start(){
		work = true;
		goingDown = true;
		gpioAction->setOutput(Gpio::Level::High);
	}
	void stop(){
		gpioAction->setOutput(Gpio::Level::Low);
		work = false;
	}

	bool isStopped(){
		return work == false;
	}

	void poll(){
		if (work){
			if (goingDown){
				if (!isUp()){
					goingDown = false;
				}
			}else{	// going up
				if (isUp()){
					goingDown = true;
					stop();
				}
			}
		}else{
			stop();
		}
	}

};

#endif /* KNIFE_H_ */
