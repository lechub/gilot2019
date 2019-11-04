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
public:
  static constexpr uint32_t TIMEOUT_MS = 4000;
protected:

  Gpio * gpioAction;
  Gpio * gpioDetect;

  bool work = false;
  bool goingDown = true;
  uint32_t timeout = 0;
  uint32_t POLL_PERIOD_MS;

  bool isUp(){
    return gpioDetect->getInput() == Gpio::Level::Low;
  }

public:
  Knife(Gpio * knifeAction, Gpio * knifeDetect, uint32_t poll_PERIOD_MS) :
    gpioAction(knifeAction), gpioDetect(knifeDetect), POLL_PERIOD_MS(poll_PERIOD_MS){

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
        timeout += POLL_PERIOD_MS;
        if ((!isUp())||(timeout > TIMEOUT_MS)){
          timeout = 0;
          goingDown = false;
        }
      }else{	// going up
        timeout += POLL_PERIOD_MS;
        if ((isUp())||(timeout > TIMEOUT_MS)){
          timeout = 0;
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
