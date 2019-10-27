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
//#include "HMI.h"

class Menu {


public:

//  static constexpr char CHAR_OPEN = 'O';


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

  static constexpr uint32_t REFRESH_DELAY_MS = 338;

 // Keyboard * keys;
  FrameBuffer * lcd;

  uint32_t tmpValue = 0;  // do przechowania wartosci tymczasowych

  uint32_t refreshDelay;

  EKRAN ekran;
  uint16_t editValue = 0;
  bool editMode = false;

public:

  void init(FrameBuffer * pLcd){
//    keys = keys;
    lcd = pLcd;

    ekran = EKRAN::e_INIT;
    //goToEkran(EKRAN::e_INIT);
  }


  void goToEkran(EKRAN nowyEkran);

  void printPattern(const char * pattern, uint32_t value);

  bool processHaslo(Keyboard::Key key);

  //bool edit(Keyboard::Key key);

  void showEkran();

  void poll();


};


#endif /* MENU_H_ */
