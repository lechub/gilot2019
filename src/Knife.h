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
  static constexpr uint32_t POLL_PERIOD_MS = 5;
protected:

  Gpio * gpioAction;
  Gpio * gpioDetect;

  bool work = false;
  bool goingDown = true;
  bool error = false;
  uint32_t timeout = 0;

  bool isUp(){
    return gpioDetect->getInput() == Gpio::Level::High;
  }

  void setCuttingMove(bool action){
    if (action){
      gpioAction->setOutput(Gpio::Level::High);
    }else{
      gpioAction->setOutput(Gpio::Level::Low);
    }
  }

  bool isCuttingMove(){
    return gpioAction->getOutput() == Gpio::Level::High;
  }

public:
  Knife(Gpio * knifeAction, Gpio * knifeDetect) :
    gpioAction(knifeAction), gpioDetect(knifeDetect){	}

  virtual ~Knife(){;}

  void setup(){
    gpioAction->setup(Gpio::GpioMode::OUTPUT, Gpio::GpioOType::PushPull, Gpio::GpioPuPd::NoPull, Gpio::GpioSpeed::HighSpeed);
    gpioDetect->setup(Gpio::GpioMode::INPUT, Gpio::GpioOType::PushPull, Gpio::GpioPuPd::NoPull, Gpio::GpioSpeed::HighSpeed);
  }

  void start(){
    error = false;
    work = true;
    timeout = 0;
    goingDown = true;
    setCuttingMove(true);
  }

  void stop(){
    work = false;
  }


  bool isStopped(){
    if (isCuttingMove()) return false;
    return work == false;
  }

  bool isInError()const{
    return error;
  }

  void poll(){

    if(timeout < TIMEOUT_MS){
      timeout += POLL_PERIOD_MS;
    }else{
      stop();
      setCuttingMove(false);  // awaryjne zatrzymanie
      error = true;
    }

    if (work){
      if (goingDown){
        if (!isUp()){
          if (timeout > 500){
            goingDown = false;
          }
        }
      }else{	// going up
        if (isUp()){
          timeout = 0;
          goingDown = true;
          stop();
        }
      }
    }else{
      if (isUp()){
        setCuttingMove(false);  //normalne zatrzymanie
        timeout = 0;
      }else{
        setCuttingMove(true);
      }
    }
  }

};

#endif /* KNIFE_H_ */
