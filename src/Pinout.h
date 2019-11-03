/*
 * Pinout.h
 *
 *      Author: lechu
 */

#ifndef PINOUT_H_
#define PINOUT_H_

#include "Gpio.h"


class Pinout {

public:

  //Wyjscia
  Gpio lcd_RS       = Gpio(GPIOA, 12);
  Gpio lcd_RW       = Gpio(GPIOF, 6);
  Gpio lcd_E        = Gpio(GPIOF, 7);
  Gpio lcd_bckLight = Gpio(GPIOC, 13);
  Gpio lcd_D0       = Gpio(GPIOA, 15);
  Gpio lcd_D1       = Gpio(GPIOB, 3);
  Gpio lcd_D2       = Gpio(GPIOB, 4);
  Gpio lcd_D3       = Gpio(GPIOB, 5);
  Gpio lcd_D4       = Gpio(GPIOB, 6);
  Gpio lcd_D5       = Gpio(GPIOB, 7);
  Gpio lcd_D6       = Gpio(GPIOB, 8);
  Gpio lcd_D7       = Gpio(GPIOB, 9);
  //   Gpio lcd_D1        = Gpio(GPIOB, 3);

  Gpio keyb_W1      = Gpio(GPIOB, 12);
  Gpio keyb_W2      = Gpio(GPIOB, 13);
  Gpio keyb_W3      = Gpio(GPIOB, 14);
  Gpio keyb_W4      = Gpio(GPIOB, 15);
  Gpio keyb_K1      = Gpio(GPIOA, 8);
  Gpio keyb_K2      = Gpio(GPIOA, 9);
  Gpio keyb_K3      = Gpio(GPIOA, 10);

  Gpio noz_Action   = Gpio(GPIOA, 0);
  Gpio noz_detect   = Gpio(GPIOA, 1);

  Gpio mot_enable   = Gpio(GPIOA, 2);
  Gpio mot_ms1      = Gpio(GPIOA, 3);
  Gpio mot_ms2      = Gpio(GPIOA, 4);
  Gpio mot_sleep    = Gpio(GPIOA, 5);
  Gpio mot_step     = Gpio(GPIOA, 6);
  Gpio mot_dir      = Gpio(GPIOA, 7);




  void setup(){

    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN
        |RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOFEN
        ;
    READ_REG_AND_FORGET(RCC->AHBENR);

  }

};

extern Pinout * pins;

#endif /* PINOUT_H_ */
