/*
 * Krokowy.cpp
 *
 *  Created on: 18 paź 2019
 *      Author: lechu
 */

#include <Krokowy.h>
#include "main.h"

void krokPollCallback();

Krokowy * Krokowy::getInstance(){
	return krokowy;
}


void krokPollCallback(){
	Krokowy::getInstance()->poll();
}

// periodycznie wykonywana funkcja monitor() opakowana w aku_callback()
QuickTask krokowyTask(QuickTask::QT_PERIODIC, krokPollCallback, Krokowy::REFRESH_INTERVAL_MILISECOND);

void Krokowy::poll(){
	if (lengthToGo == 0){		// zatrzymac bo koniec
		enable(false);
		return;
	}
	if(gpioStep->getOutput() == Gpio::Level::High){		// zbocze opadajace i przeliczenie odleglosci w nastepnym kroku
		gpioStep->setOutput(Gpio::Level::Low);

		lengthToGo = (lengthToGo > STEP_DIV8_MICROMETERS) ? lengthToGo - STEP_DIV8_MICROMETERS : 0;
		lengthFromStart += STEP_DIV8_MICROMETERS;

		uint32_t length = lengthToGo < lengthFromStart ? lengthToGo : lengthFromStart;	// mniejsza z wartosci
		uint32_t delay = length/(ROZBIEG_STEPS/ROZBIEG_MAX_DELAY);
		rozbiegDelay  = delay >= ROZBIEG_MAX_DELAY ? 0 : ROZBIEG_MAX_DELAY - delay;
		return;
	}
	if (rozbiegDelay > ROZBIEG_MAX_DELAY) rozbiegDelay = ROZBIEG_MAX_DELAY;
	if (rozbiegDelay == 0){
		gpioStep->setOutput(Gpio::Level::High);		// nastepny krok
	}else{
		rozbiegDelay--;								// albo tylko odczekanie dla wydluzenia kroku
	}

}

bool Krokowy::go(uint32_t length_mm){
	enable(false);
	uint32_t len_um = length_mm * 1000;
	lengthFromStart = 0;
	lengthToGo = len_um;
	enable(true);
	gpioStep->setOutput(Gpio::Level::High);	// poczatek 1 kroku
	return true;
}

bool Krokowy::stop(){
	enable(false);
	lengthFromStart = lengthToGo = 0;
	return true;
}

bool Krokowy::setup(){
	gpioDir->setup(Gpio::GpioMode::OUTPUT, Gpio::GpioOType::PushPull, Gpio::GpioPuPd::NoPull, Gpio::GpioSpeed::HighSpeed);
	gpioStep->setupFromClone(gpioDir);
	gpioEnable->setupFromClone(gpioDir);
	gpioMs1->setupFromClone(gpioDir);
	gpioMs2->setupFromClone(gpioDir);
	gpioSleep->setupFromClone(gpioDir);

	gpioStep->setOutputDown();
	gpioDir->setOutputDown();
	gpioEnable->setOutputUp(); //na razie zablokowany
	gpioSleep->setOutputDown();

	setStepMode(StepMode::EIGHT);
	lengthFromStart = lengthToGo = 0;

	return true;
}


bool Krokowy::setStepMode(StepMode mode){
	switch(mode){
	case StepMode::FULL:
		gpioMs1->setOutputDown();
		gpioMs2->setOutputDown();
		break;
	case StepMode::HALF:
		gpioMs1->setOutputUp();
		gpioMs2->setOutputDown();
		break;
	case StepMode::QUARTER:
		gpioMs1->setOutputDown();
		gpioMs2->setOutputUp();
		break;
	case StepMode::EIGHT:
		gpioMs1->setOutputUp();
		gpioMs2->setOutputUp();
		break;
	default:
		return false;
		break;
	}
	return true;
}
