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
  typedef enum{
    NONE,
    FORWARD,
    BACK,
  }DIRECTION;

private:
  Gpio * gpioStep;
  Gpio * gpioDir;
  Gpio * gpioEnable;
  Gpio * gpioSleep;
  Gpio * gpioMs1;
  Gpio * gpioMs2;

  Krokowy(Gpio * step, Gpio * dir, Gpio * enable, Gpio * sleep, Gpio * ms1, Gpio * ms2)
  :gpioStep(step), gpioDir(dir), gpioEnable(enable),  gpioSleep(sleep), gpioMs1(ms1), gpioMs2(ms2) {
//    gpioStep;
//      gpioDir;
//      gpioEnable;
//      gpioSleep;
//      gpioM1;
//      gpioMs2;
  }

  bool isStopped();
  bool stop();
  bool go(uint32_t length, DIRECTION);

};

#endif /* KROKOWY_H_ */
